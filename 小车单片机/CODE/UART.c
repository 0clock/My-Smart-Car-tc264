#include "headfile.h"
#include "UART.h"
#include "OLED.h"
#include "MOTOR.h"
#include "STEERING.h"
#include "CAMERA.h"

uint8 data_Buffer[RECEIVE_LENGTH + CACHE_LENGTH];//接收缓冲区，等于所有命令长度的2倍时最好
uint8 *dat = data_Buffer;//接收缓冲区的指针
uint8 UART_Flag_RX = FALSE;//用于指示接收缓冲区是否达到了所需长度
uint8 UART_Flag_TX = FALSE;//用于指示是否可以发送数据
uint8 UART_EN = TRUE;//用于表示上位机功能是否开启
uint8 data_Buffer_Shadow[RECEIVE_LENGTH];//用于取出接收缓冲区
uint8 UART_Flag_NO_IMAGE = TRUE;//用于表示是否不用传图片到上位机

void My_Init_UART(void)
{
    pit_interrupt_ms(CCU6_1, PIT_CH1, 10);//10ms检查一次UART接收数据
}

void UART(enum UARTstate state)
{
    if (state == Send)
    {
        //通过串口发送数据给上位机
        if (UART_Flag_TX == TRUE || UART_Flag_NO_IMAGE == TRUE)
        {
            if (UART_Flag_NO_IMAGE == FALSE)
            {
                //发送裁剪后的摄像头原画面，数据头00-FF-01-01，数据长度X_WIDTH*Y_WIDTH字节，数据尾00-FF-01-02
                UART_Image();
            }

            //发送二值化阈值，数据头00-FF-02-01，数据长度1字节，数据尾00-FF-02-02
            UART_Thresholding_Value();

            //发送逆透视参数，数据头00-FF-03-01，数据长度4字节，数据尾00-FF-03-02
            UART_Inverse_Perspective();

            //发送直流电机速度参数，数据头00-FF-04-01，数据长度2字节，数据尾00-FF-04-02
            UART_Speed();

            //发送舵机角度参数，数据头00-FF-05-01，数据长度1字节，数据尾00-FF-05-02
            UART_Steering();

            UART_Flag_TX = FALSE;
        }
    }
    else if (state == Read)
    {
        //通过串口接收数据，执行相应动作
        if (UART_Flag_RX == TRUE)//缓冲区接收数据满足了所需长度
        {
            uint16 i = 0;
            for (i=0;i<RECEIVE_LENGTH;i++)
            {
                data_Buffer_Shadow[i] = data_Buffer[i];
            }//把缓冲区内容复制过来
            if (OLED_Page == UART_Debug_Page)
            {
                oled_uint16(0, 0, data_Buffer_Shadow[0]);
                oled_uint16(36, 0, data_Buffer_Shadow[1]);
                oled_uint16(36*2, 0, data_Buffer_Shadow[2]);
                oled_uint16(0, 1, data_Buffer_Shadow[3]);
                oled_uint16(36, 1, data_Buffer_Shadow[4]);
                oled_uint16(36*2, 1, data_Buffer_Shadow[5]);
                oled_uint16(0, 2, data_Buffer_Shadow[6]);
                oled_uint16(36, 2, data_Buffer_Shadow[7]);
                oled_uint16(36*2, 2, data_Buffer_Shadow[8]);
                oled_uint16(0, 3, data_Buffer_Shadow[9]);
                oled_uint16(36, 3, data_Buffer_Shadow[10]);
                oled_uint16(36*2, 3, data_Buffer_Shadow[11]);
                oled_uint16(0, 4, data_Buffer_Shadow[12]);
                oled_uint16(36, 4, data_Buffer_Shadow[13]);
                oled_uint16(36*2, 4, data_Buffer_Shadow[14]);
                oled_uint16(0, 5, data_Buffer_Shadow[15]);
                oled_uint16(36, 5, data_Buffer_Shadow[16]);
                oled_uint16(36*2, 5, data_Buffer_Shadow[17]);
            }
            for (i=0;i<RECEIVE_LENGTH-4;i++)
            {
                //接收是否传输图片信号，数据头00-FF-01-01，数据长度1字节，数据尾00-FF-01-02
                if (data_Buffer_Shadow[i] == 0x00 && data_Buffer_Shadow[i+1] == 0xFF && data_Buffer_Shadow[i+2] == 0x01 && data_Buffer_Shadow[i+3] == 0x01)
                {
                    if (i<RECEIVE_LENGTH-8 && data_Buffer_Shadow[i+5] == 0x00 && data_Buffer_Shadow[i+6] == 0xFF && data_Buffer_Shadow[i+7] == 0x01 && data_Buffer_Shadow[i+8] == 0x02)
                    {
                        UART_Flag_NO_IMAGE = data_Buffer_Shadow[i+4];
                    }
                }
                //接收二值化阈值，数据头00-FF-02-01，数据长度1字节，数据尾00-FF-02-02
                if (data_Buffer_Shadow[i] == 0x00 && data_Buffer_Shadow[i+1] == 0xFF && data_Buffer_Shadow[i+2] == 0x02 && data_Buffer_Shadow[i+3] == 0x01)
                {
                    if (i<RECEIVE_LENGTH-8 && data_Buffer_Shadow[i+5] == 0x00 && data_Buffer_Shadow[i+6] == 0xFF && data_Buffer_Shadow[i+7] == 0x02 && data_Buffer_Shadow[i+8] == 0x02)
                    {
                        Set_Thresholding_Value(data_Buffer_Shadow[i+4]);
                    }
                }
                //接收逆透视参数，数据头00-FF-03-01，数据长度4字节，数据尾00-FF-03-02
                if (data_Buffer_Shadow[i] == 0x00 && data_Buffer_Shadow[i+1] == 0xFF && data_Buffer_Shadow[i+2] == 0x03 && data_Buffer_Shadow[i+3] == 0x01)
                {
                    if (i<RECEIVE_LENGTH-11 && data_Buffer_Shadow[i+8] == 0x00 && data_Buffer_Shadow[i+9] == 0xFF && data_Buffer_Shadow[i+10] == 0x03 && data_Buffer_Shadow[i+11] == 0x02)
                    {
                        Set_CameraAlphaUpOrDown(data_Buffer_Shadow[i+4]);
                        Set_CameraThetaDown(data_Buffer_Shadow[i+5]);
                        Set_RatioOfMaxDisToHG(data_Buffer_Shadow[i+6]);
                        Set_RatioOfPixelToHG(data_Buffer_Shadow[i+7]);
                    }
                }
                //接收速度参数，数据头00-FF-04-01，数据长度1字节，数据尾00-FF-04-02
                if (data_Buffer_Shadow[i] == 0x00 && data_Buffer_Shadow[i+1] == 0xFF && data_Buffer_Shadow[i+2] == 0x04 && data_Buffer_Shadow[i+3] == 0x01)
                {
                    if (i<RECEIVE_LENGTH-8 && data_Buffer_Shadow[i+5] == 0x00 && data_Buffer_Shadow[i+6] == 0xFF && data_Buffer_Shadow[i+7] == 0x04 && data_Buffer_Shadow[i+8] == 0x02)
                    {
                        Set_Speed_Target(data_Buffer_Shadow[i+4]);
                    }
                }
                //接收舵机参数，数据头00-FF-05-01，数据长度1字节，数据尾00-FF-05-02
                if (data_Buffer_Shadow[i] == 0x00 && data_Buffer_Shadow[i+1] == 0xFF && data_Buffer_Shadow[i+2] == 0x05 && data_Buffer_Shadow[i+3] == 0x01)
                {
                    if (i<RECEIVE_LENGTH-8 && data_Buffer_Shadow[i+5] == 0x00 && data_Buffer_Shadow[i+6] == 0xFF && data_Buffer_Shadow[i+7] == 0x05 && data_Buffer_Shadow[i+8] == 0x02)
                    {
                        Set_Steering_Target(data_Buffer_Shadow[i+4]);
                    }
                }


                //...
            }//逐位查找数据头

            UART_Flag_RX = FALSE;
        }
    }

}
