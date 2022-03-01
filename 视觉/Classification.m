function [class_Name] = Classification(image)
%[class_Name] = Classification(image)
%image：二值化+逆透视后的图片
%class_Name：赛道类别字符串
    BayesTable = [-1.190 -3.034 -2.802 -6.263 -0.934 -4.668 1.357 -2.981 -5.393 -0.645 -3.967 -6.710
                14.146 15.653 17.148 17.552 17.588 13.615 14.721 18.589 17.731 14.509 16.188 19.415
                15.653 14.146 18.589 17.731 14.509 16.188 14.721 17.148 17.552 17.588 13.615 19.415
                -3.034 -1.190 -2.981 -5.393 -0.645 -3.967 1.357 -2.802 -6.263 -0.934 -4.668 -6.710
                5.118 4.216 -0.315 -0.623 2.451 4.065 4.638 1.208 -3.510 -0.867 -3.273 4.480
                6.607 6.552 15.825 12.612 8.794 9.528 12.419 15.823 15.220 6.437 17.269 18.047
                6.552 6.607 15.823 15.220 6.437 17.269 12.419 15.825 12.612 8.794 9.528 18.047
                4.216 5.118 1.208 -3.510 -0.867 -3.273 4.638 -0.315 -0.623 2.451 4.065 4.480
                19.306 10.445 23.653 24.690 22.250 18.517 25.460 23.195 34.186 33.739 27.965 27.107
                10.445 19.306 23.195 34.186 33.739 27.965 25.460 23.653 24.690 22.250 18.517 27.107
                118.385 115.730 115.069 120.424 138.194 110.731 109.583 114.231 117.511 121.156 111.660 115.457
                115.730 118.385 114.231 117.511 121.156 111.660 109.583 115.069 120.424 138.194 110.731 115.457
                -114.584 -114.584 -127.289 -139.810 -154.666 -118.157 -123.839 -127.289 -139.810 -154.666 -118.157 -139.937];

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

