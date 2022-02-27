clc,clear,close all;
A = imread('test7.png');
B = rgb2gray(A);

% test4参数
% C = ImageProcessing(B, 43.6, 27, 6, 0.005);

%C = ImageProcessing(B, 29.4, 30, 6, 0.005);

%C = ImageProcessing(B, 28, 28, 6, 0.005);

C = ImageProcessing(B, 30, 20, 6, 0.005);

subplot(1 ,2 ,1),imshow(B,[0,256]);
title('原始图')
subplot(1 ,2 ,2),imshow(C,[0,256]);
title('逆透视变换图')

