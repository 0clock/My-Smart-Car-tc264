clc,clear,close all;

%读图片，然后二值+逆透视
A = imread('200.pgm');
value = Kmeans(A)
B = ImageProcessing(A>value, 40, 27.89191, 5.915322, 0.1, 2);
%求出图片的6个特征值
arg = Get6(B)
%6个特征值写入excel表格（还没写）
%然后循环上述部分，直到某个赛道类别的图片集全部结束

%PS：一个赛道类别弄一个特征值excel表格


subplot(1 ,2 ,1),imshow(A,[0,256]);
title('原始图')
subplot(1 ,2 ,2),imshow(B,[0,1]);
title('逆透视变换图二值化')