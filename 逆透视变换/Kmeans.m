function [value] = Kmeans(image)
    image = double(image);
    m1 = 85;
    m2 = 171;
    new_m1 = 0;
    new_m2 = 0;
    maxtimes = 2;
    for time = 1:maxtimes
        cnt_m1 = 0;
        cnt_m2 = 0;
        for i = 1:size(image,1)
            for j = 1:size(image,2)            
                if image(i,j)>0.5.*(m1+m2)
                    cnt_m2 = cnt_m2 + 1;
                    new_m2 = (cnt_m2-1)./cnt_m2.*new_m2 + image(i,j)./cnt_m2;
                else
                    cnt_m1 = cnt_m1 + 1;
                    new_m1 = (cnt_m1-1)./cnt_m1.*new_m1 + image(i,j)./cnt_m1;
                end
            end
        end
        m1 = new_m1;
        m2 = new_m2;
    end
    value = uint8(0.5.*(m1+m2));
end

