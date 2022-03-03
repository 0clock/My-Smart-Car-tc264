function [value] = Kmeans(image)
    image = double(image);
    KMEANS_K = 3;
    m = [85,0,0;171,0,0;255,0,0];%第一列存最终分类结果，第二列存每种分类的样本数，第三列存中间变量
    maxtimes = 2;
    for time = 1:maxtimes
        for i = 1:KMEANS_K
            m(i,2) = 0;
        end
        for i = 1:size(image,1)
            for j = 1:size(image,2)
                min_distance = abs(image(i,j)-m(1,1));
                min_distance_class = 1;
                for k=2:KMEANS_K
                    if abs(image(i,j)-m(k,1)) < min_distance
                        min_distance = abs(image(i,j)-m(k,1));
                        min_distance_class = k;
                    end
                end
                m(min_distance_class,2) = m(min_distance_class,2)+1;
                m(min_distance_class,3) = (m(min_distance_class,2)-1)/m(min_distance_class,2)*m(min_distance_class,3) + image(i,j)/m(min_distance_class,2);
            end
        end
        for i = 1:KMEANS_K
            m(i,1) = m(i,3);
        end
    end
    max_cnt = [m(1,2),m(2,2)];
    max_cnt_class = [1,2];
    if m(1,2)>=m(2,2)
        for i = 3:KMEANS_K
            if m(i,2)>max_cnt(1) && m(i,1)<205
                max_cnt(1) = m(i,2);
                max_cnt_class(1) = i;
            elseif m(i,2)>max_cnt(2) && m(i,1)<205
                max_cnt(2) = m(i,2);
                max_cnt_class(2) = i;
            end
        end
    else
        for i = 3:KMEANS_K
            if m(i,2)>max_cnt(2)  && m(i,1)<205
                max_cnt(2) = m(i,2);
                max_cnt_class(2) = i;
            elseif m(i,2)>max_cnt(1)  && m(i,1)<205
                max_cnt(1) = m(i,2);
                max_cnt_class(1) = i;
            end
        end
    end
    value = uint8(0.5*(m(max_cnt_class(1),1)+m(max_cnt_class(2),1)));
end

