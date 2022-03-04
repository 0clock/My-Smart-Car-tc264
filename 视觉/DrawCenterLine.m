function [image_withLine] = DrawCenterLine(class_Name,image)
% 找中心线算法，输入赛道类型class_Name和逆透视二值化图片image，输出绘制有左线、右线、中心线的逆透视二值化图片image_withLine
    image_withLine = image;
    % 对于左弯、右弯，可以采用，特征是滤波是负数，用于超前转向，以免冲出弯道
    if strcmp(class_Name,'左弯') || strcmp(class_Name,'右弯') 
        filter = -0.5;%滤波系数，正数时是低通滤波，负数时相当于高通滤波
        search_Lines = size(image,1);%一共要扫描多少行，最大是图片宽
        
        start_Row = size(image,1);%标记当前在处理哪一行，从最后一行开始
        start_Col = [ceil(size(image,2)/2),ceil(size(image,2)/2)];%标记当前在处理哪一列，start_Col(1)指左线，start_Col(2)指右线，默认从中心开始
        Col_Left = -2*ones(1,search_Lines);%按从下往上的顺序存储左线的列号结果，不合法的全部为-2
        Col_Right = -2*ones(1,search_Lines);%按从下往上的顺序存储右线的列号结果，不合法的全部为-2
        Col_Center = -2*ones(1,search_Lines);%按从下往上的顺序存储中心线线的列号结果，不合法的全部为-2
        for i=1:search_Lines%从最后一行开始逐行扫描，一共扫描search_Lines行
            if (image(start_Row,start_Col(1)) == 255 || image(start_Row,start_Col(2)) == 255 ) || start_Col(1)>start_Col(2)       
                start_Row = start_Row - 1;
                continue;
            end%在当前的行里，如果左线或右线有一个是255区域（未知区域），说明还没有进入到真正的视角区域（0区域或1区域）；或者如果左线跑到右线的右边去了，则说明不是道路了
            if image(start_Row,start_Col(1)) == 1 %如果左线发现1区域（道路）           
                while image(start_Row,start_Col(1)) == 1 && start_Col(1)>=1+1
                    start_Col(1) = start_Col(1) - 1;
                end
                start_Col(1) = start_Col(1) + 1;%则左线持续向左扫描直到不再是1区域（道路），有可能是0或255区域
                if image(start_Row,start_Col(1)-1) == 0 %查看此时是否是0区域（背景）                   
                    Col_Left(i) = start_Col(1); %只有是0区域的才可以将列号存储到左线里
                end
            else%如果左线发现0区域（背景）   
                while image(start_Row,start_Col(1)) == 0 && start_Col(1)<=size(image,2)-1
                    start_Col(1) = start_Col(1) + 1;
                end%则左线持续向右扫描直到不再是0区域（背景），有可能是1或255区域
                if image(start_Row,start_Col(1)) == 1 %查看此时是否是1区域（道路）      
                    Col_Left(i) = start_Col(1); %只有是1区域的才可以将列号存储到左线里
                end
            end

            if image(start_Row,start_Col(2)) == 0 %如果右线发现0区域（背景）                  
                while image(start_Row,start_Col(2)) == 0 && start_Col(2)>=1+1
                    start_Col(2) = start_Col(2) - 1;
                end      %则右线持续向左扫描直到不再是0区域（背景），有可能是1或255区域
                if image(start_Row,start_Col(2)) == 1
                    Col_Right(i) = start_Col(2); %只有是1区域的才可以将列号存储到右线里
                end
            else%如果右线发现1区域（道路）     
                while image(start_Row,start_Col(2)) == 1 && start_Col(2)<=size(image,2)-1
                    start_Col(2) = start_Col(2) + 1;
                end
                start_Col(2) = start_Col(2) - 1; %则右线持续向右扫描直到不再是1区域（道路），有可能是0或255区域
                if image(start_Row,start_Col(2)+1) == 0                   
                    Col_Right(i) = start_Col(2); %只有是0区域的才可以将列号存储到右线里
                end
            end
            start_Row = start_Row - 1; %左右线扫描完毕，标记行进入上一行，给下一次左右线扫描做准备
            
            %下面是中心线计算   
            %中心线计算有4种情况：左线合法(~=-2)或非法(==-2)）、右线合法(~=-2)或非法(==-2)）
            if Col_Right(i)~=-2 && Col_Left(i)~= -2 %左线合法，右线合法
                if Col_Center(i-1)~=-2 %如果上一个中心线也合法
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*0.5*(Col_Right(i) + Col_Left(i)); %根据上一次的中心线、这一次左右线中值，用滤波计算这次的中心线
                else %如果上一个中心线不合法
                    Col_Center(i) = filter*ceil(size(image,2)/2)+(1-filter)*0.5*(Col_Right(i) + Col_Left(i)); %假定上一次中心线在正中间，根据上一次中心线、这一次左右线中值，用滤波计算这次的中心线
                end
            elseif Col_Right(i)~=-2 && Col_Left(i)==-2 %左线非法，右线合法
                if Col_Center(i-1)~=-2 && Col_Right(i-1)~=-2 %如果上一次中心线合法，上一次右线合法
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*(Col_Right(i)-Col_Right(i-1)+Col_Center(i-1)); %根据上一次的中心线、假定的这一次中心线（保持上一次和右线的距离），用滤波计算这次的中心线
                elseif Col_Center(i-1)==-2 && Col_Right(i-1)~=-2 %如果上一次中心线非法，上一次右线合法
                    Col_Center(i) = filter*ceil(size(image,2)/2)+(1-filter)*(Col_Right(i)-Col_Right(i-1)+ceil(size(image,2)/2));%假定上一次中心线在正中间，根据上一次的中心线、假定的这一次中心线（保持上一次和右线的距离），用滤波计算这次的中心线
                elseif Col_Center(i-1)~=-2 && Col_Right(i-1)==-2 %如果上一次中心线合法，上一次右线非法
                    Col_Center(i) = Col_Center(i-1); %这一次中心线继续使用上一次的中心线
                else %如果上一次中心线非法，上一次右线非法
                    Col_Center(i) = ceil(size(image,2)/2); %这一次中心线直接假定为正中间
                end
            elseif Col_Right(i)==-2 && Col_Left(i)~=-2 %左线合法，右线非法，与上文类似
                if Col_Center(i-1)~=-2 && Col_Left(i-1)~=-2 
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*(Col_Left(i)-Col_Left(i-1)+Col_Center(i-1));
                elseif Col_Center(i-1)==-2 && Col_Left(i-1)~=-2 
                    Col_Center(i) = filter*ceil(size(image,2)/2)+(1-filter)*(Col_Left(i)-Col_Left(i-1)+ceil(size(image,2)/2));
                elseif Col_Center(i-1)~=-2 && Col_Left(i-1)==-2 
                    Col_Center(i) = Col_Center(i-1);
                else
                    Col_Center(i) = ceil(size(image,2)/2);
                end
            else %左线非法，右线非法
                if Col_Center(i-1)~=-2 %如果上一次中心线合法
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*ceil(size(image,2)/2); %根据上一次的中心线、假定的这一次中心线（取正中间），用滤波计算这次的中心线
                else %如果上一次中心线非法
                    Col_Center(i) = ceil(size(image,2)/2); %这一次中心线直接假定为正中间
                end
            end
        end
        %中心线计算完毕
        
        %下面是把左线、中心线、右线画到image_withLine图里
        %左右线取2（稍暗），中心线取3（最亮）
        for i=1:search_Lines
            if Col_Left(i)~=-2
                image_withLine(size(image,1)+1-i,Col_Left(i)) = 2;
            end
            
            if Col_Right(i)~=-2
                image_withLine(size(image,1)+1-i,Col_Right(i)) = 2;
            end
            
            if round(Col_Center(i))~=-2
                if round(Col_Center(i))>=1 && round(Col_Center(i)) <= size(image,2)
                    image_withLine(size(image,1)+1-i,round(Col_Center(i))) = 3;
                end
            end            
        end
    % 对于十字路口、直道，可以采用，特征是滤波是较大正数，用于避免中心线有过大的波动
    elseif strcmp(class_Name,'十字路口') || strcmp(class_Name,'直道')
        filter = 0.7;%滤波系数，正数时是低通滤波，负数时相当于高通滤波
        search_Lines = size(image,1);%一共要扫描多少行，最大是图片宽
        
        start_Row = size(image,1);%标记当前在处理哪一行，从最后一行开始
        start_Col = [ceil(size(image,2)/2),ceil(size(image,2)/2)];%标记当前在处理哪一列，start_Col(1)指左线，start_Col(2)指右线，默认从中心开始
        Col_Left = -2*ones(1,search_Lines);%按从下往上的顺序存储左线的列号结果，不合法的全部为-2
        Col_Right = -2*ones(1,search_Lines);%按从下往上的顺序存储右线的列号结果，不合法的全部为-2
        Col_Center = -2*ones(1,search_Lines);%按从下往上的顺序存储中心线线的列号结果，不合法的全部为-2
        for i=1:search_Lines%从最后一行开始逐行扫描，一共扫描search_Lines行
            if (image(start_Row,start_Col(1)) == 255 || image(start_Row,start_Col(2)) == 255 ) || start_Col(1)>start_Col(2)               
                start_Row = start_Row - 1;
                continue;
            end%在当前的行里，如果左线或右线有一个是255区域（未知区域），说明还没有进入到真正的视角区域（0区域或1区域）
            if image(start_Row,start_Col(1)) == 1 %如果左线发现1区域（道路）           
                while image(start_Row,start_Col(1)) == 1 && start_Col(1)>=1+1
                    start_Col(1) = start_Col(1) - 1;
                end
                start_Col(1) = start_Col(1) + 1;%则左线持续向左扫描直到不再是1区域（道路），有可能是0或255区域
                if image(start_Row,start_Col(1)-1) == 0 %查看此时是否是0区域（背景）                   
                    Col_Left(i) = start_Col(1); %只有是0区域的才可以将列号存储到左线里
                end
            else%如果左线发现0区域（背景）   
                while image(start_Row,start_Col(1)) == 0 && start_Col(1)<=size(image,2)-1
                    start_Col(1) = start_Col(1) + 1;
                end%则左线持续向右扫描直到不再是0区域（背景），有可能是1或255区域
                if image(start_Row,start_Col(1)) == 1 %查看此时是否是1区域（道路）      
                    Col_Left(i) = start_Col(1); %只有是1区域的才可以将列号存储到左线里
                end
            end

            if image(start_Row,start_Col(2)) == 0 %如果右线发现0区域（背景）                  
                while image(start_Row,start_Col(2)) == 0 && start_Col(2)>=1+1
                    start_Col(2) = start_Col(2) - 1;
                end      %则右线持续向左扫描直到不再是0区域（背景），有可能是1或255区域
                if image(start_Row,start_Col(2)) == 1
                    Col_Right(i) = start_Col(2); %只有是1区域的才可以将列号存储到右线里
                end
            else%如果右线发现1区域（道路）     
                while image(start_Row,start_Col(2)) == 1 && start_Col(2)<=size(image,2)-1
                    start_Col(2) = start_Col(2) + 1;
                end
                start_Col(2) = start_Col(2) - 1; %则右线持续向右扫描直到不再是1区域（道路），有可能是0或255区域
                if image(start_Row,start_Col(2)+1) == 0                   
                    Col_Right(i) = start_Col(2); %只有是0区域的才可以将列号存储到右线里
                end
            end
            start_Row = start_Row - 1; %左右线扫描完毕，标记行进入上一行，给下一次左右线扫描做准备
            
            %下面是中心线计算   
            %中心线计算有4种情况：左线合法(~=-2)或非法(==-2)）、右线合法(~=-2)或非法(==-2)）
            if Col_Right(i)~=-2 && Col_Left(i)~= -2 %左线合法，右线合法
                if Col_Center(i-1)~=-2 %如果上一个中心线也合法
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*0.5*(Col_Right(i) + Col_Left(i)); %根据上一次的中心线、这一次左右线中值，用滤波计算这次的中心线
                else %如果上一个中心线不合法
                    Col_Center(i) = filter*ceil(size(image,2)/2)+(1-filter)*0.5*(Col_Right(i) + Col_Left(i)); %假定上一次中心线在正中间，根据上一次中心线、这一次左右线中值，用滤波计算这次的中心线
                end
            elseif Col_Right(i)~=-2 && Col_Left(i)==-2 %左线非法，右线合法
                if Col_Center(i-1)~=-2 && Col_Right(i-1)~=-2 %如果上一次中心线合法，上一次右线合法
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*(Col_Right(i)-Col_Right(i-1)+Col_Center(i-1)); %根据上一次的中心线、假定的这一次中心线（保持上一次和右线的距离），用滤波计算这次的中心线
                elseif Col_Center(i-1)==-2 && Col_Right(i-1)~=-2 %如果上一次中心线非法，上一次右线合法
                    Col_Center(i) = filter*ceil(size(image,2)/2)+(1-filter)*(Col_Right(i)-Col_Right(i-1)+ceil(size(image,2)/2));%假定上一次中心线在正中间，根据上一次的中心线、假定的这一次中心线（保持上一次和右线的距离），用滤波计算这次的中心线
                elseif Col_Center(i-1)~=-2 && Col_Right(i-1)==-2 %如果上一次中心线合法，上一次右线非法
                    Col_Center(i) = Col_Center(i-1); %这一次中心线继续使用上一次的中心线
                else %如果上一次中心线非法，上一次右线非法
                    Col_Center(i) = ceil(size(image,2)/2); %这一次中心线直接假定为正中间
                end
            elseif Col_Right(i)==-2 && Col_Left(i)~=-2 %左线合法，右线非法，与上文类似
                if Col_Center(i-1)~=-2 && Col_Left(i-1)~=-2 
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*(Col_Left(i)-Col_Left(i-1)+Col_Center(i-1));
                elseif Col_Center(i-1)==-2 && Col_Left(i-1)~=-2 
                    Col_Center(i) = filter*ceil(size(image,2)/2)+(1-filter)*(Col_Left(i)-Col_Left(i-1)+ceil(size(image,2)/2));
                elseif Col_Center(i-1)~=-2 && Col_Left(i-1)==-2 
                    Col_Center(i) = Col_Center(i-1);
                else
                    Col_Center(i) = ceil(size(image,2)/2);
                end
            else %左线非法，右线非法
                if Col_Center(i-1)~=-2 %如果上一次中心线合法
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*ceil(size(image,2)/2); %根据上一次的中心线、假定的这一次中心线（取正中间），用滤波计算这次的中心线
                else %如果上一次中心线非法
                    Col_Center(i) = ceil(size(image,2)/2); %这一次中心线直接假定为正中间
                end
            end
        end
        %中心线计算完毕
        
        %下面是把左线、中心线、右线画到image_withLine图里
        %左右线取2（稍暗），中心线取3（最亮）
        for i=1:search_Lines
            if Col_Left(i)~=-2
                image_withLine(size(image,1)+1-i,Col_Left(i)) = 2;
            end
            
            if Col_Right(i)~=-2
                image_withLine(size(image,1)+1-i,Col_Right(i)) = 2;
            end
            
            if round(Col_Center(i))~=-2
                if round(Col_Center(i))>=1 && round(Col_Center(i)) <= size(image,2)
                    image_withLine(size(image,1)+1-i,round(Col_Center(i))) = 3;
                end
            end            
        end
    end
        
end

