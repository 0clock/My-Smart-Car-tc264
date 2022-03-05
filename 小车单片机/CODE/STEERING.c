#include "headfile.h"
#include "STEERING.h"
#include "CAMERA.h"

extern float Col_Center[height_Inverse_Perspective_Max];//按从下往上的顺序存储中心线线的列号结果，不合法的全部为-2
extern int search_Lines;//扫描范围

float steering_Error = 0;//当前图像下的实际中线与理想正中线的误差

struct
{
    float last_error;//上次
    float current_error; //当前
    float KP;
    float KI;
    float KD;
}Steering_PID={0.0f,0.0f,30.0/200.0f,0.0f,0.0f};


//需要串口通信传过来的变量（必须配以执行变量更新的函数）
float steering_Target = 0;//目标角度（°），更新函数Set_Steering_Target(uint8 val)（范围-30，30）




void My_Init_Steering(void)
{
    gtm_pwm_init(ATOM2_CH0_P33_10, 100, 1500);//设置P33.10输出PWM波，频率100Hz，占空比1500/10000；用于舵机
}

void UART_Steering(void)
{
    uart_putchar(DEBUG_UART,0x00);
    uart_putchar(DEBUG_UART,0xff);
    uart_putchar(DEBUG_UART,0x05);
    uart_putchar(DEBUG_UART,0x01);//发送数据头
    uart_putchar(DEBUG_UART, (uint8)round((steering_Target - STEERING_MIN)/(STEERING_MAX-STEERING_MIN)*255));
    uart_putchar(DEBUG_UART,0x00);
    uart_putchar(DEBUG_UART,0xff);
    uart_putchar(DEBUG_UART,0x05);
    uart_putchar(DEBUG_UART,0x02);//发送数据尾
}

void Set_Steering_Target(uint8 val)
{
    steering_Target = ((float)val) / 256 * (STEERING_MAX-STEERING_MIN)+STEERING_MIN;
}

void Set_Steering(void)
{
    uint32 duty;
    //duty = 11.759*steering_Target + 4.4933
    //R-Square = 0.9975
    duty = 11.759*(steering_Target>0?steering_Target:(-steering_Target)) + 4.4933;
    if (duty>STEERING_DUTY_MAX)//保护电机
    {
        duty = STEERING_DUTY_MAX;
    }
    if (steering_Target == 0)
    {
        duty = 0;
    }
    pwm_duty(ATOM2_CH0_P33_10, (uint32)(1500+(steering_Target<0?1:-1)*duty));
}

void Cal_Steering_Error(float Cal_Steering_Range_of_Img)
{
    //根据Col_Center和扫描范围search_Lines计算误差（全局变量）

    float steering_Error_tmp = 0;
    float Col_Center_Init = 0;

    for (int i=0;i<search_Lines;i++)
    {
        if(Col_Center[i] != -2)
        {
            Col_Center_Init = Col_Center[i];
            break;
        }
    }

    for (int i=0;i<(search_Lines*Cal_Steering_Range_of_Img);i++)
    {
        if(Col_Center[i] != -2)
        {
            steering_Error_tmp = steering_Error_tmp + Col_Center[i] - Col_Center_Init;
        }
    }

        steering_Error = steering_Error_tmp /(10.0f * ratioOfPixelToHG);

}

//https://blog.csdn.net/qq_49487109/article/details/117963017 可参考
void Cal_Steering_Target(void)
{
    //由误差（全局变量，待定义）根据位置式PD原理求转向目标Steering_Target(范围-30~30，负数左转，正数右转)
    Steering_PID.last_error = Steering_PID.current_error;
    Steering_PID.current_error = steering_Error;

    steering_Target = (Steering_PID.KP * Steering_PID.current_error) +( Steering_PID.current_error - Steering_PID.last_error );

    if(steering_Target>30) steering_Target = 30;
    if(steering_Target<-30) steering_Target = - 30;
}
