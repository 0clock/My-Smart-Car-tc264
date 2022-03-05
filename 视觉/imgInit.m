clear

filename=dir('/Users/yuhongfei/Downloads/Images/class4/右弯/*.pgm');%存储所有文件名
for i=1:length(filename)
    fname=strcat('/Users/yuhongfei/Downloads/Images/class4/右弯/',filename(i).name);%存储路径
    file=[filename(i).name];
    file(end-3:end)=[];%去掉字符串后四位
    
    A=imread(fname);%读入
    
    value = Kmeans(A(:,1:end-1))
    B = ImageProcessing(A(:,1:end-1)>value, 40, 27.89191, 5.915322, 0.1, 2);
    

    name(i,:) = str2num(file)
    
    %求出图片的16个特征值
    %arg(i,:) = Get16(B)
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
    
    
    %识别类型
if CheckStraightV2(B) == 1
    class_Name = '直道';
else
    class_Name = Classification(B);
end
%根据识别类型进行中心线绘制
[C,Col_Center,search_Lines] = DrawCenterLine(class_Name,B);

Ori_Dev= CalOriDev(0.5,Col_Center,search_Lines)

subplot(1 ,3 ,1),imshow(A,[0,256]);
title(file)
subplot(1 ,3 ,2),imshow(B,[0,1]);
title(class_Name)
subplot(1 ,3 ,3),imshow(C,[0,4]);
title(Ori_Dev)
   
   
    saveas(gcf,[file,'.jpg']);
end

%C = num2cell([name,arg])

%writecell(C,'左弯.xls')