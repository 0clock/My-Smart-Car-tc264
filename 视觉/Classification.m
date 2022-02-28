function [class_Name] = Classification(image)
%[class_Name] = Classification(image)
%image：二值化+逆透视后的图片
%class_Name：赛道类别字符串
    BayesTable = [-21.024 -23.935 -22.277 -26.653 -24.604 -24.966 -22.080
                    17.306 20.274 18.979 20.795 22.689 16.765 19.506
                    2.265 2.281 10.617 6.770 -1.019 6.054 3.593
                    23.408 23.549 28.814 24.413 28.015 26.285 35.713
                    22.493 25.917 18.779 19.169 23.811 25.751 28.534
                    15.709 14.274 22.393 19.843 14.499 19.230 17.711
                    23.711 22.523 40.919 37.841 28.278 41.404 43.184
                    -10.613 -4.339 -12.353 -15.560 -15.330 -13.566 -7.511
                    19.252 11.638 33.799 36.333 28.810 25.505 35.836
                    5.043 16.106 23.240 31.473 24.390 28.288 26.469
                    104.680 95.429 89.987 92.188 112.734 85.795 77.350
                    170.450 172.813 169.789 177.016 187.627 170.067 182.731
                    -133.418 -131.981 -153.207 -162.923 -175.203 -146.300 -164.499];

    arg = Get16(image);
    classification_Data = zeros(1,7);
    for i = 1:7
        classification_Data(i) = arg(1)*BayesTable(1,i)+arg(2)*BayesTable(2,i)+arg(3)*BayesTable(3,i)+arg(4)*BayesTable(4,i)+arg(5)*BayesTable(5,i)+arg(6)*BayesTable(6,i)+arg(7)*BayesTable(7,i)+arg(8)*BayesTable(8,i)+arg(10)*BayesTable(9,i)+arg(11)*BayesTable(10,i)+arg(14)*BayesTable(11,i)+arg(15)*BayesTable(12,i)+BayesTable(13,i);
        if i==1
            classification_Data_max = classification_Data(1);
            classification_Result = 1;
        elseif classification_Data(i)>classification_Data_max
            classification_Data_max = classification_Data(i);
            classification_Result = i;         
        end
    end
    class_Name_Group = {'左弯','右弯','发现右环岛', '右环岛中心','入右环岛','出右环岛','三岔路口'};
    class_Name = class_Name_Group(classification_Result);
end

