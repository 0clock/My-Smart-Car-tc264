clc,clear,close all;

%��ͼƬ��Ȼ���ֵ+��͸��
A = imread('151.pgm');
value = Kmeans(A);
B = ImageProcessing(A>value, 40, 27.89191, 5.915322, 0.1, 2);
%ʶ������
class_Name = Classification(B);


subplot(1 ,2 ,1),imshow(A,[0,256]);
title('ԭʼͼ')
subplot(1 ,2 ,2),imshow(B,[0,1]);
title(class_Name)