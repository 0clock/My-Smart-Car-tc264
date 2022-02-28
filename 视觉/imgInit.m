clear

filename=dir('/Users/yuhongfei/Downloads/Images/class2/左弯/*.pgm');%存储所有文件名
for i=1:length(filename)
    fname=strcat('/Users/yuhongfei/Downloads/Images/class2/左弯/',filename(i).name);%存储路径
    file=[filename(i).name];
    file(end-3:end)=[];%去掉字符串后四位
    
    A=imread(fname);%读入
    
    value = Kmeans(A)
    B = ImageProcessing(A>value, 40, 27.89191, 5.915322, 0.1, 2);
    

    name(i,:) = str2num(file)
    
    %求出图片的16个特征值
    arg(i,:) = Get16(B)
%9个特征值写入excel表格（还没写）
%然后循环上述部分，直到某个赛道类别的图片集全部结束

%PS：一个赛道类别弄一个特征值excel表格

    %绘图
    
    %subplot(2 ,2 ,1),imshow(A,[0,256]);
    %title('原始图')
    %subplot(2 ,2 ,2),imshow(B,[0,256]);
    %title('逆透视变换图')
    %subplot(2 ,2 ,3),imshow(A>value,[0,1]);
    %title('原始图二值化')
    %subplot(2 ,2 ,4),imshow(B>value,[0,1]);
    %title('逆透视变换图二值化')
   
   
    %saveas(gcf,file);
end

C = num2cell([name,arg])

writecell(C,'左弯.xls')