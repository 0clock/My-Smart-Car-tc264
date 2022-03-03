clc,clear,close all;

%读图片，然后二值+逆透视
A = imread('1684.04.pgm');
value = Kmeans(A);
B = ImageProcessing(A>value, 40, 27.89191, 5.915322, 0.1, 2);
%识别类型
if CheckStraightV2(B) == 1
    class_Name = '直道';
else
    class_Name = Classification(B);
end

subplot(1 ,2 ,1),imshow(A,[0,256]);
title('原始图')
subplot(1 ,2 ,2),imshow(B,[0,1]);
title(class_Name)