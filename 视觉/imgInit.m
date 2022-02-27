

filename=dir('/Users/yuhongfei/Downloads/Images/*.pgm');%存储所有文件名
for i=1:length(filename)
    fname=strcat('/Users/yuhongfei/Downloads/Images/',filename(i).name);%存储路径
    A=imread(fname);%读入
    
    B=ImageProcessing(A, 40, 27.89191, 5.915322, 0.1, 2);
    
    value = Kmeans(A)
    
    subplot(2 ,2 ,1),imshow(A,[0,256]);
title('原始图')
subplot(2 ,2 ,2),imshow(B,[0,256]);
title('逆透视变换图')
subplot(2 ,2 ,3),imshow(A>value,[0,1]);
title('原始图二值化')
subplot(2 ,2 ,4),imshow(B>value,[0,1]);
title('逆透视变换图二值化')
    
    file=[filename(i).name '.jpg'];
   
   
    saveas(gcf,file);
end