clc,clear,close all;

%��ͼƬ��Ȼ���ֵ+��͸��
A = imread('185.34ʮ��·��.pgm');
value = Kmeans(A);
B = ImageProcessing(A(:,1:end-1)>value, 40, 27.89191, 5.915322, 0.1, 2);
%ʶ������
if CheckStraightV2(B) == 1
    class_Name = 'ֱ��';
else
    class_Name = Classification(B);
end
%����ʶ�����ͽ��������߻���
[C,~,~] = DrawCenterLine(class_Name,B);

subplot(1 ,3 ,1),imshow(A,[0,256]);
title('ԭʼͼ')
subplot(1 ,3 ,2),imshow(B,[0,1]);
title(class_Name)
subplot(1 ,3 ,3),imshow(C,[0,4]);
title(class_Name)