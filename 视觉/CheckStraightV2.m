function [is_Straight] = CheckStraightV2(image)
    search_Lines = 20;
    
    start_Row = size(image,1);
    real_Start_Row = 1;
    
    start_Col = [ceil(size(image,2)/2),ceil(size(image,2)/2)];
    Col_Left = -2*ones(1,search_Lines);
    Col_Right = -2*ones(1,search_Lines);
    
    for i=1:search_Lines
        if image(start_Row,start_Col(1)) == 255 || image(start_Row,start_Col(2)) == 255          
            start_Row = start_Row - 1;
            continue;
        end
        if image(start_Row,start_Col(1)) == 1            
            while image(start_Row,start_Col(1)) == 1 && start_Col(1)>=1+1
                start_Col(1) = start_Col(1) - 1;
            end
            if image(start_Row,start_Col(1)) == 0
                start_Col(1) = start_Col(1) + 1;
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
            if image(start_Row,start_Col(2)) == 0
                start_Col(2) = start_Col(2) - 1;
                Col_Right(i) = start_Col(2);
            end
        end
        start_Row = start_Row - 1;
    end
    
    max_Col_Left = 1;
    min_Col_Left = size(image,2);
    detect_Left = 0;
    max_Col_Right = 1;
    min_Col_Right = size(image,2);
    detect_Right = 0;
    for i=1:search_Lines
        if Col_Left(i)~=-2
            detect_Left = 1;
            if Col_Left(i)>max_Col_Left
                max_Col_Left = Col_Left(i);
            end
            if Col_Left(i)<min_Col_Left
                min_Col_Left = Col_Left(i);
            end
        elseif detect_Left == 1
            is_Straight = 0;
            return;
        end
        
        if Col_Right(i)~=-2
            detect_Right = 1;
            if Col_Right(i)>max_Col_Right
                max_Col_Right = Col_Right(i);
            end
            if Col_Right(i)<max_Col_Right
                max_Col_Right = Col_Right(i);
            end
        elseif detect_Right == 1
            is_Straight = 0;
            return;
        end
    end
    if (max_Col_Left - min_Col_Left <=5 && max_Col_Left >= min_Col_Left) && (max_Col_Right - min_Col_Right <=5 && max_Col_Right >= min_Col_Right)  && max_Col_Left < min_Col_Right
        is_Straight = 1;
    else
        is_Straight = 0;
    end
end

