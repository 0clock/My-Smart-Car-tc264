clc,clear,close all;
A = imread('1688.64.pgm');
B = ImageProcessing(A, 40, 27.89191, 5.915322, 0.1, 2);



tic
value = Kmeans(A)
toc



subplot(2 ,2 ,1),imshow(A,[0,256]);
title('ԭʼͼ')
subplot(2 ,2 ,2),imshow(B,[0,256]);
title('��͸�ӱ任ͼ')
subplot(2 ,2 ,3),imshow(A>value,[0,1]);
title('ԭʼͼ��ֵ��')
subplot(2 ,2 ,4),imshow(B>value,[0,1]);
title('��͸�ӱ任ͼ��ֵ��')