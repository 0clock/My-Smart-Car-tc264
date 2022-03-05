#include "headfile.h"
#include "CAMERA.h"
#include "fastlz.h"//压缩算法
#include <stdlib.h>
#include "OLED.h"


//需要串口通信输出去，但不用传过来的变量
IFX_ALIGN(4) uint8 mt9v03x_image_cutted[Y_WIDTH_CAMERA][X_WIDTH_CAMERA];//裁剪后的原始图像
uint8 classification_Result;

float Col_Center[height_Inverse_Perspective_Max] = {-2};//按从下往上的顺序存储中心线线的列号结果，不合法的全部为-2
int search_Lines;//指Col_Center的有效扫描行数，用于遍历Col_Center


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

float BayesTable[13][CLASS_NUM] = {{-3.420,-2.854,-4.667,-5.363,-2.945,-4.882,1.235,-9.615,-8.909,-2.680,-7.525,-8.391},
        {15.410,10.233,9.674,9.086,15.070,8.994,12.950,24.741,22.143,13.690,18.635,18.403},
        {10.233,15.410,24.741,22.143,13.690,18.635,12.950,9.674,9.086,15.070,8.994,18.403},
        {-2.854,-3.420,-9.615,-8.909,-2.680,-7.525,1.235,-4.667,-5.363,-2.945,-4.882,-8.391},
        {0.761,2.867,-0.149,-3.448,-2.142,2.116,1.934,0.369,-10.633,-5.238,-4.894,0.881},
        {8.207,1.513,7.723,6.257,7.265,8.238,11.747,13.404,17.776,5.030,16.593,16.923},
        {1.513,8.207,13.404,17.776,5.030,16.593,11.747,7.723,6.257,7.265,8.238,16.923},
        {2.867,0.761,0.369,-10.633,-5.238,-4.894,1.934,-0.149,-3.448,-2.142,2.116,0.881},
        {14.138,-15.595,-0.647,-1.221,4.191,-2.929,12.334,10.959,34.429,21.828,22.361,16.250},
        {-15.595,14.138,10.959,34.429,21.828,22.361,12.334,-0.647,-1.221,4.191,-2.929,16.250},
        {169.243,179.710,166.997,176.743,203.214,169.881,156.108,176.459,157.122,179.855,166.889,163.575},
        {179.710,169.243,176.459,157.122,179.855,166.889,156.108,166.997,176.743,203.214,169.881,163.575},
        {-152.679,-152.679,-158.761,-165.717,-197.104,-158.416,-147.119,-158.761,-165.717,-197.104,-158.416,-166.916}};
char *class_Name_Group[CLASS_NUM+1] = {"左弯", "右弯", "发现右环岛", "右环岛中心", "入右环岛", "出右环岛", "三岔路口", "发现左环岛", "左环岛中心", "入左环岛", "出左环岛", "十字路口","直道"};


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
        origin_j = (MT9V03X_W - X_WIDTH_CAMERA)/2;
        for(int j = 0; j < X_WIDTH_CAMERA; j++)
        {
            mt9v03x_image_cutted[i][j] = mt9v03x_image[origin_i][origin_j];
            origin_j++;
        }
        origin_i+=2;
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
            if (m[i][1]>max_cnt[0] && m[i][0]<GOD_LIGHT)
            {
                max_cnt[0] = m[i][1];
                max_cnt_class[0] = i;
            }
            else if (m[i][1]>max_cnt[1] && m[i][0]<GOD_LIGHT)
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
            if (m[i][1]>max_cnt[1] && m[i][0]<GOD_LIGHT)
            {
                max_cnt[1] = m[i][1];
                max_cnt_class[1] = i;
            }
            else if (m[i][1]>max_cnt[0] && m[i][0]<GOD_LIGHT)
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

uint8 Classification(void)
{
    float arg_Classification[16];
    Get16(arg_Classification);
    float classification_Data[CLASS_NUM] = {0};
    float classification_Data_max;
    uint8 classification_Result_temp;
    for (uint8 i = 0;i<CLASS_NUM;i++)
    {
        classification_Data[i] = arg_Classification[0]*BayesTable[0][i]+
                                arg_Classification[1]*BayesTable[1][i]+
                                arg_Classification[2]*BayesTable[2][i]+
                                arg_Classification[3]*BayesTable[3][i]+
                                arg_Classification[4]*BayesTable[4][i]+
                                arg_Classification[5]*BayesTable[5][i]+
                                arg_Classification[6]*BayesTable[6][i]+
                                arg_Classification[7]*BayesTable[7][i]+
                                arg_Classification[9]*BayesTable[8][i]+
                                arg_Classification[10]*BayesTable[9][i]+
                                arg_Classification[13]*BayesTable[10][i]+
                                arg_Classification[14]*BayesTable[11][i]+
                                BayesTable[12][i];
        if (i==0)
        {
            classification_Data_max = classification_Data[0];
            classification_Result_temp = 0;
        }
        else if (classification_Data[i]>classification_Data_max)
        {
            classification_Data_max = classification_Data[i];
            classification_Result_temp = i;
        }
    }
    return classification_Result_temp;
}


int Check_Straight(void)
{
    int start_Row = height_Inverse_Perspective-1;
    int start_Col[2] = {width_Inverse_Perspective/2,width_Inverse_Perspective/2};
    int Col_Left[SEARCH_LINES_STRAIGHT] = {-2};
    int Col_Right[SEARCH_LINES_STRAIGHT] = {-2};
    for (int i=0;i<SEARCH_LINES_STRAIGHT;i++)
    {
        Col_Left[i] = -2;
        Col_Right[i] = -2;
    }
    for (int i=0;i<SEARCH_LINES_STRAIGHT;i++)
    {
        if (mt9v03x_image_cutted_thresholding_inversePerspective[start_Row][start_Col[0]] == 255 || mt9v03x_image_cutted_thresholding_inversePerspective[start_Row][start_Col[1]] == 255 || start_Col[0]>start_Col[1])
        {
            start_Row = start_Row - 1;
            continue;
        }
        if (mt9v03x_image_cutted_thresholding_inversePerspective[start_Row][start_Col[0]] == 1)
        {
            while (mt9v03x_image_cutted_thresholding_inversePerspective[start_Row][start_Col[0]] == 1 && start_Col[0]>=1)
            {
                start_Col[0] = start_Col[0] - 1;
            }
            start_Col[0] = start_Col[0] + 1;
            if (mt9v03x_image_cutted_thresholding_inversePerspective[start_Row][start_Col[0]-1] == 0)
            {
                Col_Left[i] = start_Col[0];
            }
        }
        else
        {
            while (mt9v03x_image_cutted_thresholding_inversePerspective[start_Row][start_Col[0]] == 0 && start_Col[0]<=width_Inverse_Perspective-2)
            {
                start_Col[0] = start_Col[0] + 1;
            }
            if (mt9v03x_image_cutted_thresholding_inversePerspective[start_Row][start_Col[0]] == 1)
            {
                Col_Left[i] = start_Col[0];
            }
        }
        if (mt9v03x_image_cutted_thresholding_inversePerspective[start_Row][start_Col[1]] == 0)
        {
            while (mt9v03x_image_cutted_thresholding_inversePerspective[start_Row][start_Col[1]] == 0 && start_Col[1]>=1)
            {
                start_Col[1] = start_Col[1] - 1;
            }
            if (mt9v03x_image_cutted_thresholding_inversePerspective[start_Row][start_Col[1]] == 1)
            {
                Col_Right[i] = start_Col[1];
            }
        }
        else
        {
            while (mt9v03x_image_cutted_thresholding_inversePerspective[start_Row][start_Col[1]] == 1 && start_Col[1]<=width_Inverse_Perspective-2)
            {
                start_Col[1] = start_Col[1] + 1;
            }
            start_Col[1] = start_Col[1] - 1;
            if (mt9v03x_image_cutted_thresholding_inversePerspective[start_Row][start_Col[1]+1] == 0)
            {
                Col_Right[i] = start_Col[1];
            }
        }
        start_Row = start_Row - 1;
    }

    int max_Col_Left = 0;
    int min_Col_Left = width_Inverse_Perspective;
    int detect_Left = 0;
    int max_Col_Right = 0;
    int min_Col_Right = width_Inverse_Perspective;
    int detect_Right = 0;
    for (int i=0;i<SEARCH_LINES_STRAIGHT;i++)
    {
        if (Col_Left[i] != -2)
        {
            detect_Left = 1;
            if (Col_Left[i] > max_Col_Left)
            {
                max_Col_Left = Col_Left[i];
            }
            if (Col_Left[i] < min_Col_Left)
            {
                min_Col_Left = Col_Left[i];
            }
        }
        else if (detect_Left == 1)
        {
            return 0;
        }

        if (Col_Right[i] != -2)
        {
            detect_Right = 1;
            if (Col_Right[i] > max_Col_Right)
            {
                max_Col_Right = Col_Right[i];
            }
            if (Col_Right[i] < min_Col_Right)
            {
                min_Col_Right = Col_Right[i];
            }
        }
        else if (detect_Right == 1)
        {
            return 0;
        }
    }
    return ((max_Col_Left - min_Col_Left <= STRAIGHT_CONDITION && max_Col_Left >= min_Col_Left)
            && (max_Col_Right - min_Col_Right <= STRAIGHT_CONDITION && max_Col_Right >= min_Col_Right)
            && max_Col_Left < min_Col_Right);
}


void DrawCenterLine(void)
{
    search_Lines = ((int)(height_Inverse_Perspective/(10.0f*ratioOfPixelToHG)));//一共要扫描多少行，最大是图片宽

    // 对于左弯、右弯，可以采用，特征是滤波是负数，用于超前转向，以免冲出弯道
    if (strcmp(class_Name_Group[classification_Result],"左弯") || strcmp(class_Name_Group[classification_Result],"右弯"))
    {
        DrawCenterLinewithConfig(-0.5);
    }
    // 对于十字路口、直道，可以采用，特征是滤波是较大正数，用于避免中心线有过大的波动
    else if (strcmp(class_Name_Group[classification_Result],"十字路口") || strcmp(class_Name_Group[classification_Result],"直道"))
    {
        DrawCenterLinewithConfig(0.7);
    }
}

//filter滤波系数，正数时是低通滤波，负数时相当于高通滤波
void DrawCenterLinewithConfig(float filter)
{
    int start_Row = height_Inverse_Perspective-1;//标记当前在处理哪一行，从最后一行开始
    int start_Col[2] = {width_Inverse_Perspective/2,width_Inverse_Perspective/2};//标记当前在处理哪一列，start_Col(1)指左线，start_Col(2)指右线，默认从中心开始
    int Col_Left[height_Inverse_Perspective_Max] = {-2};//按从下往上的顺序存储左线的列号结果，不合法的全部为-2
    int Col_Right[height_Inverse_Perspective_Max] = {-2};//按从下往上的顺序存储右线的列号结果，不合法的全部为-2
    for (int i=0;i<search_Lines;i++)
    {
        Col_Left[i] = -2;
        Col_Right[i] = -2;
        Col_Center[i] = -2;
    }
    for (int i=0;i<search_Lines;i++)//从最后一行开始逐行扫描，一共扫描search_Lines行
    {
        if (mt9v03x_image_cutted_thresholding_inversePerspective[start_Row][start_Col[0]] == 255 || mt9v03x_image_cutted_thresholding_inversePerspective[start_Row][start_Col[1]] == 255 || start_Col[0]>start_Col[1])
        {
            start_Row = start_Row - 1;
            continue;
        }//在当前的行里，如果左线或右线有一个是255区域（未知区域），说明还没有进入到真正的视角区域（0区域或1区域）；或者如果左线跑到右线的右边去了，则说明不是道路了
        if (mt9v03x_image_cutted_thresholding_inversePerspective[start_Row][start_Col[0]] == 1) //如果左线发现1区域（道路）
        {
            while (mt9v03x_image_cutted_thresholding_inversePerspective[start_Row][start_Col[0]] == 1 && start_Col[0]>=1)
            {
                start_Col[0] = start_Col[0] - 1;
            }
            start_Col[0] = start_Col[0] + 1;//则左线持续向左扫描直到不再是1区域（道路），有可能是0或255区域
            if (mt9v03x_image_cutted_thresholding_inversePerspective[start_Row][start_Col[0]-1] == 0)//查看此时是否是0区域（背景）
            {
                Col_Left[i] = start_Col[0];//只有是0区域的才可以将列号存储到左线里
            }
        }
        else//如果左线发现0区域（背景）
        {
            while (mt9v03x_image_cutted_thresholding_inversePerspective[start_Row][start_Col[0]] == 0 && start_Col[0]<=width_Inverse_Perspective-2)
            {
                start_Col[0] = start_Col[0] + 1;
            }//则左线持续向右扫描直到不再是0区域（背景），有可能是1或255区域
            if (mt9v03x_image_cutted_thresholding_inversePerspective[start_Row][start_Col[0]] == 1)//查看此时是否是1区域（道路）
            {
                Col_Left[i] = start_Col[0];//只有是1区域的才可以将列号存储到左线里
            }
        }
        if (mt9v03x_image_cutted_thresholding_inversePerspective[start_Row][start_Col[1]] == 0)//如果右线发现0区域（背景）
        {
            while (mt9v03x_image_cutted_thresholding_inversePerspective[start_Row][start_Col[1]] == 0 && start_Col[1]>=1)
            {
                start_Col[1] = start_Col[1] - 1;
            }//则右线持续向左扫描直到不再是0区域（背景），有可能是1或255区域
            if (mt9v03x_image_cutted_thresholding_inversePerspective[start_Row][start_Col[1]] == 1)
            {
                Col_Right[i] = start_Col[1];//只有是1区域的才可以将列号存储到右线里
            }
        }
        else//如果右线发现1区域（道路）
        {
            while (mt9v03x_image_cutted_thresholding_inversePerspective[start_Row][start_Col[1]] == 1 && start_Col[1]<=width_Inverse_Perspective-2)
            {
                start_Col[1] = start_Col[1] + 1;
            }
            start_Col[1] = start_Col[1] - 1;//则右线持续向右扫描直到不再是1区域（道路），有可能是0或255区域
            if (mt9v03x_image_cutted_thresholding_inversePerspective[start_Row][start_Col[1]+1] == 0)
            {
                Col_Right[i] = start_Col[1];//只有是0区域的才可以将列号存储到右线里
            }
        }
        start_Row = start_Row - 1; //左右线扫描完毕，标记行进入上一行，给下一次左右线扫描做准备

        //下面是中心线计算
        if (start_Col[0]>start_Col[1])
        {
            continue;
        }
        //中心线计算有4种情况：左线合法(!=-2)或非法(==-2)）、右线合法(!=-2)或非法(==-2)）
        if (Col_Right[i]!=-2 && Col_Left[i]!= -2) //左线合法，右线合法
        {
            if (Col_Center[i-1]!=-2) //如果上一个中心线也合法
            {
                Col_Center[i] = filter*Col_Center[i-1]+(1-filter)*0.5*(Col_Right[i] + Col_Left[i]); //根据上一次的中心线、这一次左右线中值，用滤波计算这次的中心线
            }
            else //如果上一个中心线不合法
            {
                Col_Center[i] = filter*width_Inverse_Perspective/2.0+(1-filter)*0.5*(Col_Right[i] + Col_Left[i]); //假定上一次中心线在正中间，根据上一次中心线、这一次左右线中值，用滤波计算这次的中心线
            }
        }
        else if (Col_Right[i]!=-2 && Col_Left[i]==-2) //左线非法，右线合法
        {
            if (Col_Center[i-1]!=-2 && Col_Right[i-1]!=-2) //如果上一次中心线合法，上一次右线合法
            {
                Col_Center[i] = filter*Col_Center[i-1]+(1-filter)*(Col_Right[i]-Col_Right[i-1]+Col_Center[i-1]); //根据上一次的中心线、假定的这一次中心线（保持上一次和右线的距离），用滤波计算这次的中心线
            }
            else if (Col_Center[i-1]==-2 && Col_Right[i-1]!=-2) //如果上一次中心线非法，上一次右线合法
            {
                Col_Center[i] = filter*width_Inverse_Perspective/2.0+(1-filter)*(Col_Right[i]-Col_Right[i-1]+width_Inverse_Perspective/2.0);//假定上一次中心线在正中间，根据上一次的中心线、假定的这一次中心线（保持上一次和右线的距离），用滤波计算这次的中心线
            }
            else if (Col_Center[i-1]!=-2 && Col_Right[i-1]==-2) //如果上一次中心线合法，上一次右线非法
            {
                Col_Center[i] = Col_Center[i-1]; //这一次中心线继续使用上一次的中心线
            }
            else //如果上一次中心线非法，上一次右线非法
            {
                Col_Center[i] = width_Inverse_Perspective/2.0; //这一次中心线直接假定为正中间
            }
        }
        else if (Col_Right[i]==-2 && Col_Left[i]!=-2) //左线合法，右线非法，与上文类似
        {
            if (Col_Center[i-1]!=-2 && Col_Left[i-1]!=-2)
            {
                Col_Center[i] = filter*Col_Center[i-1]+(1-filter)*(Col_Left[i]-Col_Left[i-1]+Col_Center[i-1]);
            }
            else if (Col_Center[i-1]==-2 && Col_Left[i-1]!=-2)
            {
                Col_Center[i] = filter*width_Inverse_Perspective/2.0+(1-filter)*(Col_Left[i]-Col_Left[i-1]+width_Inverse_Perspective/2.0);
            }
            else if (Col_Center[i-1]!=-2 && Col_Left[i-1]==-2)
            {
                Col_Center[i] = Col_Center[i-1];
            }
            else
            {
                Col_Center[i] = width_Inverse_Perspective/2.0;
            }
        }
        else //左线非法，右线非法
        {
            if (Col_Center[i-1]!=-2) //如果上一次中心线合法
            {
                Col_Center[i] = filter*Col_Center[i-1]+(1-filter)*width_Inverse_Perspective/2.0; //根据上一次的中心线、假定的这一次中心线（取正中间），用滤波计算这次的中心线
            }
            else //如果上一次中心线非法
            {
                Col_Center[i] = width_Inverse_Perspective/2.0; //这一次中心线直接假定为正中间
            }
        }
        //中心线计算完毕
    }
}
