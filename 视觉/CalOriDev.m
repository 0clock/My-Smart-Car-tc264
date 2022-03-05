function [dev_value] = CalOriDev(cal_range,Col_Center,search_Lines)
% 计算当前绘制的中线与行驶正方向的偏差
    Center_data = Col_Center;
    sum=0;
    length(Center_data(:));
    for i=1:search_Lines   %从最后一行开始逐行扫描，一共扫描search_Lines行
        if Center_data(i) ~= -2
            data_Ini=Center_data(i);
            break;
        end
    end
    
    for i=1:floor(search_Lines*cal_range)    %从最后一行开始逐行扫描，一共扫描search_Lines*cal_range行
        if Center_data(i) ~= -2
            sum = sum + Center_data(i)-data_Ini;
        end
    end
    
    dev_value = sum;
end