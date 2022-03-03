function [image_withLine] = DrawCenterLine(class_Name,image)
    image_withLine = image;
    if strcmp(class_Name,'左弯') || strcmp(class_Name,'右弯') || strcmp(class_Name,'出左环岛') || strcmp(class_Name,'出右环岛')
        filter = -0.5;
        search_Lines = size(image,1);
        
        start_Row = size(image,1);
        start_Col = [ceil(size(image,2)/2),ceil(size(image,2)/2)];
        Col_Left = -2*ones(1,search_Lines);
        Col_Right = -2*ones(1,search_Lines);
        Col_Center = -2*ones(1,search_Lines);
        for i=1:search_Lines
            if image(start_Row,start_Col(1)) == 255 || image(start_Row,start_Col(2)) == 255          
                start_Row = start_Row - 1;
                continue;
            end
            if image(start_Row,start_Col(1)) == 1            
                while image(start_Row,start_Col(1)) == 1 && start_Col(1)>=1+1
                    start_Col(1) = start_Col(1) - 1;
                end
                start_Col(1) = start_Col(1) + 1;
                if image(start_Row,start_Col(1)-1) == 0                    
                    Col_Left(i) = start_Col(1);
                end
            else
                while image(start_Row,start_Col(1)) ~= 1 && start_Col(1)<=size(image,2)-1
                    start_Col(1) = start_Col(1) + 1;
                end
                if image(start_Row,start_Col(1)) == 1
                    Col_Left(i) = start_Col(1);
                end
            end

            if image(start_Row,start_Col(2)) ~= 1            
                while image(start_Row,start_Col(2)) ~= 1 && start_Col(2)>=1+1
                    start_Col(2) = start_Col(2) - 1;
                end      
                if image(start_Row,start_Col(2)) == 1
                    Col_Right(i) = start_Col(2);
                end
            else
                while image(start_Row,start_Col(2)) == 1 && start_Col(2)<=size(image,2)-1
                    start_Col(2) = start_Col(2) + 1;
                end
                start_Col(2) = start_Col(2) - 1;
                if image(start_Row,start_Col(2)+1) == 0                   
                    Col_Right(i) = start_Col(2);
                end
            end
            start_Row = start_Row - 1;
            if Col_Right(i)~=-2 && Col_Left(i)~= -2
                if Col_Center(i-1)~=-2
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*0.5*(Col_Right(i) + Col_Left(i));
                else
                    Col_Center(i) = filter*ceil(size(image,2)/2)+(1-filter)*0.5*(Col_Right(i) + Col_Left(i));
                end
            elseif Col_Right(i)~=-2 && Col_Left(i)==-2
                if Col_Center(i-1)~=-2 && Col_Right(i-1)~=-2 
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*(Col_Right(i)-Col_Right(i-1)+Col_Center(i-1));
                elseif Col_Center(i-1)==-2 && Col_Right(i-1)~=-2 
                    Col_Center(i) = filter*ceil(size(image,2)/2)+(1-filter)*(Col_Right(i)-Col_Right(i-1)+ceil(size(image,2)/2));
                elseif Col_Center(i-1)~=-2 && Col_Right(i-1)==-2 
                    Col_Center(i) = Col_Center(i-1);
                else
                    Col_Center(i) = ceil(size(image,2)/2);
                end
            elseif Col_Right(i)==-2 && Col_Left(i)~=-2
                if Col_Center(i-1)~=-2 && Col_Left(i-1)~=-2 
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*(Col_Left(i)-Col_Left(i-1)+Col_Center(i-1));
                elseif Col_Center(i-1)==-2 && Col_Left(i-1)~=-2 
                    Col_Center(i) = filter*ceil(size(image,2)/2)+(1-filter)*(Col_Left(i)-Col_Left(i-1)+ceil(size(image,2)/2));
                elseif Col_Center(i-1)~=-2 && Col_Left(i-1)==-2 
                    Col_Center(i) = Col_Center(i-1);
                else
                    Col_Center(i) = ceil(size(image,2)/2);
                end
            else
                if Col_Center(i-1)~=-2
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*ceil(size(image,2)/2);
                else
                    Col_Center(i) = ceil(size(image,2)/2);
                end
            end
        end
        

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
    elseif strcmp(class_Name,'十字路口')
        filter = 0.7;
        search_Lines = size(image,1);
        
        start_Row = size(image,1);
        start_Col = [ceil(size(image,2)/2),ceil(size(image,2)/2)];
        Col_Left = -2*ones(1,search_Lines);
        Col_Right = -2*ones(1,search_Lines);
        Col_Center = -2*ones(1,search_Lines);
        for i=1:search_Lines
            if image(start_Row,start_Col(1)) == 255 || image(start_Row,start_Col(2)) == 255          
                start_Row = start_Row - 1;
                continue;
            end
            if image(start_Row,start_Col(1)) == 1            
                while image(start_Row,start_Col(1)) == 1 && start_Col(1)>=1+1
                    start_Col(1) = start_Col(1) - 1;
                end
                start_Col(1) = start_Col(1) + 1;
                if image(start_Row,start_Col(1)-1) == 0                    
                    Col_Left(i) = start_Col(1);
                end
            else
                while image(start_Row,start_Col(1)) ~= 1 && start_Col(1)<=size(image,2)-1
                    start_Col(1) = start_Col(1) + 1;
                end
                if image(start_Row,start_Col(1)) == 1
                    Col_Left(i) = start_Col(1);
                end
            end

            if image(start_Row,start_Col(2)) ~= 1            
                while image(start_Row,start_Col(2)) ~= 1 && start_Col(2)>=1+1
                    start_Col(2) = start_Col(2) - 1;
                end      
                if image(start_Row,start_Col(2)) == 1
                    Col_Right(i) = start_Col(2);
                end
            else
                while image(start_Row,start_Col(2)) == 1 && start_Col(2)<=size(image,2)-1
                    start_Col(2) = start_Col(2) + 1;
                end
                start_Col(2) = start_Col(2) - 1;
                if image(start_Row,start_Col(2)+1) == 0                   
                    Col_Right(i) = start_Col(2);
                end
            end
            start_Row = start_Row - 1;
            if Col_Right(i)~=-2 && Col_Left(i)~= -2
                if Col_Center(i-1)~=-2
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*0.5*(Col_Right(i) + Col_Left(i));
                else
                    Col_Center(i) = filter*ceil(size(image,2)/2)+(1-filter)*0.5*(Col_Right(i) + Col_Left(i));
                end
            elseif Col_Right(i)~=-2 && Col_Left(i)==-2
                if Col_Center(i-1)~=-2 && Col_Right(i-1)~=-2 
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*(Col_Right(i)-Col_Right(i-1)+Col_Center(i-1));
                elseif Col_Center(i-1)==-2 && Col_Right(i-1)~=-2 
                    Col_Center(i) = filter*ceil(size(image,2)/2)+(1-filter)*(Col_Right(i)-Col_Right(i-1)+ceil(size(image,2)/2));
                elseif Col_Center(i-1)~=-2 && Col_Right(i-1)==-2 
                    Col_Center(i) = Col_Center(i-1);
                else
                    Col_Center(i) = ceil(size(image,2)/2);
                end
            elseif Col_Right(i)==-2 && Col_Left(i)~=-2
                if Col_Center(i-1)~=-2 && Col_Left(i-1)~=-2 
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*(Col_Left(i)-Col_Left(i-1)+Col_Center(i-1));
                elseif Col_Center(i-1)==-2 && Col_Left(i-1)~=-2 
                    Col_Center(i) = filter*ceil(size(image,2)/2)+(1-filter)*(Col_Left(i)-Col_Left(i-1)+ceil(size(image,2)/2));
                elseif Col_Center(i-1)~=-2 && Col_Left(i-1)==-2 
                    Col_Center(i) = Col_Center(i-1);
                else
                    Col_Center(i) = ceil(size(image,2)/2);
                end
            else
                if Col_Center(i-1)~=-2
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*ceil(size(image,2)/2);
                else
                    Col_Center(i) = ceil(size(image,2)/2);
                end
            end
        end
        

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

