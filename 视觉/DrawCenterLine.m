function [image_withLine,Col_Center,search_Lines] = DrawCenterLine(class_Name,image)
% ���������㷨��������������class_Name����͸�Ӷ�ֵ��ͼƬimage��������������ߡ����ߡ������ߵ���͸�Ӷ�ֵ��ͼƬimage_withLine
    image_withLine = image;
    % �������䡢���䣬���Բ��ã��������˲��Ǹ��������ڳ�ǰת�����������
    if strcmp(class_Name,'����') || strcmp(class_Name,'����') 
        filter = -0.5;%�˲�ϵ��������ʱ�ǵ�ͨ�˲�������ʱ�൱�ڸ�ͨ�˲�
        search_Lines = size(image,1);%һ��Ҫɨ������У������ͼƬ��
        
        start_Row = size(image,1);%��ǵ�ǰ�ڴ�����һ�У������һ�п�ʼ
        start_Col = [ceil(size(image,2)/2),ceil(size(image,2)/2)];%��ǵ�ǰ�ڴ�����һ�У�start_Col(1)ָ���ߣ�start_Col(2)ָ���ߣ�Ĭ�ϴ����Ŀ�ʼ
        Col_Left = -2*ones(1,search_Lines);%���������ϵ�˳��洢���ߵ��кŽ�������Ϸ���ȫ��Ϊ-2
        Col_Right = -2*ones(1,search_Lines);%���������ϵ�˳��洢���ߵ��кŽ�������Ϸ���ȫ��Ϊ-2
        Col_Center = -2*ones(1,search_Lines);%���������ϵ�˳��洢�������ߵ��кŽ�������Ϸ���ȫ��Ϊ-2
        for i=1:search_Lines%�����һ�п�ʼ����ɨ�裬һ��ɨ��search_Lines��
            if (image(start_Row,start_Col(1)) == 255 || image(start_Row,start_Col(2)) == 255 ) || start_Col(1)>start_Col(2)       
                start_Row = start_Row - 1;
                continue;
            end%�ڵ�ǰ�����������߻�������һ����255����δ֪���򣩣�˵����û�н��뵽�������ӽ�����0�����1���򣩣�������������ܵ����ߵ��ұ�ȥ�ˣ���˵�����ǵ�·��
            if image(start_Row,start_Col(1)) == 1 %������߷���1���򣨵�·��           
                while image(start_Row,start_Col(1)) == 1 && start_Col(1)>=1+1
                    start_Col(1) = start_Col(1) - 1;
                end
                start_Col(1) = start_Col(1) + 1;%�����߳�������ɨ��ֱ��������1���򣨵�·�����п�����0��255����
                if image(start_Row,start_Col(1)-1) == 0 %�鿴��ʱ�Ƿ���0���򣨱�����                   
                    Col_Left(i) = start_Col(1); %ֻ����0����Ĳſ��Խ��кŴ洢��������
                end
            else%������߷���0���򣨱�����   
                while image(start_Row,start_Col(1)) == 0 && start_Col(1)<=size(image,2)-1
                    start_Col(1) = start_Col(1) + 1;
                end%�����߳�������ɨ��ֱ��������0���򣨱��������п�����1��255����
                if image(start_Row,start_Col(1)) == 1 %�鿴��ʱ�Ƿ���1���򣨵�·��      
                    Col_Left(i) = start_Col(1); %ֻ����1����Ĳſ��Խ��кŴ洢��������
                end
            end

            if image(start_Row,start_Col(2)) == 0 %������߷���0���򣨱�����                  
                while image(start_Row,start_Col(2)) == 0 && start_Col(2)>=1+1
                    start_Col(2) = start_Col(2) - 1;
                end      %�����߳�������ɨ��ֱ��������0���򣨱��������п�����1��255����
                if image(start_Row,start_Col(2)) == 1
                    Col_Right(i) = start_Col(2); %ֻ����1����Ĳſ��Խ��кŴ洢��������
                end
            else%������߷���1���򣨵�·��     
                while image(start_Row,start_Col(2)) == 1 && start_Col(2)<=size(image,2)-1
                    start_Col(2) = start_Col(2) + 1;
                end
                start_Col(2) = start_Col(2) - 1; %�����߳�������ɨ��ֱ��������1���򣨵�·�����п�����0��255����
                if image(start_Row,start_Col(2)+1) == 0                   
                    Col_Right(i) = start_Col(2); %ֻ����0����Ĳſ��Խ��кŴ洢��������
                end
            end
            start_Row = start_Row - 1; %������ɨ����ϣ�����н�����һ�У�����һ��������ɨ����׼��
            
            %�����������߼���   
            if start_Col(1)>start_Col(2)
                continue;
            end
            %�����߼�����4����������ߺϷ�(~=-2)��Ƿ�(==-2)�������ߺϷ�(~=-2)��Ƿ�(==-2)��
            if Col_Right(i)~=-2 && Col_Left(i)~= -2 %���ߺϷ������ߺϷ�
                if Col_Center(i-1)~=-2 %�����һ��������Ҳ�Ϸ�
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*0.5*(Col_Right(i) + Col_Left(i)); %������һ�ε������ߡ���һ����������ֵ�����˲�������ε�������
                else %�����һ�������߲��Ϸ�
                    Col_Center(i) = filter*ceil(size(image,2)/2)+(1-filter)*0.5*(Col_Right(i) + Col_Left(i)); %�ٶ���һ�������������м䣬������һ�������ߡ���һ����������ֵ�����˲�������ε�������
                end
            elseif Col_Right(i)~=-2 && Col_Left(i)==-2 %���߷Ƿ������ߺϷ�
                if Col_Center(i-1)~=-2 && Col_Right(i-1)~=-2 %�����һ�������ߺϷ�����һ�����ߺϷ�
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*(Col_Right(i)-Col_Right(i-1)+Col_Center(i-1)); %������һ�ε������ߡ��ٶ�����һ�������ߣ�������һ�κ����ߵľ��룩�����˲�������ε�������
                elseif Col_Center(i-1)==-2 && Col_Right(i-1)~=-2 %�����һ�������߷Ƿ�����һ�����ߺϷ�
                    Col_Center(i) = filter*ceil(size(image,2)/2)+(1-filter)*(Col_Right(i)-Col_Right(i-1)+ceil(size(image,2)/2));%�ٶ���һ�������������м䣬������һ�ε������ߡ��ٶ�����һ�������ߣ�������һ�κ����ߵľ��룩�����˲�������ε�������
                elseif Col_Center(i-1)~=-2 && Col_Right(i-1)==-2 %�����һ�������ߺϷ�����һ�����߷Ƿ�
                    Col_Center(i) = Col_Center(i-1); %��һ�������߼���ʹ����һ�ε�������
                else %�����һ�������߷Ƿ�����һ�����߷Ƿ�
                    Col_Center(i) = ceil(size(image,2)/2); %��һ��������ֱ�Ӽٶ�Ϊ���м�
                end
            elseif Col_Right(i)==-2 && Col_Left(i)~=-2 %���ߺϷ������߷Ƿ�������������
                if Col_Center(i-1)~=-2 && Col_Left(i-1)~=-2 
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*(Col_Left(i)-Col_Left(i-1)+Col_Center(i-1));
                elseif Col_Center(i-1)==-2 && Col_Left(i-1)~=-2 
                    Col_Center(i) = filter*ceil(size(image,2)/2)+(1-filter)*(Col_Left(i)-Col_Left(i-1)+ceil(size(image,2)/2));
                elseif Col_Center(i-1)~=-2 && Col_Left(i-1)==-2 
                    Col_Center(i) = Col_Center(i-1);
                else
                    Col_Center(i) = ceil(size(image,2)/2);
                end
            else %���߷Ƿ������߷Ƿ�
                if Col_Center(i-1)~=-2 %�����һ�������ߺϷ�
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*ceil(size(image,2)/2); %������һ�ε������ߡ��ٶ�����һ�������ߣ�ȡ���м䣩�����˲�������ε�������
                else %�����һ�������߷Ƿ�
                    Col_Center(i) = ceil(size(image,2)/2); %��һ��������ֱ�Ӽٶ�Ϊ���м�
                end
            end
        end
        %�����߼������
        
        %�����ǰ����ߡ������ߡ����߻���image_withLineͼ��
        %������ȡ2���԰�����������ȡ3��������
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
    % ����ֱ�������Բ��ã��������˲��ǽϴ����������ڱ����������й���Ĳ���
    elseif strcmp(class_Name,'ֱ��')
        filter = 0.7;%�˲�ϵ��������ʱ�ǵ�ͨ�˲�������ʱ�൱�ڸ�ͨ�˲�
        search_Lines = size(image,1);%һ��Ҫɨ������У������ͼƬ��
        
        start_Row = size(image,1);%��ǵ�ǰ�ڴ�����һ�У������һ�п�ʼ
        start_Col = [ceil(size(image,2)/2),ceil(size(image,2)/2)];%��ǵ�ǰ�ڴ�����һ�У�start_Col(1)ָ���ߣ�start_Col(2)ָ���ߣ�Ĭ�ϴ����Ŀ�ʼ
        Col_Left = -2*ones(1,search_Lines);%���������ϵ�˳��洢���ߵ��кŽ�������Ϸ���ȫ��Ϊ-2
        Col_Right = -2*ones(1,search_Lines);%���������ϵ�˳��洢���ߵ��кŽ�������Ϸ���ȫ��Ϊ-2
        Col_Center = -2*ones(1,search_Lines);%���������ϵ�˳��洢�������ߵ��кŽ�������Ϸ���ȫ��Ϊ-2
        for i=1:search_Lines%�����һ�п�ʼ����ɨ�裬һ��ɨ��search_Lines��
            if (image(start_Row,start_Col(1)) == 255 || image(start_Row,start_Col(2)) == 255 ) || start_Col(1)>start_Col(2)               
                start_Row = start_Row - 1;
                continue;
            end%�ڵ�ǰ�����������߻�������һ����255����δ֪���򣩣�˵����û�н��뵽�������ӽ�����0�����1����
            if image(start_Row,start_Col(1)) == 1 %������߷���1���򣨵�·��           
                while image(start_Row,start_Col(1)) == 1 && start_Col(1)>=1+1
                    start_Col(1) = start_Col(1) - 1;
                end
                start_Col(1) = start_Col(1) + 1;%�����߳�������ɨ��ֱ��������1���򣨵�·�����п�����0��255����
                if image(start_Row,start_Col(1)-1) == 0 %�鿴��ʱ�Ƿ���0���򣨱�����                   
                    Col_Left(i) = start_Col(1); %ֻ����0����Ĳſ��Խ��кŴ洢��������
                end
            else%������߷���0���򣨱�����   
                while image(start_Row,start_Col(1)) == 0 && start_Col(1)<=size(image,2)-1
                    start_Col(1) = start_Col(1) + 1;
                end%�����߳�������ɨ��ֱ��������0���򣨱��������п�����1��255����
                if image(start_Row,start_Col(1)) == 1 %�鿴��ʱ�Ƿ���1���򣨵�·��      
                    Col_Left(i) = start_Col(1); %ֻ����1����Ĳſ��Խ��кŴ洢��������
                end
            end

            if image(start_Row,start_Col(2)) == 0 %������߷���0���򣨱�����                  
                while image(start_Row,start_Col(2)) == 0 && start_Col(2)>=1+1
                    start_Col(2) = start_Col(2) - 1;
                end      %�����߳�������ɨ��ֱ��������0���򣨱��������п�����1��255����
                if image(start_Row,start_Col(2)) == 1
                    Col_Right(i) = start_Col(2); %ֻ����1����Ĳſ��Խ��кŴ洢��������
                end
            else%������߷���1���򣨵�·��     
                while image(start_Row,start_Col(2)) == 1 && start_Col(2)<=size(image,2)-1
                    start_Col(2) = start_Col(2) + 1;
                end
                start_Col(2) = start_Col(2) - 1; %�����߳�������ɨ��ֱ��������1���򣨵�·�����п�����0��255����
                if image(start_Row,start_Col(2)+1) == 0                   
                    Col_Right(i) = start_Col(2); %ֻ����0����Ĳſ��Խ��кŴ洢��������
                end
            end
            start_Row = start_Row - 1; %������ɨ����ϣ�����н�����һ�У�����һ��������ɨ����׼��
            
            %�����������߼���   
            if start_Col(1)>start_Col(2)
                continue;
            end
            %�����߼�����4����������ߺϷ�(~=-2)��Ƿ�(==-2)�������ߺϷ�(~=-2)��Ƿ�(==-2)��
            if Col_Right(i)~=-2 && Col_Left(i)~= -2 %���ߺϷ������ߺϷ�
                if Col_Center(i-1)~=-2 %�����һ��������Ҳ�Ϸ�
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*0.5*(Col_Right(i) + Col_Left(i)); %������һ�ε������ߡ���һ����������ֵ�����˲�������ε�������
                else %�����һ�������߲��Ϸ�
                    Col_Center(i) = filter*ceil(size(image,2)/2)+(1-filter)*0.5*(Col_Right(i) + Col_Left(i)); %�ٶ���һ�������������м䣬������һ�������ߡ���һ����������ֵ�����˲�������ε�������
                end
            elseif Col_Right(i)~=-2 && Col_Left(i)==-2 %���߷Ƿ������ߺϷ�
                if Col_Center(i-1)~=-2 && Col_Right(i-1)~=-2 %�����һ�������ߺϷ�����һ�����ߺϷ�
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*(Col_Right(i)-Col_Right(i-1)+Col_Center(i-1)); %������һ�ε������ߡ��ٶ�����һ�������ߣ�������һ�κ����ߵľ��룩�����˲�������ε�������
                elseif Col_Center(i-1)==-2 && Col_Right(i-1)~=-2 %�����һ�������߷Ƿ�����һ�����ߺϷ�
                    Col_Center(i) = filter*ceil(size(image,2)/2)+(1-filter)*(Col_Right(i)-Col_Right(i-1)+ceil(size(image,2)/2));%�ٶ���һ�������������м䣬������һ�ε������ߡ��ٶ�����һ�������ߣ�������һ�κ����ߵľ��룩�����˲�������ε�������
                elseif Col_Center(i-1)~=-2 && Col_Right(i-1)==-2 %�����һ�������ߺϷ�����һ�����߷Ƿ�
                    Col_Center(i) = Col_Center(i-1); %��һ�������߼���ʹ����һ�ε�������
                else %�����һ�������߷Ƿ�����һ�����߷Ƿ�
                    Col_Center(i) = ceil(size(image,2)/2); %��һ��������ֱ�Ӽٶ�Ϊ���м�
                end
            elseif Col_Right(i)==-2 && Col_Left(i)~=-2 %���ߺϷ������߷Ƿ�������������
                if Col_Center(i-1)~=-2 && Col_Left(i-1)~=-2 
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*(Col_Left(i)-Col_Left(i-1)+Col_Center(i-1));
                elseif Col_Center(i-1)==-2 && Col_Left(i-1)~=-2 
                    Col_Center(i) = filter*ceil(size(image,2)/2)+(1-filter)*(Col_Left(i)-Col_Left(i-1)+ceil(size(image,2)/2));
                elseif Col_Center(i-1)~=-2 && Col_Left(i-1)==-2 
                    Col_Center(i) = Col_Center(i-1);
                else
                    Col_Center(i) = ceil(size(image,2)/2);
                end
            else %���߷Ƿ������߷Ƿ�
                if Col_Center(i-1)~=-2 %�����һ�������ߺϷ�
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*ceil(size(image,2)/2); %������һ�ε������ߡ��ٶ�����һ�������ߣ�ȡ���м䣩�����˲�������ε�������
                else %�����һ�������߷Ƿ�
                    Col_Center(i) = ceil(size(image,2)/2); %��һ��������ֱ�Ӽٶ�Ϊ���м�
                end
            end
        end
        %�����߼������
        
        %�����ǰ����ߡ������ߡ����߻���image_withLineͼ��
        %������ȡ2���԰�����������ȡ3��������
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
    % ����ʮ��·�ڣ����Բ��ã�������
    elseif strcmp(class_Name,'ʮ��·��')
        flag_left=5;
        flag_right=5;
        filter = 0.7;%�˲�ϵ��������ʱ�ǵ�ͨ�˲�������ʱ�൱�ڸ�ͨ�˲�
        search_Lines = size(image,1);%һ��Ҫɨ������У������ͼƬ��
        
        start_Row = size(image,1);%��ǵ�ǰ�ڴ�����һ�У������һ�п�ʼ
        start_Col = [ceil(size(image,2)/2),ceil(size(image,2)/2)];%��ǵ�ǰ�ڴ�����һ�У�start_Col(1)ָ���ߣ�start_Col(2)ָ���ߣ�Ĭ�ϴ����Ŀ�ʼ
        last_start_Col = [ceil(size(image,2)/2),ceil(size(image,2)/2)];
        last1_start_Col = [ceil(size(image,2)/2),ceil(size(image,2)/2)];
        last2_start_Col = [ceil(size(image,2)/2),ceil(size(image,2)/2)];
        last3_start_Col = [ceil(size(image,2)/2),ceil(size(image,2)/2)];
        Col_Left = -2*ones(1,search_Lines);%���������ϵ�˳��洢���ߵ��кŽ�������Ϸ���ȫ��Ϊ-2
        Col_Right = -2*ones(1,search_Lines);%���������ϵ�˳��洢���ߵ��кŽ�������Ϸ���ȫ��Ϊ-2
        Col_Center = -2*ones(1,search_Lines);%���������ϵ�˳��洢�������ߵ��кŽ�������Ϸ���ȫ��Ϊ-2
        for i=1:search_Lines%�����һ�п�ʼ����ɨ�裬һ��ɨ��search_Lines��
            if (image(start_Row,start_Col(1)) == 255 || image(start_Row,start_Col(2)) == 255 ) || start_Col(1)>start_Col(2)               
                start_Row = start_Row - 1;
                continue;
            end%�ڵ�ǰ�����������߻�������һ����255����δ֪���򣩣�˵����û�н��뵽�������ӽ�����0�����1����
            last3_start_Col(1) = last2_start_Col(1);
            last2_start_Col(1) = last1_start_Col(1);
            last1_start_Col(1) = last_start_Col(1);
            last_start_Col(1) = start_Col(1);
            if image(start_Row,start_Col(1)) == 1 %������߷���1���򣨵�·��   
                if flag_left>=1
                    while image(start_Row,start_Col(1)) == 1 && start_Col(1)>=1+1
                        start_Col(1) = start_Col(1) - 1;
                    end
                    start_Col(1) = start_Col(1) + 1;%�����߳�������ɨ��ֱ��������1���򣨵�·�����п�����0��255����
                    if image(start_Row,start_Col(1)-1) == 0 %�鿴��ʱ�Ƿ���0���򣨱�����                   
                        Col_Left(i) = start_Col(1); %ֻ����0����Ĳſ��Խ��кŴ洢��������
                    end
                    flag_left = flag_left - 1;
                else
                    start_Col(1) = last_start_Col(1) +  0.4*(last_start_Col(1)- last1_start_Col(1))+0.3*(last1_start_Col(1)- last2_start_Col(1))+0.3*(last2_start_Col(1)- last3_start_Col(1));
                    start_Col(1) = round(start_Col(1));
                    if start_Col(1)<1
                        start_Col(1)=1;
                    end
                    if start_Col(1)>size(image,2)
                        start_Col(1) = size(image,2);
                    end
                    if image(start_Row,start_Col(1)) ~= 255
                        Col_Left(i) = start_Col(1);
                    end
                end
            else%������߷���0���򣨱�����   
                flag_left = 5;
                while image(start_Row,start_Col(1)) == 0 && start_Col(1)<=size(image,2)-1
                    start_Col(1) = start_Col(1) + 1;
                end%�����߳�������ɨ��ֱ��������0���򣨱��������п�����1��255����
                if image(start_Row,start_Col(1)) == 1 %�鿴��ʱ�Ƿ���1���򣨵�·��      
                    Col_Left(i) = start_Col(1); %ֻ����1����Ĳſ��Խ��кŴ洢��������
                end
            end
            
            last3_start_Col(2) = last2_start_Col(2);
            last2_start_Col(2) = last1_start_Col(2);
            last1_start_Col(2) = last_start_Col(2);
            last_start_Col(2) = start_Col(2);
            if image(start_Row,start_Col(2)) == 0 %������߷���0���򣨱����� 
                flag_right = 5;
                while image(start_Row,start_Col(2)) == 0 && start_Col(2)>=1+1
                    start_Col(2) = start_Col(2) - 1;
                end      %�����߳�������ɨ��ֱ��������0���򣨱��������п�����1��255����
                if image(start_Row,start_Col(2)) == 1
                    Col_Right(i) = start_Col(2); %ֻ����1����Ĳſ��Խ��кŴ洢��������
                end
            else%������߷���1���򣨵�·��     
                if flag_right>=1
                    while image(start_Row,start_Col(2)) == 1 && start_Col(2)<=size(image,2)-1
                        start_Col(2) = start_Col(2) + 1;
                    end
                    start_Col(2) = start_Col(2) - 1; %�����߳�������ɨ��ֱ��������1���򣨵�·�����п�����0��255����
                    if image(start_Row,start_Col(2)+1) == 0                   
                        Col_Right(i) = start_Col(2); %ֻ����0����Ĳſ��Խ��кŴ洢��������
                    end
                    flag_right = flag_right - 1;
                else
                    start_Col(2) = last_start_Col(2) +  0.4*(last_start_Col(2)- last1_start_Col(2))+0.3*(last1_start_Col(2)- last2_start_Col(2))+0.3*(last2_start_Col(2)- last3_start_Col(2));
                    start_Col(2) = round(start_Col(2));
                    if start_Col(2)<1
                        start_Col(2)=1;
                    end
                    if start_Col(2)>size(image,2)
                        start_Col(2) = size(image,2);
                    end
                    if image(start_Row,start_Col(2)) ~= 255
                        Col_Right(i) = start_Col(2);
                    end
                end
            end
            start_Row = start_Row - 1; %������ɨ����ϣ�����н�����һ�У�����һ��������ɨ����׼��
            
            %�����������߼���   
            if start_Col(1)>start_Col(2)
                continue;
            end
            %�����߼�����4����������ߺϷ�(~=-2)��Ƿ�(==-2)�������ߺϷ�(~=-2)��Ƿ�(==-2)��
            if Col_Right(i)~=-2 && Col_Left(i)~= -2 %���ߺϷ������ߺϷ�
                if Col_Center(i-1)~=-2 %�����һ��������Ҳ�Ϸ�
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*0.5*(Col_Right(i) + Col_Left(i)); %������һ�ε������ߡ���һ����������ֵ�����˲�������ε�������
                else %�����һ�������߲��Ϸ�
                    Col_Center(i) = filter*ceil(size(image,2)/2)+(1-filter)*0.5*(Col_Right(i) + Col_Left(i)); %�ٶ���һ�������������м䣬������һ�������ߡ���һ����������ֵ�����˲�������ε�������
                end
            elseif Col_Right(i)~=-2 && Col_Left(i)==-2 %���߷Ƿ������ߺϷ�
                if Col_Center(i-1)~=-2 && Col_Right(i-1)~=-2 %�����һ�������ߺϷ�����һ�����ߺϷ�
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*(Col_Right(i)-Col_Right(i-1)+Col_Center(i-1)); %������һ�ε������ߡ��ٶ�����һ�������ߣ�������һ�κ����ߵľ��룩�����˲�������ε�������
                elseif Col_Center(i-1)==-2 && Col_Right(i-1)~=-2 %�����һ�������߷Ƿ�����һ�����ߺϷ�
                    Col_Center(i) = filter*ceil(size(image,2)/2)+(1-filter)*(Col_Right(i)-Col_Right(i-1)+ceil(size(image,2)/2));%�ٶ���һ�������������м䣬������һ�ε������ߡ��ٶ�����һ�������ߣ�������һ�κ����ߵľ��룩�����˲�������ε�������
                elseif Col_Center(i-1)~=-2 && Col_Right(i-1)==-2 %�����һ�������ߺϷ�����һ�����߷Ƿ�
                    Col_Center(i) = Col_Center(i-1); %��һ�������߼���ʹ����һ�ε�������
                else %�����һ�������߷Ƿ�����һ�����߷Ƿ�
                    Col_Center(i) = ceil(size(image,2)/2); %��һ��������ֱ�Ӽٶ�Ϊ���м�
                end
            elseif Col_Right(i)==-2 && Col_Left(i)~=-2 %���ߺϷ������߷Ƿ�������������
                if Col_Center(i-1)~=-2 && Col_Left(i-1)~=-2 
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*(Col_Left(i)-Col_Left(i-1)+Col_Center(i-1));
                elseif Col_Center(i-1)==-2 && Col_Left(i-1)~=-2 
                    Col_Center(i) = filter*ceil(size(image,2)/2)+(1-filter)*(Col_Left(i)-Col_Left(i-1)+ceil(size(image,2)/2));
                elseif Col_Center(i-1)~=-2 && Col_Left(i-1)==-2 
                    Col_Center(i) = Col_Center(i-1);
                else
                    Col_Center(i) = ceil(size(image,2)/2);
                end
            else %���߷Ƿ������߷Ƿ�
                if Col_Center(i-1)~=-2 %�����һ�������ߺϷ�
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*ceil(size(image,2)/2); %������һ�ε������ߡ��ٶ�����һ�������ߣ�ȡ���м䣩�����˲�������ε�������
                else %�����һ�������߷Ƿ�
                    Col_Center(i) = ceil(size(image,2)/2); %��һ��������ֱ�Ӽٶ�Ϊ���м�
                end
            end
        end
        %�����߼������
        
        %�����ǰ����ߡ������ߡ����߻���image_withLineͼ��
        %������ȡ2���԰�����������ȡ3��������
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
    % ��������·�ڿ��Բ��ã��������˲��ǽϴ���������ʼ�տ�����ʻ
    elseif strcmp(class_Name,'����·��')
        flag=1;
        road_width = 0;
        filter = -0.3;%�˲�ϵ��������ʱ�ǵ�ͨ�˲�������ʱ�൱�ڸ�ͨ�˲�
        search_Lines = size(image,1);%һ��Ҫɨ������У������ͼƬ��
        
        start_Row = size(image,1);%��ǵ�ǰ�ڴ�����һ�У������һ�п�ʼ
        start_Col = [ceil(size(image,2)/2),ceil(size(image,2)/2)];%��ǵ�ǰ�ڴ�����һ�У�start_Col(1)ָ���ߣ�start_Col(2)ָ���ߣ�Ĭ�ϴ����Ŀ�ʼ
        Col_Left = -2*ones(1,search_Lines);%���������ϵ�˳��洢���ߵ��кŽ�������Ϸ���ȫ��Ϊ-2
        Col_Right = -2*ones(1,search_Lines);%���������ϵ�˳��洢���ߵ��кŽ�������Ϸ���ȫ��Ϊ-2
        Col_Center = -2*ones(1,search_Lines);%���������ϵ�˳��洢�������ߵ��кŽ�������Ϸ���ȫ��Ϊ-2
        for i=1:search_Lines%�����һ�п�ʼ����ɨ�裬һ��ɨ��search_Lines��
            if (image(start_Row,start_Col(1)) == 255 || image(start_Row,start_Col(2)) == 255 ) || start_Col(1)>start_Col(2)               
                start_Row = start_Row - 1;
                continue;
            end%�ڵ�ǰ�����������߻�������һ����255����δ֪���򣩣�˵����û�н��뵽�������ӽ�����0�����1����
            if image(start_Row,start_Col(2)) == 0 %������߷���0���򣨱�����                  
                while image(start_Row,start_Col(2)) == 0 && start_Col(2)>=1+1
                    start_Col(2) = start_Col(2) - 1;
                end      %�����߳�������ɨ��ֱ��������0���򣨱��������п�����1��255����
                if image(start_Row,start_Col(2)) == 1
                    Col_Right(i) = start_Col(2); %ֻ����1����Ĳſ��Խ��кŴ洢��������
                end
            else%������߷���1���򣨵�·��     
                while image(start_Row,start_Col(2)) == 1 && start_Col(2)<=size(image,2)-1
                    start_Col(2) = start_Col(2) + 1;
                end
                start_Col(2) = start_Col(2) - 1; %�����߳�������ɨ��ֱ��������1���򣨵�·�����п�����0��255����
                if image(start_Row,start_Col(2)+1) == 0                   
                    Col_Right(i) = start_Col(2); %ֻ����0����Ĳſ��Խ��кŴ洢��������
                end
            end   
            start_Col(1) = start_Col(2)-road_width;%���ߵĳ�ʼɨ�������������road_width
            if image(start_Row,start_Col(1)) == 1 %������߷���1���򣨵�·��  
                if flag==1%��һ�γ�ʼ��road_width
                    while image(start_Row,start_Col(1)) == 1 && start_Col(1)>=1+1
                        start_Col(1) = start_Col(1) - 1;
                    end
                    start_Col(1) = start_Col(1) + 1;%�����߳�������ɨ��ֱ��������1���򣨵�·�����п�����0��255����
                    if image(start_Row,start_Col(1)-1) == 0 %�鿴��ʱ�Ƿ���0���򣨱�����                   
                        Col_Left(i) = start_Col(1); %ֻ����0����Ĳſ��Խ��кŴ洢��������
                    end
                
                    road_width = start_Col(2)-start_Col(1);
                    flag=0;
                end       
                Col_Left(i) = start_Col(1);%��һ��֮��ʣ��ʱ����1����ͱ�����������road_width
            else%������߷���0���򣨱�����   
                while image(start_Row,start_Col(1)) == 0 && start_Col(1)<=size(image,2)-1
                    start_Col(1) = start_Col(1) + 1;
                end%�����߳�������ɨ��ֱ��������0���򣨱��������п�����1��255����
                if image(start_Row,start_Col(1)) == 1 %�鿴��ʱ�Ƿ���1���򣨵�·��      
                    Col_Left(i) = start_Col(1); %ֻ����1����Ĳſ��Խ��кŴ洢��������
                end
            end


            start_Row = start_Row - 1; %������ɨ����ϣ�����н�����һ�У�����һ��������ɨ����׼��
            
            %�����������߼���   
            if start_Col(1)>start_Col(2)
                continue;
            end
            %�����߼�����4����������ߺϷ�(~=-2)��Ƿ�(==-2)�������ߺϷ�(~=-2)��Ƿ�(==-2)��
            if Col_Right(i)~=-2 && Col_Left(i)~= -2 %���ߺϷ������ߺϷ�
                if Col_Center(i-1)~=-2 %�����һ��������Ҳ�Ϸ�
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*0.5*(Col_Right(i) + Col_Left(i)); %������һ�ε������ߡ���һ����������ֵ�����˲�������ε�������
                else %�����һ�������߲��Ϸ�
                    Col_Center(i) = filter*ceil(size(image,2)/2)+(1-filter)*0.5*(Col_Right(i) + Col_Left(i)); %�ٶ���һ�������������м䣬������һ�������ߡ���һ����������ֵ�����˲�������ε�������
                end
            elseif Col_Right(i)~=-2 && Col_Left(i)==-2 %���߷Ƿ������ߺϷ�
                if Col_Center(i-1)~=-2 && Col_Right(i-1)~=-2 %�����һ�������ߺϷ�����һ�����ߺϷ�
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*(Col_Right(i)-Col_Right(i-1)+Col_Center(i-1)); %������һ�ε������ߡ��ٶ�����һ�������ߣ�������һ�κ����ߵľ��룩�����˲�������ε�������
                elseif Col_Center(i-1)==-2 && Col_Right(i-1)~=-2 %�����һ�������߷Ƿ�����һ�����ߺϷ�
                    Col_Center(i) = filter*ceil(size(image,2)/2)+(1-filter)*(Col_Right(i)-Col_Right(i-1)+ceil(size(image,2)/2));%�ٶ���һ�������������м䣬������һ�ε������ߡ��ٶ�����һ�������ߣ�������һ�κ����ߵľ��룩�����˲�������ε�������
                elseif Col_Center(i-1)~=-2 && Col_Right(i-1)==-2 %�����һ�������ߺϷ�����һ�����߷Ƿ�
                    Col_Center(i) = Col_Center(i-1); %��һ�������߼���ʹ����һ�ε�������
                else %�����һ�������߷Ƿ�����һ�����߷Ƿ�
                    Col_Center(i) = ceil(size(image,2)/2); %��һ��������ֱ�Ӽٶ�Ϊ���м�
                end
            elseif Col_Right(i)==-2 && Col_Left(i)~=-2 %���ߺϷ������߷Ƿ�������������
                if Col_Center(i-1)~=-2 && Col_Left(i-1)~=-2 
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*(Col_Left(i)-Col_Left(i-1)+Col_Center(i-1));
                elseif Col_Center(i-1)==-2 && Col_Left(i-1)~=-2 
                    Col_Center(i) = filter*ceil(size(image,2)/2)+(1-filter)*(Col_Left(i)-Col_Left(i-1)+ceil(size(image,2)/2));
                elseif Col_Center(i-1)~=-2 && Col_Left(i-1)==-2 
                    Col_Center(i) = Col_Center(i-1);
                else
                    Col_Center(i) = ceil(size(image,2)/2);
                end
            else %���߷Ƿ������߷Ƿ�
                if Col_Center(i-1)~=-2 %�����һ�������ߺϷ�
                    Col_Center(i) = filter*Col_Center(i-1)+(1-filter)*ceil(size(image,2)/2); %������һ�ε������ߡ��ٶ�����һ�������ߣ�ȡ���м䣩�����˲�������ε�������
                else %�����һ�������߷Ƿ�
                    Col_Center(i) = ceil(size(image,2)/2); %��һ��������ֱ�Ӽٶ�Ϊ���м�
                end
            end
        end
        %�����߼������
        
        %�����ǰ����ߡ������ߡ����߻���image_withLineͼ��
        %������ȡ2���԰�����������ȡ3��������
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

