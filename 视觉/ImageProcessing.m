function [img_Processed] = ImageProcessing(img, cameraAlphaUpDown, cameraThetaDown, ratioOfMaxDisToHG, ratioOfPixelToHG, ratio)
% ���������
% img��ԭʼ�Ҷ�ͼ�����
% cameraAlphaUpDown���������Ұ�������Žǣ�������׼ȷ��
% cameraThetaDown����������ᳯ��ʱƫ��ˮƽ�ĽǶȣ�������׼ȷ��
% ratioOfMaxDisToHG��ת���������Զ��ʾ���������������߶ȵı���
% ratioOfPixelToHG��ת�������ÿ�����ص㳤�������������߶ȵı���
% ���������
% img_Processed��ת������Ҷ�ͼ�������ײ��м�����������λ��
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

