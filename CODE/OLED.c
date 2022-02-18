#include "headfile.h"
#include "OLED.h"
#include "CAMERA.h"
#include "UART.h"
#include "MOTOR.h"
#include "STEERING.h"

enum OLEDPage OLED_Page = Speed_Page;

void My_Init_OLED(void)
{
    oled_init();
    pit_interrupt_ms(CCU6_0, PIT_CH1, 10);//Update_OLED_per10ms
}

void Update_OLED_per10ms(void)
{
    //根据OLED应该显示的内容，刷新OLED屏幕
    switch(OLED_Page)
    {
       case Camera_Page:
           oled_dis_bmp(64, 128, *mt9v03x_image_cutted, thresholding_Value);
           break;
       case UART_Debug_Page:
           //显示内容由Cpu0_Main.c的串口通信部分完成
           break;
       case Speed_Page:
           OLED_PRINTF(0,0,"Speed:%01.05fm/s   ",speed_Measured);
           OLED_PRINTF(0,1,"Steering:%02.04f   ",steering_Target);
           break;
       default:
           break;
    }
}
