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



#ifndef _SEEKFREE_7725_UART_h
#define _SEEKFREE_7725_UART_h


#include "common.h"
#include "IfxDma_cfg.h"
#include "zf_uart.h"


#define OV7725_UART_W    		160	//ֻ����80 160 240 320 ��Ⱥ͸߶����һһ��Ӧ������������Ϊ80���߶�ֻ������Ϊ60��
#define OV7725_UART_H    		120	//ֻ����60 120 180 240
	
	
//��������ͷ����
#define OV7725_UART_SIZE    	(OV7725_UART_W * OV7725_UART_H/8 )	//ͼ��ռ�ÿռ��С
#define OV7725_UART_DMA_NUM  	(OV7725_UART_SIZE )               	//DMA�ɼ�����


//--------------------------------------------------------------------------------------------------
//��������
//--------------------------------------------------------------------------------------------------
#define OV7725_COF_UART        	UART_1         		//��������ͷ��ʹ�õ��Ĵ���
#define OV7725_COF_UART_TX     	UART1_TX_P02_2
#define OV7725_COF_UART_RX     	UART1_RX_P02_3

#define OV7725_UART_VSYNC_PIN 	ERU_CH3_REQ6_P02_0  //���ж�����	��ѡ��Χ�ο�ERU_PIN_enumö�� ��������������ѡ��Ϊͬһ��ͨ���������ǹ����жϵ�ͨ��
													//���糡�ж�ѡ��ERU_CH3 ��ô�������žͲ���ѡ��ERU_CH7����Ϊ3��7���ж��ǹ��õġ�

#define OV7725_UART_DATA_PIN  	P00_0  				//����D0��������  ����D0����ΪP00_0 ��ôD1��ʹ�õ�������ΪP00_1����������
													//�����ò���P00_0��P02_0��P15_0����������Ϊ��������

#define OV7725_UART_PCLK_PIN  	ERU_CH2_REQ14_P02_1 //��������ʱ������ ��ѡ��Χ�ο�ERU_PIN_enumö�� �����볡�ж�����ѡ��Ϊͬһ��ͨ���������ǹ����жϵ�ͨ��
													//���糡�ж�ѡ��ERU_CH3 ��ô�������žͲ���ѡ��ERU_CH7����Ϊ3��7���ж��ǹ��õġ�

#define OV7725_UART_DMA_CH		IfxDma_ChannelId_5	//����ʹ�õ�DMAͨ�� 0-47��ѡ  ͨ����Խ�����ȼ�Խ��
													//�޸Ĵ˴�֮����Ҫ�޸Ķ�Ӧ��ERU�ж����ȼ�������˴�����ΪIfxDma_ChannelId_5���Ӧ��ERU�ж����ȼ�Ӧ������Ϊ5


typedef enum
{
    OV7725_INIT 	= 0x00,			//��ʼ������
	OV7725_RESERVE,					//����λ
    OV7725_CONTRAST,				//�Աȶȵ��� ʵ����ֵ�ĵ���
    OV7725_FPS,						//֡������ ֻ֧��50 75 100 112 150
    OV7725_COL,						//�ֱ������� ֻ����80 160 240 320
    OV7725_ROW,						//�ֱ������� ֻ����60 120 180 240
	OV7725_CONFIG_FINISH,			//������λ����Ҫ����ռλ����
    
	OV7725_GET_WHO_AM_I= 0xEF,
	OV7725_GET_STATUS  = 0xF1,
    OV7725_GET_VERSION = 0xF2,
	
    OV7725_SET_ADDR    = 0xFE,
    OV7725_SET_DATA    = 0xFF,
}OV7725_CMD;

extern uint8 ov7725_uart_image_bin[OV7725_UART_H][OV7725_UART_W/8];		//����洢����ͼ�������
extern uint8 ov7725_uart_image_dec[OV7725_UART_H][OV7725_UART_W];    	//ͼ���ѹ�������

extern uint8 ov7725_uart_finish_flag;									//ͼ��ɼ���ɱ�־λ


//��������
void ov7725_uart_callback(void);
void ov7725_uart_init(void);
void ov7725_uart_vsync(void);
void ov7725_uart_dma(void);
void image_decompression(uint8 *data1,uint8 *data2);
void seekfree_sendimg_7725(UARTN_enum uartn, uint8 *image, uint16 width, uint16 height);


#endif



