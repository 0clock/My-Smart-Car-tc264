clear

filename=dir('D:\CarData\2022-03-06 06-36-07 Images\*.pgm');%存储所有文件名
for i=1:length(filename)
    fname=strcat('D:\CarData\2022-03-06 06-36-07 Images\',filename(i).name);%存储路径
    file=[filename(i).name];
    file(end-3:end)=[];%去掉字符串后四位
    
    A=imread(fname);%读入
    
    value = Kmeans(A);
    B = ImageProcessing(A(:,1:end-1)>value, 40, 27.89191, 5.915322, 0.1, 2);
       
    
    if CheckStraightV2(B) == 1
        class_Name = '直道';
    else
        class_Name = Classification(B);
    end
    %根据识别类型进行中心线绘制
    C = DrawCenterLine(class_Name,B);
    
    subplot(3 ,2 ,1),imshow(A,[0,256]);
    title('原始图')
    subplot(3 ,2 ,2),imshow(ImageProcessing(A, 40, 27.89191, 5.915322, 0.1, 2),[0,256]);
    title('逆透视变换图')
    subplot(3 ,2 ,3),imshow(A>value,[0,1]);
    title('原始图二值化')
    subplot(3 ,2 ,4),imshow(B,[0,1]);
    title(class_Name)
    subplot(3 ,2 ,5),imshow(C,[0,4]);
    title(class_Name)
    
   
   
    saveas(gcf,strcat(file,'.jpg'));
end
