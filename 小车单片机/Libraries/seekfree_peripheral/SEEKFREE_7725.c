/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		main
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		ADS v1.2.2
 * @Target core		TC264D
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-3-23
 * @note		
					���߶��壺
					------------------------------------ 
					ģ��ܽ�           			��Ƭ���ܽ�
					SDA   				�鿴SEEKFREE_IIC�ļ��ڵ�SEEKFREE_SDA�궨��
					SCL         	           �鿴SEEKFREE_IIC�ļ��ڵ�SEEKFREE_SCL�궨��
					���ж�(VSY)         	�鿴SEEKFREE_OV7725.h�ļ��е�OV7725_VSYNC_PIN�궨��
					���ж�(HREF)			δʹ�ã���˲���
					�����ж�(PCLK)      	�鿴SEEKFREE_OV7725.h�ļ��е�OV7725_PCLK_PIN�궨��
					���ݿ�(D0-D7)		�鿴SEEKFREE_OV7725.h�ļ��е�OV7725_DATA_PIN�궨��
					------------------------------------ 
	
					Ĭ�Ϸֱ�����            		160*120
					Ĭ��FPS            	50֡
 ********************************************************************************************************************/


#include "IfxDma.h"
#include "IfxScuEru.h"
#include "zf_stm_systick.h"
#include "zf_gpio.h"
#include "zf_eru.h"
#include "zf_eru_dma.h"
#include "SEEKFREE_IIC.h"
#include "SEEKFREE_7725.h"



IFX_ALIGN(4) uint8 image_bin[OV7725_H][OV7725_W/8];                      //����洢����ͼ�������
uint8 image_dec[OV7725_H][OV7725_W];

uint8 ov7725_idcode = 0;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      С�������ͷ�ڲ��Ĵ�����ʼ��(�ڲ�ʹ�ã��û��������)
//  @param      NULL
//  @return     uint8			����0���������1��ɹ�
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint8 ov7725_reg_init(void)
{
    simiic_delay_set(700);
    simiic_write_reg ( OV7725_DEV_ADD, OV7725_COM7, 0x80 );	//��λ����ͷ
    systick_delay_ms(STM0, 50);
	ov7725_idcode = simiic_read_reg( OV7725_DEV_ADD, OV7725_VER ,SCCB);
    if( ov7725_idcode != OV7725_ID )    return 0;			//У������ͷID��

    //ID��ȷ������   Ȼ�����üĴ���
    simiic_write_reg(OV7725_DEV_ADD, OV7725_COM4         , 0xC1);  
    simiic_write_reg(OV7725_DEV_ADD, OV7725_CLKRC        , 0x01);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_COM2         , 0x03);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_COM3         , 0xD0);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_COM7         , 0x40);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_COM8         , 0xCE);   //0xCE:�ر��Զ��ع�  0xCF�������Զ��ع�
    simiic_write_reg(OV7725_DEV_ADD, OV7725_HSTART       , 0x3F);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_HSIZE        , 0x50);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_VSTRT        , 0x03);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_VSIZE        , 0x78);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_HREF         , 0x00);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_SCAL0        , 0x0A);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_AWB_Ctrl0    , 0xE0);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_DSPAuto      , 0xff);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_DSP_Ctrl2    , 0x0C);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_DSP_Ctrl3    , 0x00);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_DSP_Ctrl4    , 0x00);
    
    if(OV7725_W == 80)              simiic_write_reg(OV7725_DEV_ADD, OV7725_HOutSize    , 0x14);
    else if(OV7725_W == 160)        simiic_write_reg(OV7725_DEV_ADD, OV7725_HOutSize    , 0x28);
    else if(OV7725_W == 240)        simiic_write_reg(OV7725_DEV_ADD, OV7725_HOutSize    , 0x3c);
    else if(OV7725_W == 320)        simiic_write_reg(OV7725_DEV_ADD, OV7725_HOutSize    , 0x50);
    
    if(OV7725_H == 60)              simiic_write_reg(OV7725_DEV_ADD, OV7725_VOutSize    , 0x1E);
    else if(OV7725_H == 120)        simiic_write_reg(OV7725_DEV_ADD, OV7725_VOutSize    , 0x3c);
    else if(OV7725_H == 180)        simiic_write_reg(OV7725_DEV_ADD, OV7725_VOutSize    , 0x5a);
    else if(OV7725_H == 240)        simiic_write_reg(OV7725_DEV_ADD, OV7725_VOutSize    , 0x78);
    
    simiic_write_reg(OV7725_DEV_ADD, OV7725_REG28        , 0x01);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_EXHCH        , 0x10);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_EXHCL        , 0x1F);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM1         , 0x0c);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM2         , 0x16);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM3         , 0x2a);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM4         , 0x4e);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM5         , 0x61);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM6         , 0x6f);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM7         , 0x7b);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM8         , 0x86);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM9         , 0x8e);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM10        , 0x97);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM11        , 0xa4);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM12        , 0xaf);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM13        , 0xc5);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM14        , 0xd7);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_GAM15        , 0xe8);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_SLOP         , 0x20);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_LC_RADI      , 0x00);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_LC_COEF      , 0x13);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_LC_XC        , 0x08);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_LC_COEFB     , 0x14);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_LC_COEFR     , 0x17);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_LC_CTR       , 0x05);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_BDBase       , 0x99);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_BDMStep      , 0x03);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_SDE          , 0x04);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_BRIGHT       , 0x00);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_CNST         , 0x40);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_SIGN         , 0x06);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_UVADJ0       , 0x11);
    simiic_write_reg(OV7725_DEV_ADD, OV7725_UVADJ1       , 0x02);


    simiic_delay_set(SIMIIC_DELAY_TIME);//�޸�ΪĬ�ϵ�20
    return 1;


}





//-------------------------------------------------------------------------------------------------------------------
//  @brief      С�������ͷ�ɼ������ʼ��(�ڲ�ʹ�ã��û��������)
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void ov7725_port_init(void)
{
	uint8 i;
    camera_type = CAMERA_BIN;//������������ͷ����
    camera_buffer_addr = image_bin[0];
    
	//����ͷ�ɼ���ʼ��
	//��ʼ�� ��������
	for(i=0; i<8; i++)
	{
		gpio_init((PIN_enum)(OV7725_DATA_PIN+i), GPI, 0, PULLUP);
	}
	eru_dma_init(OV7725_DMA_CH, GET_PORT_IN_ADDR(OV7725_DATA_PIN), camera_buffer_addr, OV7725_PCLK_PIN, FALLING, OV7725_DMA_NUM);
    eru_init(OV7725_VSYNC_PIN, FALLING);	//��ʼ�����жϣ�������Ϊ�½��ش����ж�
    
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      С�������ͷ��ʼ��(����֮�����ú�����жϺ������ɲɼ�ͼ��)
//  @param      NULL
//  @return     0
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint8 ov7725_init(void)
{
	simiic_init();
	ov7725_reg_init();                                          //����ͷ�Ĵ�������
    ov7725_port_init();                                         //����ͷ�ж����ż�DMA����
    return 0;
}




uint8   ov7725_finish_flag = 0;
//-------------------------------------------------------------------------------------------------------------------
//  @brief      С�������ͷ���ж�
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:					�˺�����isr.c�б�eru��GPIO�жϣ��жϵ���
//-------------------------------------------------------------------------------------------------------------------
void ov7725_vsync(void)
{
	CLEAR_GPIO_FLAG(OV7725_VSYNC_PIN);

	if(!ov7725_finish_flag)//�鿴ͼ�������Ƿ�ʹ����ϣ����δʹ������򲻿�ʼ�ɼ���������ַ��ʳ�ͻ
	{
		DMA_SET_DESTINATION(OV7725_DMA_CH, camera_buffer_addr);
		dma_start(OV7725_DMA_CH);
	}
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      С�������ͷDMA����ж�
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:					�˺�����isr.c�б�dma�жϵ���
//-------------------------------------------------------------------------------------------------------------------
void ov7725_dma(void)
{
    CLEAR_DMA_FLAG(OV7725_DMA_CH);
	ov7725_finish_flag = 1;
	dma_stop(OV7725_DMA_CH);
}



