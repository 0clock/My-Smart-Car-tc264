clc,clear,close all;

%��ͼƬ��Ȼ���ֵ+��͸��
A = imread('1064.7.pgm');
value = Kmeans(A)
B = ImageProcessing(A>value, 40, 27.89191, 5.915322, 0.1, 2);
%���ͼƬ��9������ֵ
arg = Get9(B)
%9������ֵд��excel��񣨻�ûд��
%Ȼ��ѭ���������֣�ֱ��ĳ����������ͼƬ��ȫ������

%PS��һ���������Ūһ������ֵexcel���


subplot(1 ,2 ,1),imshow(A,[0,256]);
title('ԭʼͼ')
subplot(1 ,2 ,2),imshow(B,[0,1]);
title('��͸�ӱ任ͼ��ֵ��')