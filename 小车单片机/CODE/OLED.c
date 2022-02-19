#include "headfile.h"
#include "OLED.h"
#include "CAMERA.h"
#include "UART.h"
#include "MOTOR.h"
#include "STEERING.h"

enum OLEDPage OLED_Page = Speed_Page;
uint8 OLED_EN = TRUE;//用于表示OLED屏幕是否开启

void My_Init_OLED(void)
{
    oled_init();
    pit_interrupt_ms(CCU6_0, PIT_CH1, 16);//Update_OLED_per16ms
}

void Update_OLED_per16ms(void)
{
    if (OLED_EN == TRUE)
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
           case UART_Setting_Page:
               if (UART_EN == TRUE)
               {
                   OLED_PRINTF(0,0,"UART: ON ");
               }
               else
               {
                   OLED_PRINTF(0,0,"UART: OFF");
               }
               break;
           case OLED_Setting_Page:
               if (OLED_EN == TRUE)
               {
                   OLED_PRINTF(0,0,"OLED: ON ");
               }
               else
               {
                   OLED_PRINTF(0,0,"OLED: OFF");
               }
               break;
           default:
               oled_fill(0x00);
               break;
        }
    }
    else
    {
        oled_fill(0x00);
    }
}
