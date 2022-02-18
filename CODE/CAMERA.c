#include "headfile.h"
#include "CAMERA.h"
#include "fastlz.h"//ѹ���㷨
#include <stdlib.h>
#include "OLED.h"


//��Ҫ����ͨ�����ȥ�������ô������ı���
IFX_ALIGN(4) uint8 mt9v03x_image_cutted[Y_WIDTH][X_WIDTH];//�ü����ԭʼͼ��
IFX_ALIGN(4) uint8 mt9v03x_image_cutted_compressed[Y_WIDTH][X_WIDTH];//ѹ�����ͼ��


//��Ҫ����ͨ�Ŵ������ı�������������ִ�б������µĺ�����
uint8 thresholding_Value = 20; //��Ӧ�ĸ��º���Ϊ��void Set_Thresholding_Value(uint8 val);
float cameraAlphaUpOrDown = 30 * 3.1415926 / 2 / 180;//����У��
float cameraThetaDown = 16.4 * 3.1415926 / 180;//��ҪУ��
float ratioOfMaxDisToHG = 4;//��Ӱ����ʾ����
float ratioOfPixelToHG = 0.033;//��Ӱ��ֱ���


//����Ҫ�������������
uint32 compressed_Size = 0;//ѹ����С
IFX_ALIGN(4) uint8 mt9v03x_image_cutted_thresholding[Y_WIDTH][X_WIDTH];
IFX_ALIGN(4) uint8 mt9v03x_image_cutted_thresholding_inversePerspective[height_Inverse_Perspective_Max][width_Inverse_Perspective_Max];
int width_Inverse_Perspective;
int height_Inverse_Perspective;


void My_Init_Camera(void)
{
    mt9v03x_init();
}

void UART_Image(void)
{
    uart_putchar(DEBUG_UART,0x00);
    uart_putchar(DEBUG_UART,0xff);
    uart_putchar(DEBUG_UART,0x01);
    uart_putchar(DEBUG_UART,0x01);//��������ͷ
    uart_putbuff(DEBUG_UART, *mt9v03x_image_cutted, X_WIDTH*Y_WIDTH);  //����ѹ�����ͼ��
    //uart_putbuff(DEBUG_UART, *mt9v03x_image_cutted_compressed, compressed_Size);  //����ѹ�����ͼ��
    uart_putchar(DEBUG_UART,0x00);
    uart_putchar(DEBUG_UART,0xff);
    uart_putchar(DEBUG_UART,0x01);
    uart_putchar(DEBUG_UART,0x02);//��������β
}

void UART_Thresholding_Value(void)
{
    uart_putchar(DEBUG_UART,0x00);
    uart_putchar(DEBUG_UART,0xff);
    uart_putchar(DEBUG_UART,0x02);
    uart_putchar(DEBUG_UART,0x01);//��������ͷ
    uart_putchar(DEBUG_UART, thresholding_Value);  //���Ͷ�ֵ����ֵ
    uart_putchar(DEBUG_UART,0x00);
    uart_putchar(DEBUG_UART,0xff);
    uart_putchar(DEBUG_UART,0x02);
    uart_putchar(DEBUG_UART,0x02);//��������β
}

void UART_Inverse_Perspective(void)
{
    uart_putchar(DEBUG_UART,0x00);
    uart_putchar(DEBUG_UART,0xff);
    uart_putchar(DEBUG_UART,0x03);
    uart_putchar(DEBUG_UART,0x01);//��������ͷ
    uart_putchar(DEBUG_UART, (uint8)round((cameraAlphaUpOrDown*2*180/3.1415926 - 0)/(90-0)*255));
    uart_putchar(DEBUG_UART, (uint8)round((cameraThetaDown*180/3.1415926 - 0)/(90-0)*255));
    uart_putchar(DEBUG_UART, (uint8)round((ratioOfMaxDisToHG - 0)/(15-0)*255));
    uart_putchar(DEBUG_UART, (uint8)round((ratioOfPixelToHG - 0)/(0.1-0)*255));
    uart_putchar(DEBUG_UART,0x00);
    uart_putchar(DEBUG_UART,0xff);
    uart_putchar(DEBUG_UART,0x03);
    uart_putchar(DEBUG_UART,0x02);//��������β
}

void Set_Thresholding_Value(uint8 val)
{
    thresholding_Value = val;
}

void Set_CameraAlphaUpOrDown(uint8 val)
{
    cameraAlphaUpOrDown = (((float)val)/255*(90-0)+0) * 3.1415926 / 2 / 180;
}
void Set_CameraThetaDown(uint8 val)
{
    cameraThetaDown = (((float)val)/255*(90-0)+0) * 3.1415926 / 180;
}
void Set_RatioOfMaxDisToHG(uint8 val)
{
    ratioOfMaxDisToHG = (((float)val)/255*(15-0)+0);
}
void Set_RatioOfPixelToHG(uint8 val)
{
    ratioOfPixelToHG = (((float)val)/255*(0.1-0)+0);
}

void Get_Cutted_Image(void)
{
    uint8 div_h, div_w;
    uint32 temp_h = 0;
    uint32 temp_w = 0;
    uint32 row_start = 0;
    uint32 lin_start = 0;

    div_h = MT9V03X_H/Y_WIDTH;
    div_w = MT9V03X_W/X_WIDTH;

    //������ȡͼ��
    if(Y_WIDTH * div_h != MT9V03X_H)
    {
        row_start = (MT9V03X_H - Y_WIDTH * div_h)/2;
        temp_h = row_start;
    }
    if(X_WIDTH * div_w != MT9V03X_W)
    {
        lin_start = (MT9V03X_W - X_WIDTH * div_w)/2;
    }
    for(int i = 0; i < Y_WIDTH; i++)
    {
        temp_w = lin_start;
        for(int j = 0; j < X_WIDTH; j++)
        {
            mt9v03x_image_cutted[i][j] = mt9v03x_image[temp_h][temp_w];
            temp_w += div_w;
        }
        temp_h += div_h;
    }
}



void Get_Thresholding_Image(void)
{
    for(int j = 0; j < Y_WIDTH; j++)
    {
        for(int i = 0; i < X_WIDTH; i++)
        {
            mt9v03x_image_cutted_thresholding[j][i] = mt9v03x_image_cutted[j][i]>thresholding_Value;
        }
    }
}

void Get_Inverse_Perspective_Image(void)
{
    width_Inverse_Perspective = (int)round(2 * (X_WIDTH) / (Y_WIDTH) * tan(cameraAlphaUpOrDown) / cos(cameraThetaDown) * ratioOfMaxDisToHG / ratioOfPixelToHG);
    height_Inverse_Perspective = (int)round(ratioOfMaxDisToHG / ratioOfPixelToHG);

    for (int j_Processed = 0;j_Processed<height_Inverse_Perspective;j_Processed++)
    {
        float y_Processed = (float)(height_Inverse_Perspective - 1 - j_Processed);
        float y = (tan(cameraThetaDown)-1/y_Processed/ratioOfPixelToHG)*Y_WIDTH/2/tan(cameraAlphaUpOrDown);
        int j = (int)round(-y + (Y_WIDTH-1)/2);
        if (j>=0 && j<Y_WIDTH)
        {
            for (int i_Processed = 0;i_Processed<width_Inverse_Perspective;i_Processed++)
            {
                float x_Processed = (float)i_Processed - ((float)width_Inverse_Perspective-1)/2;
                float x = x_Processed*(Y_WIDTH/2/tan(cameraAlphaUpOrDown)*sin(cameraThetaDown)-y*cos(cameraThetaDown))*ratioOfPixelToHG;
                int i = (int)round(x + (X_WIDTH-1)/2);
                if (i>=0 && i<X_WIDTH)
                {
                    mt9v03x_image_cutted_thresholding_inversePerspective[j_Processed][i_Processed] = mt9v03x_image_cutted_thresholding[j][i];
                }
                else
                {
                    mt9v03x_image_cutted_thresholding_inversePerspective[j_Processed][i_Processed] = 255;
                }
            }
        }
        else
        {
            for (int i_Processed = 1;i_Processed<width_Inverse_Perspective;i_Processed++)
            {
                mt9v03x_image_cutted_thresholding_inversePerspective[j_Processed][i_Processed] = 255;
            }
        }
    }
}
