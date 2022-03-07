clc,clear,close all;

%读图片，然后二值+逆透视
A = imread('185.34十字路口.pgm');
value = Kmeans(A);
B = ImageProcessing(A(:,1:end-1)>value, 40, 27.89191, 5.915322, 0.1, 2);
%识别类型
if CheckStraightV2(B) == 1
    class_Name = '直道';
else
    class_Name = Classification(B);
end
%根据识别类型进行中心线绘制
[C,~,~] = DrawCenterLine(class_Name,B);

subplot(1 ,3 ,1),imshow(A,[0,256]);
title('原始图')
subplot(1 ,3 ,2),imshow(B,[0,1]);
title(class_Name)
subplot(1 ,3 ,3),imshow(C,[0,4]);
title(class_Name)