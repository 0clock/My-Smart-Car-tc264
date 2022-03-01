#include "headfile.h"
#include "CAMERA.h"
#include "fastlz.h"//压缩算法
#include <stdlib.h>
#include "OLED.h"


//需要串口通信输出去，但不用传过来的变量
IFX_ALIGN(4) uint8 mt9v03x_image_cutted[Y_WIDTH_CAMERA][X_WIDTH_CAMERA];//裁剪后的原始图像
uint8 classification_Result;


//需要串口通信传过来的变量（必须配以执行变量更新的函数）
uint8 thresholding_Value = 128; //对应的更新函数为：void Set_Thresholding_Value(uint8 val);
float cameraAlphaUpOrDown = 40.0f * 3.1415926 / 2 / 180;//无需校正
float cameraThetaDown = 27.89191f * 3.1415926 / 180;//需要校正
float ratioOfMaxDisToHG = 5.915322f;//仅影响显示距离
float ratioOfPixelToHG = 0.1f;//仅影响分辨率


//不需要传输的其他变量
IFX_ALIGN(4) uint8 mt9v03x_image[MT9V03X_H][MT9V03X_W];//原始图像
IFX_ALIGN(4) uint8 mt9v03x_image_cutted_thresholding[Y_WIDTH_CAMERA][X_WIDTH_CAMERA];
IFX_ALIGN(4) uint8 mt9v03x_image_cutted_thresholding_inversePerspective[height_Inverse_Perspective_Max][width_Inverse_Perspective_Max];
int width_Inverse_Perspective;
int height_Inverse_Perspective;

float BayesTable[13][CLASS_NUM] = {{-1.190,-3.034,-2.802,-6.263,-0.934,-4.668,1.357,-2.981,-5.393,-0.645,-3.967,-6.710},
{14.146,15.653,17.148,17.552,17.588,13.615,14.721,18.589,17.731,14.509,16.188,19.415},
{15.653,14.146,18.589,17.731,14.509,16.188,14.721,17.148,17.552,17.588,13.615,19.415},
{-3.034,-1.190,-2.981,-5.393,-0.645,-3.967,1.357,-2.802,-6.263,-0.934,-4.668,-6.710},
{5.118,4.216,-0.315,-0.623,2.451,4.065,4.638,1.208,-3.510,-0.867,-3.273,4.480},
{6.607,6.552,15.825,12.612,8.794,9.528,12.419,15.823,15.220,6.437,17.269,18.047},
{6.552,6.607,15.823,15.220,6.437,17.269,12.419,15.825,12.612,8.794,9.528,18.047},
{4.216,5.118,1.208,-3.510,-0.867,-3.273,4.638,-0.315,-0.623,2.451,4.065,4.480},
{19.306,10.445,23.653,24.690,22.250,18.517,25.460,23.195,34.186,33.739,27.965,27.107},
{10.445,19.306,23.195,34.186,33.739,27.965,25.460,23.653,24.690,22.250,18.517,27.107},
{118.385,115.730,115.069,120.424,138.194,110.731,109.583,114.231,117.511,121.156,111.660,115.457},
{115.730,118.385,114.231,117.511,121.156,111.660,109.583,115.069,120.424,138.194,110.731,115.457},
{-114.584,-114.584,-127.289,-139.810,-154.666,-118.157,-123.839,-127.289,-139.810,-154.666,-118.157,-139.937}};
char *class_Name_Group[CLASS_NUM] = {"左弯", "右弯", "发现右环岛", "右环岛中心", "入右环岛", "出右环岛", "三岔路口", "发现左环岛", "左环岛中心", "入左环岛", "出左环岛", "十字路口"};


void My_Init_Camera(void)
{
    mt9v03x_init();
}

void UART_Image(void)
{
    uart_putchar(DEBUG_UART,0x00);
    uart_putchar(DEBUG_UART,0xff);
    uart_putchar(DEBUG_UART,0x01);
    uart_putchar(DEBUG_UART,0x01);//发送数据头
    uart_putbuff(DEBUG_UART, *mt9v03x_image_cutted, X_WIDTH_CAMERA*Y_WIDTH_CAMERA);  //发送压缩后的图像
    //uart_putbuff(DEBUG_UART, *mt9v03x_image_cutted_compressed, compressed_Size);  //发送压缩后的图像
    uart_putchar(DEBUG_UART,0x00);
    uart_putchar(DEBUG_UART,0xff);
    uart_putchar(DEBUG_UART,0x01);
    uart_putchar(DEBUG_UART,0x02);//发送数据尾
}

void UART_Thresholding_Value(void)
{
    uart_putchar(DEBUG_UART,0x00);
    uart_putchar(DEBUG_UART,0xff);
    uart_putchar(DEBUG_UART,0x02);
    uart_putchar(DEBUG_UART,0x01);//发送数据头
    uart_putchar(DEBUG_UART, thresholding_Value);  //发送二值化阈值
    uart_putchar(DEBUG_UART,0x00);
    uart_putchar(DEBUG_UART,0xff);
    uart_putchar(DEBUG_UART,0x02);
    uart_putchar(DEBUG_UART,0x02);//发送数据尾
}

void UART_Inverse_Perspective(void)
{
    uart_putchar(DEBUG_UART,0x00);
    uart_putchar(DEBUG_UART,0xff);
    uart_putchar(DEBUG_UART,0x03);
    uart_putchar(DEBUG_UART,0x01);//发送数据头
    uart_putchar(DEBUG_UART, (uint8)round((cameraAlphaUpOrDown*2*180/3.1415926 - 0)/(90-0)*255));
    uart_putchar(DEBUG_UART, (uint8)round((cameraThetaDown*180/3.1415926 - 0)/(90-0)*255));
    uart_putchar(DEBUG_UART, (uint8)round((ratioOfMaxDisToHG - 0)/(15-0)*255));
    uart_putchar(DEBUG_UART, (uint8)round((ratioOfPixelToHG - 0)/(0.1-0)*255));
    uart_putchar(DEBUG_UART,0x00);
    uart_putchar(DEBUG_UART,0xff);
    uart_putchar(DEBUG_UART,0x03);
    uart_putchar(DEBUG_UART,0x02);//发送数据尾
}

void UART_Classification(void)
{
    uart_putchar(DEBUG_UART,0x00);
    uart_putchar(DEBUG_UART,0xff);
    uart_putchar(DEBUG_UART,0x08);
    uart_putchar(DEBUG_UART,0x01);//发送数据头
    uart_putchar(DEBUG_UART, classification_Result);
    uart_putchar(DEBUG_UART,0x00);
    uart_putchar(DEBUG_UART,0xff);
    uart_putchar(DEBUG_UART,0x08);
    uart_putchar(DEBUG_UART,0x02);//发送数据尾
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

/*//压缩
void Get_Cutted_Image(void)
{
    uint8 div_h, div_w;
    uint32 temp_h = 0;
    uint32 temp_w = 0;
    uint32 row_start = 0;
    uint32 lin_start = 0;

    div_h = MT9V03X_H/Y_WIDTH_CAMERA;
    div_w = MT9V03X_W/X_WIDTH_CAMERA;

    //从中心取图像
    if(Y_WIDTH_CAMERA * div_h != MT9V03X_H)
    {
        row_start = (MT9V03X_H - Y_WIDTH_CAMERA * div_h)/2;
        temp_h = row_start;
    }
    if(X_WIDTH_CAMERA * div_w != MT9V03X_W)
    {
        lin_start = (MT9V03X_W - X_WIDTH_CAMERA * div_w)/2;
    }
    for(int i = 0; i < Y_WIDTH_CAMERA; i++)
    {
        temp_w = lin_start;
        for(int j = 0; j < X_WIDTH_CAMERA; j++)
        {
            mt9v03x_image_cutted[i][j] = mt9v03x_image[temp_h][temp_w];
            temp_w += div_w;
        }
        temp_h += div_h;
    }
}*/

//真正的裁剪
void Get_Cutted_Image(void)
{
    int origin_i=0,origin_j=0;
    origin_i = (MT9V03X_H - Y_WIDTH_CAMERA)/2;
    origin_j = (MT9V03X_W - X_WIDTH_CAMERA)/2;
    for(int i = 0; i < Y_WIDTH_CAMERA; i++)
    {
        for(int j = 0; j < X_WIDTH_CAMERA; j++)
        {
            mt9v03x_image_cutted[i][j] = mt9v03x_image[origin_i][origin_j];
            origin_j++;
        }
        origin_i++;
    }
}




void Get_Thresholding_Image(void)
{
    //Kmeans法更新二值化阈值
    float m[KMEANS_K][3] = {{85,0,0},{171,0,0},{255,0,0}};//第一列存最终分类结果，第二列存每种分类的样本数，第三列存中间变量
    float maxtimes = 2;
    for (int time = 0;time<maxtimes;time++)
    {
        for (int i = 0;i<KMEANS_K;i++)
        {
            m[i][1] = 0;
        }
        for (int i = 0;i<Y_WIDTH_CAMERA;i++)
        {
            for (int j = 0;j<X_WIDTH_CAMERA;j++)
            {
                float min_distance = fabs(mt9v03x_image_cutted[i][j]-m[0][0]);
                int min_distance_class = 0;
                for (int k=1;k<KMEANS_K;k++)
                {
                    if (fabs(mt9v03x_image_cutted[i][j]-m[k][0]) < min_distance)
                    {
                        min_distance = fabs(mt9v03x_image_cutted[i][j]-m[k][0]);
                        min_distance_class = k;
                    }
                }
                m[min_distance_class][1] = m[min_distance_class][1]+1;
                m[min_distance_class][2] = (m[min_distance_class][1]-1)/m[min_distance_class][1]*m[min_distance_class][2] + mt9v03x_image_cutted[i][j]/m[min_distance_class][1];
            }
        }
        for (int i = 0;i<KMEANS_K;i++)
        {
            m[i][0] = m[i][2];
        }
    }
    float max_cnt[2] = {m[0][1],m[1][1]};
    int max_cnt_class[2] = {0,1};
    if (m[0][1] >= m[1][1])
    {
        for (int i = 2;i<KMEANS_K;i++)
        {
            if (m[i][1]>max_cnt[0])
            {
                max_cnt[0] = m[i][1];
                max_cnt_class[0] = i;
            }
            else if (m[i][1]>max_cnt[1])
            {
                max_cnt[1] = m[i][1];
                max_cnt_class[1] = i;
            }
        }
    }
    else
    {
        for (int i = 2;i<KMEANS_K;i++)
        {
            if (m[i][1]>max_cnt[1])
            {
                max_cnt[1] = m[i][1];
                max_cnt_class[1] = i;
            }
            else if (m[i][1]>max_cnt[0])
            {
                max_cnt[0] = m[i][1];
                max_cnt_class[0] = i;
            }
        }
    }

    thresholding_Value = (uint8)(0.5*(m[max_cnt_class[0]][0]+m[max_cnt_class[1]][0]));

    for(int j = 0; j < Y_WIDTH_CAMERA; j++)
    {
        for(int i = 0; i < X_WIDTH_CAMERA; i++)
        {
            mt9v03x_image_cutted_thresholding[j][i] = mt9v03x_image_cutted[j][i]>thresholding_Value;
        }
    }
}

void Get_Inverse_Perspective_Image(void)
{
    int ratio=2;
    width_Inverse_Perspective = (int)round(2 * (X_WIDTH_CAMERA) / (ratio*Y_WIDTH_CAMERA) * tan(cameraAlphaUpOrDown) / cos(cameraThetaDown) * ratioOfMaxDisToHG / ratioOfPixelToHG);
    height_Inverse_Perspective = (int)round(ratioOfMaxDisToHG / ratioOfPixelToHG);

    for (int j_Processed = 0;j_Processed<height_Inverse_Perspective;j_Processed++)
    {
        float y_Processed = (float)(height_Inverse_Perspective - 1 - j_Processed);
        float y = (tan(cameraThetaDown)-1/y_Processed/ratioOfPixelToHG)*ratio*Y_WIDTH_CAMERA/2/tan(cameraAlphaUpOrDown);
        int j = (int)round(-y + (ratio*Y_WIDTH_CAMERA-1)/2);
        if (j>=0 && j<ratio*Y_WIDTH_CAMERA)
        {
            for (int i_Processed = 0;i_Processed<width_Inverse_Perspective;i_Processed++)
            {
                float x_Processed = (float)i_Processed - ((float)width_Inverse_Perspective-1)/2;
                float x = x_Processed*(ratio*Y_WIDTH_CAMERA/2/tan(cameraAlphaUpOrDown)*sin(cameraThetaDown)-y*cos(cameraThetaDown))*ratioOfPixelToHG;
                int i = (int)round(x + (X_WIDTH_CAMERA-1)/2);
                if (i>=0 && i<X_WIDTH_CAMERA)
                {
                    mt9v03x_image_cutted_thresholding_inversePerspective[j_Processed][i_Processed] = mt9v03x_image_cutted_thresholding[j/ratio][i];
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

void Get16(float* arg)
{
    int col_edge[5],row_edge[5];
    col_edge[0] = 0;
    col_edge[1] = width_Inverse_Perspective/4;
    col_edge[2] = 2*col_edge[1];
    col_edge[3] = 3*col_edge[1];
    col_edge[4] = width_Inverse_Perspective-1;
    row_edge[0] = 0;
    row_edge[1] = height_Inverse_Perspective/4;
    row_edge[2] = 2*row_edge[1];
    row_edge[3] = 3*row_edge[1];
    row_edge[4] = height_Inverse_Perspective-1;

    int white_cnt[16] = {0};
    int black_cnt[16] = {0};
    for (int i = 0;i<height_Inverse_Perspective;i++)
    {
        for (int j = 0;j<width_Inverse_Perspective;j++)
        {
            white_cnt[4*i/row_edge[1] + j/col_edge[1]] = (mt9v03x_image_cutted_thresholding_inversePerspective[i][j]==1) + white_cnt[4*i/row_edge[1] + j/col_edge[1]];
            black_cnt[4*i/row_edge[1] + j/col_edge[1]] = (mt9v03x_image_cutted_thresholding_inversePerspective[i][j]==0) + black_cnt[4*i/row_edge[1] + j/col_edge[1]];
        }
    }
    for (int i = 0;i<16;i++)
    {
        if ((white_cnt[i]+black_cnt[i]) == 0)
        {
           arg[i] = 0;
        }
        else
        {
           arg[i] = white_cnt[i]*1.0f/(white_cnt[i]+black_cnt[i]);
        }
    }
}

void Classification(void)
{
    float arg_Classification[16];
    Get16(arg_Classification);
    float classification_Data[CLASS_NUM] = {0};
    float classification_Data_max;
    for (uint8 i = 0;i<CLASS_NUM;i++)
    {
        classification_Data[i] = arg_Classification[1]*BayesTable[1][i]+
                                arg_Classification[2]*BayesTable[2][i]+
                                arg_Classification[3]*BayesTable[3][i]+
                                arg_Classification[4]*BayesTable[4][i]+
                                arg_Classification[5]*BayesTable[5][i]+
                                arg_Classification[6]*BayesTable[6][i]+
                                arg_Classification[7]*BayesTable[7][i]+
                                arg_Classification[8]*BayesTable[8][i]+
                                arg_Classification[10]*BayesTable[9][i]+
                                arg_Classification[11]*BayesTable[10][i]+
                                arg_Classification[14]*BayesTable[11][i]+
                                arg_Classification[15]*BayesTable[12][i]+
                                BayesTable[13][i];
        if (i==1)
        {
            classification_Data_max = classification_Data[1];
            classification_Result = 1;
        }
        else if (classification_Data[i]>classification_Data_max)
        {
            classification_Data_max = classification_Data[i];
            classification_Result = i;
        }
    }
}

