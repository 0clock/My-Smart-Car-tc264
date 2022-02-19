#include "headfile.h"
#include "UART.h"
#include "OLED.h"
#include "MOTOR.h"
#include "STEERING.h"
#include "CAMERA.h"

uint8 data_Buffer[RECEIVE_LENGTH + CACHE_LENGTH];//���ջ�������������������ȵ�2��ʱ���
uint8 *dat = data_Buffer;//���ջ�������ָ��
uint8 UART_Flag_RX = FALSE;//����ָʾ���ջ������Ƿ�ﵽ�����賤��
uint8 UART_Flag_TX = FALSE;//����ָʾ�Ƿ���Է�������
uint8 UART_EN = TRUE;//���ڱ�ʾ��λ�������Ƿ���
uint8 data_Buffer_Shadow[RECEIVE_LENGTH];//����ȡ�����ջ�����
uint8 UART_Flag_NO_IMAGE = TRUE;//���ڱ�ʾ�Ƿ��ô�ͼƬ����λ��

void My_Init_UART(void)
{
    pit_interrupt_ms(CCU6_1, PIT_CH1, 10);//10ms���һ��UART��������
}

void UART(enum UARTstate state)
{
    if (state == Send)
    {
        //ͨ�����ڷ������ݸ���λ��
        if (UART_Flag_TX == TRUE || UART_Flag_NO_IMAGE == TRUE)
        {
            if (UART_Flag_NO_IMAGE == FALSE)
            {
                //���Ͳü��������ͷԭ���棬����ͷ00-FF-01-01�����ݳ���X_WIDTH*Y_WIDTH�ֽڣ�����β00-FF-01-02
                UART_Image();
            }

            //���Ͷ�ֵ����ֵ������ͷ00-FF-02-01�����ݳ���1�ֽڣ�����β00-FF-02-02
            UART_Thresholding_Value();

            //������͸�Ӳ���������ͷ00-FF-03-01�����ݳ���4�ֽڣ�����β00-FF-03-02
            UART_Inverse_Perspective();

            //����ֱ������ٶȲ���������ͷ00-FF-04-01�����ݳ���2�ֽڣ�����β00-FF-04-02
            UART_Speed();

            //���Ͷ���ǶȲ���������ͷ00-FF-05-01�����ݳ���1�ֽڣ�����β00-FF-05-02
            UART_Steering();

            UART_Flag_TX = FALSE;
        }
    }
    else if (state == Read)
    {
        //ͨ�����ڽ������ݣ�ִ����Ӧ����
        if (UART_Flag_RX == TRUE)//�����������������������賤��
        {
            uint16 i = 0;
            for (i=0;i<RECEIVE_LENGTH;i++)
            {
                data_Buffer_Shadow[i] = data_Buffer[i];
            }//�ѻ��������ݸ��ƹ���
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
                //�����Ƿ���ͼƬ�źţ�����ͷ00-FF-01-01�����ݳ���1�ֽڣ�����β00-FF-01-02
                if (data_Buffer_Shadow[i] == 0x00 && data_Buffer_Shadow[i+1] == 0xFF && data_Buffer_Shadow[i+2] == 0x01 && data_Buffer_Shadow[i+3] == 0x01)
                {
                    if (i<RECEIVE_LENGTH-8 && data_Buffer_Shadow[i+5] == 0x00 && data_Buffer_Shadow[i+6] == 0xFF && data_Buffer_Shadow[i+7] == 0x01 && data_Buffer_Shadow[i+8] == 0x02)
                    {
                        UART_Flag_NO_IMAGE = data_Buffer_Shadow[i+4];
                    }
                }
                //���ն�ֵ����ֵ������ͷ00-FF-02-01�����ݳ���1�ֽڣ�����β00-FF-02-02
                if (data_Buffer_Shadow[i] == 0x00 && data_Buffer_Shadow[i+1] == 0xFF && data_Buffer_Shadow[i+2] == 0x02 && data_Buffer_Shadow[i+3] == 0x01)
                {
                    if (i<RECEIVE_LENGTH-8 && data_Buffer_Shadow[i+5] == 0x00 && data_Buffer_Shadow[i+6] == 0xFF && data_Buffer_Shadow[i+7] == 0x02 && data_Buffer_Shadow[i+8] == 0x02)
                    {
                        Set_Thresholding_Value(data_Buffer_Shadow[i+4]);
                    }
                }
                //������͸�Ӳ���������ͷ00-FF-03-01�����ݳ���4�ֽڣ�����β00-FF-03-02
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
                //�����ٶȲ���������ͷ00-FF-04-01�����ݳ���1�ֽڣ�����β00-FF-04-02
                if (data_Buffer_Shadow[i] == 0x00 && data_Buffer_Shadow[i+1] == 0xFF && data_Buffer_Shadow[i+2] == 0x04 && data_Buffer_Shadow[i+3] == 0x01)
                {
                    if (i<RECEIVE_LENGTH-8 && data_Buffer_Shadow[i+5] == 0x00 && data_Buffer_Shadow[i+6] == 0xFF && data_Buffer_Shadow[i+7] == 0x04 && data_Buffer_Shadow[i+8] == 0x02)
                    {
                        Set_Speed_Target(data_Buffer_Shadow[i+4]);
                    }
                }
                //���ն������������ͷ00-FF-05-01�����ݳ���1�ֽڣ�����β00-FF-05-02
                if (data_Buffer_Shadow[i] == 0x00 && data_Buffer_Shadow[i+1] == 0xFF && data_Buffer_Shadow[i+2] == 0x05 && data_Buffer_Shadow[i+3] == 0x01)
                {
                    if (i<RECEIVE_LENGTH-8 && data_Buffer_Shadow[i+5] == 0x00 && data_Buffer_Shadow[i+6] == 0xFF && data_Buffer_Shadow[i+7] == 0x05 && data_Buffer_Shadow[i+8] == 0x02)
                    {
                        Set_Steering_Target(data_Buffer_Shadow[i+4]);
                    }
                }


                //...
            }//��λ��������ͷ

            UART_Flag_RX = FALSE;
        }
    }

}
