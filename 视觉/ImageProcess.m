clc,clear,close all;
A = imread('test.pgm');
tic
value = Kmeans(A)
toc
B = ImageProcessing(A>value, 40, 27.89191, 5.915322, 0.1, 2);







subplot(1 ,2 ,1),imshow(A,[0,256]);
title('ԭʼͼ')
subplot(1 ,2 ,2),imshow(B,[0,1]);
title('��͸�ӱ任ͼ��ֵ��')