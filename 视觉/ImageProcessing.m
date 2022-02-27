function [img_Processed] = ImageProcessing(img, cameraAlphaUpDown, cameraThetaDown, ratioOfMaxDisToHG, ratioOfPixelToHG, ratio)
% 输入参数：
% img：原始灰度图像矩阵
% cameraAlphaUpDown：摄像机视野的上下张角（尽可能准确）
% cameraThetaDown：摄像机视轴朝下时偏离水平的角度（尽可能准确）
% ratioOfMaxDisToHG：转换结果的最远显示距离相对于摄像机高度的比例
% ratioOfPixelToHG：转换结果的每个像素点长度相对于摄像机高度的比例
% 输出参数：
% img_Processed：转换结果灰度图像矩阵，最底部中间代表摄像机的位置
    img = double(img);
    width = size(img,2);
    height = ratio.*size(img,1);
    cameraAlphaUpOrDown = cameraAlphaUpDown./2./180.*3.1415926;
    cameraThetaDown = cameraThetaDown./180.*3.1415926;
    width_Processed = round(2.*width./height.*tan(cameraAlphaUpOrDown)./cos(cameraThetaDown).*ratioOfMaxDisToHG./ratioOfPixelToHG);
    height_Processed = round(ratioOfMaxDisToHG./ratioOfPixelToHG);
    img_Processed = 255.*ones(height_Processed,width_Processed); 
    for j_Processed = 1:height_Processed
        y_Processed = 1+height_Processed - j_Processed;
        y = (tan(cameraThetaDown)-1./y_Processed./ratioOfPixelToHG).*height./2./tan(cameraAlphaUpOrDown);
        j = round(-y + (1+height)./2);
        if (j>0 && j<height+1)
            for i_Processed = 1:width_Processed
                x_Processed = i_Processed - (1+width_Processed)./2;
                x = x_Processed.*(height./2./tan(cameraAlphaUpOrDown).*sin(cameraThetaDown)-y.*cos(cameraThetaDown)).*ratioOfPixelToHG;
                i = round(x + (1+width)./2);
                if (i>0 && i<width+1)
                    img_Processed(j_Processed,i_Processed) = img(ceil(j/ratio),i);
                end
            end
        end
    end
    img_Processed = uint8(img_Processed);
end

