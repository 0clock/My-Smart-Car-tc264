clc,clear,close all;
A = imread('1064.7.pgm');
B = ImageProcessing(A, 30, 20, 6, 0.005, 2);

subplot(1 ,2 ,1),imshow(B,[0,256]);
title('原始图')
subplot(1 ,2 ,2),imshow(C,[0,256]);
title('逆透视变换图')

