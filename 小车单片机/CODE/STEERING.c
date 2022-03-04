#include "headfile.h"
#include "STEERING.h"

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

void Cal_Steering_Error(void)
{
    //根据Col_Center和扫描范围search_Lines计算误差（全局变量，待定义），待完成
}

void Cal_Steering_Target(void)
{
    //待完成，由误差（全局变量，待定义）根据位置式PD原理求转向目标Steering_Target(范围-30~30，负数左转，正数右转)
}
