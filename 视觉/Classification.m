function [class_Name] = Classification(image)
%[class_Name] = Classification(image)
%image：二值化+逆透视后的图片
%class_Name：赛道类别字符串
    BayesTable = [-3.420 -2.854 -4.667 -5.363 -2.945 -4.882 1.235 -9.615 -8.909 -2.680 -7.525 -8.391
                15.410 10.233 9.674 9.086 15.070 8.994 12.950 24.741 22.143 13.690 18.635 18.403
                10.233 15.410 24.741 22.143 13.690 18.635 12.950 9.674 9.086 15.070 8.994 18.403
                -2.854 -3.420 -9.615 -8.909 -2.680 -7.525 1.235 -4.667 -5.363 -2.945 -4.882 -8.391
                0.761 2.867 -0.149 -3.448 -2.142 2.116 1.934 0.369 -10.633 -5.238 -4.894 0.881
                8.207 1.513 7.723 6.257 7.265 8.238 11.747 13.404 17.776 5.030 16.593 16.923
                1.513 8.207 13.404 17.776 5.030 16.593 11.747 7.723 6.257 7.265 8.238 16.923
                2.867 0.761 0.369 -10.633 -5.238 -4.894 1.934 -0.149 -3.448 -2.142 2.116 0.881
                14.138 -15.595 -0.647 -1.221 4.191 -2.929 12.334 10.959 34.429 21.828 22.361 16.250
                -15.595 14.138 10.959 34.429 21.828 22.361 12.334 -0.647 -1.221 4.191 -2.929 16.250
                169.243 179.710 166.997 176.743 203.214 169.881 156.108 176.459 157.122 179.855 166.889 163.575
                179.710 169.243 176.459 157.122 179.855 166.889 156.108 166.997 176.743 203.214 169.881 163.575
                -152.679 -152.679 -158.761 -165.717 -197.104 -158.416 -147.119 -158.761 -165.717 -197.104 -158.416 -166.916];

    arg = Get16(image);
    classification_Data = zeros(1,12);
    for i = 1:12
        classification_Data(i) = arg(1)*BayesTable(1,i)+arg(2)*BayesTable(2,i)+arg(3)*BayesTable(3,i)+arg(4)*BayesTable(4,i)+arg(5)*BayesTable(5,i)+arg(6)*BayesTable(6,i)+arg(7)*BayesTable(7,i)+arg(8)*BayesTable(8,i)+arg(10)*BayesTable(9,i)+arg(11)*BayesTable(10,i)+arg(14)*BayesTable(11,i)+arg(15)*BayesTable(12,i)+BayesTable(13,i);
        if i==1
            classification_Data_max = classification_Data(1);
            classification_Result = 1;
        elseif classification_Data(i)>classification_Data_max
            classification_Data_max = classification_Data(i);
            classification_Result = i;         
        end
    end
    class_Name_Group = {'左弯','右弯','发现右环岛', '右环岛中心','入右环岛','出右环岛','三岔路口','发现左环岛', '左环岛中心','入左环岛','出左环岛','十字路口'};
    class_Name = class_Name_Group(classification_Result);
end

