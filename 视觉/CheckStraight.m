function [is_Straight] = CheckStraight(image)
    search_Lines = 25;
    
    start_Row = size(image,1);
    start_Col = [ceil(size(image,2)/2),ceil(size(image,2)/2)];
    center_Col = -2*ones(1,search_Lines);
    
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
        else
            while image(start_Row,start_Col(1)) ~= 1 && start_Col(1)<=size(image,2)-1
                start_Col(1) = start_Col(1) + 1;
            end
        end
        
        if image(start_Row,start_Col(2)) ~= 1            
            while image(start_Row,start_Col(2)) ~= 1 && start_Col(2)>=1+1
                start_Col(2) = start_Col(2) - 1;
            end          
        else
            while image(start_Row,start_Col(2)) == 1 && start_Col(2)<=size(image,2)-1
                start_Col(2) = start_Col(2) + 1;
            end
            start_Col(2) = start_Col(2) - 1;
        end
        if start_Col(1) <= start_Col(2)
            center_Col(i) = floor(0.5*(start_Col(1)+start_Col(2)));
        else
            center_Col(i) = -1;
            is_Straight = 0;
            return;
        end
        start_Row = start_Row - 1;
    end
    
    max_Col = 1;
    min_Col = size(image,2);
    for i=1:search_Lines
        if center_Col(i)~=-2
            if center_Col(i)>max_Col
                max_Col = center_Col(i);
            end
            if center_Col(i)<min_Col
                min_Col = center_Col(i);
            end
        end
    end
    if max_Col - min_Col <=5
        is_Straight = 1;
    else
        is_Straight = 0;
    end
end

