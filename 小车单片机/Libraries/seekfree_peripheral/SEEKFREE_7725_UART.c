/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file            7725_UART
 * @company         �ɶ���ɿƼ����޹�˾
 * @author          ��ɿƼ�(QQ3184284598)
 * @version         �鿴doc��version�ļ� �汾˵��
 * @Software        ADS v1.2.2
 * @Target core     TC264D
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-3-23
 * @note
					OV7725���߶��壺
                    ------------------------------------
                                        ģ��ܽ�                     			��Ƭ���ܽ�
                    SDA(51��RX)             		�鿴SEEKFREE_7725_UART.h�ļ��ڵ�OV7725_COF_UART_TX �궨��
                    SCL(51��TX)       			�鿴SEEKFREE_7725_UART.h�ļ��ڵ�OV7725_COF_UART_RX �궨��
                                        ���ж�(VSY)           		�鿴SEEKFREE_7725_UART.h�ļ��ڵ�OV7725_UART_VSYNC_PIN �궨��
                                        ���ж�(HREF)          		δʹ�ã���˲���
                                        �����ж�(PCLK)   			�鿴SEEKFREE_7725_UART.h�ļ��ڵ�OV7725_UART_PCLK_PIN �궨��
                                        ���ݿ�(D0-D7)         		�鿴SEEKFREE_7725_UART.h�ļ��ڵ�OV7725_UART_DATA_PIN �궨��
                                        Ĭ�Ϸֱ���              			 	160*120
                  	Ĭ��FPS                  	50֡
                    ------------------------------------
 ********************************************************************************************************************/


#include "IfxDma.h"
#include "IfxScuEru.h"
#include "zf_stm_systick.h"
#include "zf_gpio.h"
#include "zf_eru.h"
#include "zf_eru_dma.h"
#include "SEEKFREE_7725_UART.h"

IFX_ALIGN(4) uint8 ov7725_uart_image_bin[OV7725_UART_H][OV7725_UART_W/8];	//����洢����ͼ�������
uint8 ov7725_uart_image_dec[OV7725_UART_H][OV7725_UART_W];

//��Ҫ���õ�����ͷ������
uint16 OV7725_CFG[OV7725_CONFIG_FINISH][2]=
{
	{OV7725_RESERVE,           0},
    {OV7725_CONTRAST,       0x30},    	//�Աȶ�0x00-0xff(�ı����ֵ ���ʵ����ֵ�޸ĵ�Ч��)
    {OV7725_FPS,              50},    	//֡������ ֻ֧��50 75 100 112 150
    {OV7725_COL,   OV7725_UART_W},    	//ֻ����80 160 240 320 ��Ⱥ͸߶����һһ��Ӧ������������Ϊ80���߶�ֻ������Ϊ60��
	{OV7725_ROW,   OV7725_UART_H},    	//ֻ����60 120 180 240
	
    {OV7725_INIT,              0}     	//����ͷ��ʼ��ʼ��
};


//������ͷ�ڲ���ȡ������������
uint16 OV7725_GET_CFG[OV7725_CONFIG_FINISH-1][2]= 
{
	{OV7725_RESERVE,    	   0},
    {OV7725_CONTRAST,   	   0}, 		
    {OV7725_FPS,        	   0}, 		
    {OV7725_COL,   			   0},    	
	{OV7725_ROW,   			   0}    	
};


uint8   ov7725_uart_receive[3];
uint8   ov7725_uart_receive_num = 0;
uint8   ov7725_uart_receive_flag = 0;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      С�������ͷ�����жϺ���
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       �ú�����ISR�ļ� ����3�жϳ��򱻵���
//-------------------------------------------------------------------------------------------------------------------
void ov7725_uart_callback(void)
{
    uart_getchar(OV7725_COF_UART, &ov7725_uart_receive[ov7725_uart_receive_num]);;
    ov7725_uart_receive_num++;
    
    if(1==ov7725_uart_receive_num && 0XA5!=ov7725_uart_receive[0])	ov7725_uart_receive_num = 0;
    if(3 == ov7725_uart_receive_num)
    {
        ov7725_uart_receive_num = 0;
        ov7725_uart_receive_flag = 1;
    }
        
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      С��������ͷ�ڲ��Ĵ�������д����
//  @param      UARTN_enum   ����ͷ����
//  @param      buff    ��Ҫд�������
//  @return     void  
//  @since      v1.0
//  Sample usage:		���øú���ǰ���ȳ�ʼ������
//-------------------------------------------------------------------------------------------------------------------
void ov7725_set_all_config(UARTN_enum uartn, uint16 buff[OV7725_CONFIG_FINISH-1][2])
{
    uint16 temp, i;
    uint8  send_buffer[4];

    ov7725_uart_receive_flag = 0;
    
    //���ò���  ������ο���������ֲ�
    //��ʼ��������ͷ�����³�ʼ��
    for(i=0; i<OV7725_CONFIG_FINISH; i++)
    {
        send_buffer[0] = 0xA5;
        send_buffer[1] = (uint8)buff[i][0];
        temp           = buff[i][1];
        send_buffer[2] = temp>>8;
        send_buffer[3] = (uint8)temp;
        
        uart_putbuff(uartn, send_buffer,4);
        systick_delay_ms(STM0, 2);
    }
    //�ȴ�����ͷ��ʼ���ɹ�
    while(!ov7725_uart_receive_flag);
    ov7725_uart_receive_flag = 0;
    while((0xff != ov7725_uart_receive[1]) || (0xff != ov7725_uart_receive[2]));
    //���ϲ��ֶ�����ͷ���õ�����ȫ�����ᱣ��������ͷ��51��Ƭ����eeprom��
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      С����ȡ����ͷ�ڲ�������Ϣ
//  @param      UARTN_enum   ����ͷ����
//  @param      buff    ��Ҫ��ȡ������
//  @return     void
//  @since      v1.0
//  Sample usage:		���øú���ǰ���ȳ�ʼ������
//-------------------------------------------------------------------------------------------------------------------
void ov7725_get_all_config(UARTN_enum uartn, uint16 buff[OV7725_CONFIG_FINISH-1][2])
{
    uint16 temp, i;
    uint8  send_buffer[4];
    
    for(i=0; i<OV7725_CONFIG_FINISH-1; i++)
    {
        send_buffer[0] = 0xA5;
        send_buffer[1] = OV7725_GET_STATUS;
        temp           = buff[i][0];
        send_buffer[2] = temp>>8;
        send_buffer[3] = (uint8)temp;
        
        uart_putbuff(uartn, send_buffer,4);
        
        //�ȴ����ܻش�����
        while(!ov7725_uart_receive_flag);
        ov7725_uart_receive_flag = 0;
        
        buff[i][1] = ov7725_uart_receive[1]<<8 | ov7725_uart_receive[2];
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      С����ȡ����ͷ�̼��汾
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:		���øú���ǰ���ȳ�ʼ������
//-------------------------------------------------------------------------------------------------------------------
uint16 ov7725_get_id(UARTN_enum uartn)
{
	uint16 temp;
    uint8  send_buffer[4];
	
    send_buffer[0] = 0xA5;
    send_buffer[1] = OV7725_GET_WHO_AM_I;
    temp           = 0;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8)temp;
    
    uart_putbuff(uartn, send_buffer,4);
        
    //�ȴ����ܻش�����
    while(!ov7725_uart_receive_flag);
    ov7725_uart_receive_flag = 0;
    
    return ((uint16)(ov7725_uart_receive[1]<<8) | ov7725_uart_receive[2]);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      С����ȡ����ͷ�̼��汾
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				���øú���ǰ���ȳ�ʼ��uart3
//-------------------------------------------------------------------------------------------------------------------
uint16 ov7725_get_version(UARTN_enum uartn)
{
    uint16 temp;
    uint8  send_buffer[4];
    send_buffer[0] = 0xA5;
    send_buffer[1] = OV7725_GET_STATUS;
    temp           = OV7725_GET_VERSION;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8)temp;
    
    uart_putbuff(uartn, send_buffer,4);
        
    //�ȴ����ܻش�����
    while(!ov7725_uart_receive_flag);
    ov7725_uart_receive_flag = 0;
    
    return ((uint16)(ov7725_uart_receive[1]<<8) | ov7725_uart_receive[2]);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      С�������ͷ��ʼ��(����֮�����ú�����жϺ������ɲɼ�ͼ��)
//  @param      NULL
//  @return     0
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void ov7725_uart_init(void)
{
	uint8 i;

	camera_type = CAMERA_BIN_UART;//������������ͷ����
	camera_buffer_addr = ov7725_uart_image_bin[0];

	boolean interrupt_state = disableInterrupts();

	uart_init (OV7725_COF_UART, 9600, OV7725_COF_UART_TX, OV7725_COF_UART_RX);	//��ʼ������ ��������ͷ
	enableInterrupts();//�����ж�

	ov7725_uart_receive_flag = 0;
	//�ȴ�����ͷ�ϵ��ʼ���ɹ� ��ʽ�����֣���ʱ����ͨ����ȡ���õķ�ʽ ��ѡһ
    //systick_delay_ms(STM0, 1000);//��ʱ��ʽ
    ov7725_get_all_config(OV7725_COF_UART, OV7725_GET_CFG);//��ȡ���õķ�ʽ

	ov7725_uart_receive_flag = 0;
    ov7725_set_all_config(OV7725_COF_UART, OV7725_CFG);

	//��ȡ���ñ��ڲ鿴�����Ƿ���ȷ
    ov7725_get_all_config(OV7725_COF_UART, OV7725_GET_CFG);
	disableInterrupts();

    //����ͷ�ɼ���ʼ��
	//��ʼ�� ��������
	for(i=0; i<8; i++)
	{
		gpio_init((PIN_enum)(OV7725_UART_DATA_PIN+i), GPI, 0, PULLUP);
	}

	eru_dma_init(OV7725_UART_DMA_CH, GET_PORT_IN_ADDR(OV7725_UART_DATA_PIN), camera_buffer_addr, OV7725_UART_PCLK_PIN, FALLING, OV7725_UART_DMA_NUM);
	eru_init(OV7725_UART_VSYNC_PIN, FALLING);	//��ʼ�����жϣ�������Ϊ�½��ش����ж�

	restoreInterrupts(interrupt_state);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      С�������ͷ���ж�
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:					��isr.c�����ȴ�����Ӧ���жϺ�����Ȼ����øú���(֮�����������жϱ�־λ)
//-------------------------------------------------------------------------------------------------------------------
void ov7725_uart_vsync(void) 
{
	CLEAR_GPIO_FLAG(OV7725_UART_VSYNC_PIN);

	if(!ov7725_uart_finish_flag)//�鿴ͼ�������Ƿ�ʹ����ϣ����δʹ������򲻿�ʼ�ɼ���������ַ��ʳ�ͻ
	{
		DMA_SET_DESTINATION(OV7725_UART_DMA_CH, camera_buffer_addr);
		dma_start(OV7725_UART_DMA_CH);
	}
}


uint8 ov7725_uart_finish_flag = 0;
//-------------------------------------------------------------------------------------------------------------------
//  @brief      С�������ͷDMA����ж�
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:					��isr.c�����ȴ�����Ӧ���жϺ�����Ȼ����øú���(֮�����������жϱ�־λ)
//-------------------------------------------------------------------------------------------------------------------
void ov7725_uart_dma(void)
{
    CLEAR_DMA_FLAG(OV7725_UART_DMA_CH);
	ov7725_uart_finish_flag = 1;
	dma_stop(OV7725_UART_DMA_CH);
}       


//-------------------------------------------------------------------------------------------------------------------
//  @brief      С�������ͷ���ݽ�ѹ����
//  @param      *data1				Դ��ַ
//  @param      *data2				Ŀ�ĵ�ַ
//  @return     void
//  @since      v1.0
//  Sample usage:					Image_Decompression(da1,dat2[0]);//��һά����dat1�����ݽ�ѹ����ά����dat2��.
//-------------------------------------------------------------------------------------------------------------------
void image_decompression(uint8 *data1,uint8 *data2)
{
    uint8  temp[2] = {0,255};
    uint16 lenth = OV7725_UART_SIZE;
    uint8  i = 8;


    while(lenth--)
    {
        i = 8;
        while(i--)
        {
            *data2++ = temp[(*data1 >> i) & 0x01];
        }
        data1++;
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      С�������ͷδ��ѹͼ��������λ���鿴ͼ��
//  @param      uartn				ʹ�õĴ��ں�
//  @param      image				��Ҫ���͵�ͼ���ַ
//  @param      width				ͼ�����
//  @param      height				ͼ�����
//  @return     void
//  @since      v1.0
//  Sample usage:					���øú���ǰ���ȳ�ʼ��uart2
//-------------------------------------------------------------------------------------------------------------------
void seekfree_sendimg_7725(UARTN_enum uartn, uint8 *image, uint16 width, uint16 height)
{
    uart_putchar(uartn, 0x00);uart_putchar(uartn, 0xff);uart_putchar(uartn, 0x01);uart_putchar(uartn, 0x01);//�����ĸ��ֽ�����
    uart_putbuff(uartn, image, width*height/8);   //�ٷ���ͼ��
}

