function [class_Name] = ClassificationV2(image)
    Classification_Table = [-1 -1 2 -1 2 -1
-1 -1 2 2 -1 2
-1 -1 -1 2 2 -1
2 -1 2 -1 2 2
-1 -1 2 2 -1 2
-1 2 -1 2 2 2
0 0 0 0 0 0
2 2 2 2 2 2
0 0 0 0 0 0
];
    arg = Get9(image);
    for j=1:9
%         if arg(j)<0.3
%             arg(j)=-1;
%         elseif arg(j)<0.7
%             arg(j)=5*arg(j)-2.5;
%         else
%             arg(j)=1;
%         end
        arg(j) = arg(j)*2-1;
    end
    classification_Data = zeros(1,6);
    for i = 1:6
        for j=1:9          
            classification_Data(i) = classification_Data(i) + arg(j)*Classification_Table(j,i);
        end
        if i==1
            classification_Data_max = classification_Data(1);
            classification_Result = 1;
        elseif classification_Data(i)>classification_Data_max
            classification_Data_max = classification_Data(i);
            classification_Result = i;         
        end
    end
    class_Name_Group = {'左弯','右弯','发现左路', '发现右路','发现岔路','十字路口'};
    class_Name = class_Name_Group(classification_Result);
end

