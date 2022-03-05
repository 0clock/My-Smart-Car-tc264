/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		�����
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
					ģ��ܽ�            			��Ƭ���ܽ�
					SDA(51��RX)         		�鿴SEEKFREE_MT9V03X.h�ļ��е�MT9V03X_COF_UART_TX�궨��
					SCL(51��TX)         		�鿴SEEKFREE_MT9V03X.h�ļ��е�MT9V03X_COF_UART_RX�궨��
					���ж�(VSY)         		�鿴SEEKFREE_MT9V03X.h�ļ��е�MT9V03X_VSYNC_PIN�궨��
					���ж�(HREF)				����û��ʹ�ã���˲�����
					�����ж�(PCLK)      		�鿴SEEKFREE_MT9V03X.h�ļ��е�MT9V03X_PCLK_PIN�궨��
					���ݿ�(D0-D7)			�鿴SEEKFREE_MT9V03X.h�ļ��е�MT9V03X_DATA_PIN�궨��
					------------------------------------ 
	
					Ĭ�Ϸֱ�����           			188*120
					Ĭ��FPS                 50֡
 ********************************************************************************************************************/


#include "IfxDma.h"
#include "IfxScuEru.h"
#include "zf_stm_systick.h"
#include "zf_gpio.h"
#include "zf_eru.h"
#include "zf_eru_dma.h"
#include "SEEKFREE_MT9V03X.h"
#include "CAMERA.h"


//����4�ֽڶ���
//IFX_ALIGN(4) uint8 mt9v03x_image[MT9V03X_H][MT9V03X_W];

uint8   link_list_num;

////��Ҫ���õ�����ͷ������
//int16 MT9V03X_CFG[CONFIG_FINISH][2]=
//{
//    {AUTO_EXP,          0},   //�Զ��ع�����      ��Χ1-63 0Ϊ�ر� ����Զ��ع⿪��  EXP_TIME�������õ����ݽ����Ϊ����ع�ʱ�䣬Ҳ�����Զ��ع�ʱ�������
//                              //һ������ǲ���Ҫ����������ܣ���Ϊ�������ع���һ�㶼�ȽϾ��ȣ�����������߷ǳ������ȵ�������Գ������ø�ֵ������ͼ���ȶ���
//    {EXP_TIME,          450}, //�ع�ʱ��          ����ͷ�յ�����Զ����������ع�ʱ�䣬������ù���������Ϊ�������������ع�ֵ
//    {FPS,               50},  //ͼ��֡��          ����ͷ�յ�����Զ���������FPS���������������Ϊ������������FPS
//    {SET_COL,           MT9V03X_W}, //ͼ��������        ��Χ1-752     K60�ɼ���������188
//    {SET_ROW,           MT9V03X_H}, //ͼ��������        ��Χ1-480
//    {LR_OFFSET,         0},   //ͼ������ƫ����    ��ֵ ��ƫ��   ��ֵ ��ƫ��  ��Ϊ188 376 752ʱ�޷�����ƫ��    ����ͷ��ƫ�����ݺ���Զ��������ƫ�ƣ�������������ü�����������ƫ��
//    {UD_OFFSET,         0},   //ͼ������ƫ����    ��ֵ ��ƫ��   ��ֵ ��ƫ��  ��Ϊ120 240 480ʱ�޷�����ƫ��    ����ͷ��ƫ�����ݺ���Զ��������ƫ�ƣ�������������ü�����������ƫ��
//    {GAIN,              32},  //ͼ������          ��Χ16-64     ����������ع�ʱ��̶�������¸ı�ͼ�������̶�
//
//
//    {INIT,              0}    //����ͷ��ʼ��ʼ��
//};
//
////������ͷ�ڲ���ȡ������������
//int16 GET_CFG[CONFIG_FINISH-1][2]=
//{
//    {AUTO_EXP,          0},   //�Զ��ع�����
//    {EXP_TIME,          0},   //�ع�ʱ��
//    {FPS,               0},   //ͼ��֡��
//    {SET_COL,           0},   //ͼ��������
//    {SET_ROW,           0},   //ͼ��������
//    {LR_OFFSET,         0},   //ͼ������ƫ����
//    {UD_OFFSET,         0},   //ͼ������ƫ����
//    {GAIN,              0},   //ͼ������
//};



//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V03X����ͷ��ʼ��
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	ʹ��FLEXIO�ӿڲɼ�����ͷ	
//-------------------------------------------------------------------------------------------------------------------
void mt9v03x_init(void)
{
	uint8 i;
    camera_type = CAMERA_GRAYSCALE;//������������ͷ����
    camera_buffer_addr = mt9v03x_image[0];

    boolean interrupt_state = disableInterrupts();


    MT9V034_Init(200);//����֡��

	//����ͷ�ɼ���ʼ��
	//��ʼ�� ��������
	for(i=0; i<8; i++)
	{
		gpio_init((PIN_enum)(MT9V03X_DATA_PIN+i), GPI, 0, PULLUP);
	}

    link_list_num = eru_dma_init(MT9V03X_DMA_CH, GET_PORT_IN_ADDR(MT9V03X_DATA_PIN), camera_buffer_addr, MT9V03X_PCLK_PIN, FALLING, MT9V03X_W*MT9V03X_H);//�����Ƶ��300M �����ڶ�������������ΪFALLING

    eru_init(MT9V03X_VSYNC_PIN, FALLING);	//��ʼ�����жϣ�������Ϊ�½��ش����ж�
    restoreInterrupts(interrupt_state);
}


uint8   mt9v03x_finish_flag = 0;    //һ��ͼ��ɼ���ɱ�־λ
uint8	mt9v03x_dma_int_num;	//��ǰDMA�жϴ���
//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V03X����ͷ���ж�
//  @param      NULL
//  @return     void			
//  @since      v1.0
//  Sample usage:				�˺�����isr.c�б�eru��GPIO�жϣ��жϵ���
//-------------------------------------------------------------------------------------------------------------------
void mt9v03x_vsync(void)
{
	CLEAR_GPIO_FLAG(MT9V03X_VSYNC_PIN);
	mt9v03x_dma_int_num = 0;
	if(!mt9v03x_finish_flag)//�鿴ͼ�������Ƿ�ʹ����ϣ����δʹ������򲻿�ʼ�ɼ���������ַ��ʳ�ͻ
	{
		if(1 == link_list_num)
		{
			//û�в������Ӵ���ģʽ ��������Ŀ�ĵ�ַ
			DMA_SET_DESTINATION(MT9V03X_DMA_CH, camera_buffer_addr);
		}
		dma_start(MT9V03X_DMA_CH);
	}

}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V03X����ͷDMA����ж�
//  @param      NULL
//  @return     void			
//  @since      v1.0
//  Sample usage:				�˺�����isr.c�б�dma�жϵ���
//-------------------------------------------------------------------------------------------------------------------
void mt9v03x_dma(void)
{
	CLEAR_DMA_FLAG(MT9V03X_DMA_CH);
	mt9v03x_dma_int_num++;

	if(mt9v03x_dma_int_num >= link_list_num)
	{
		//�ɼ����
		mt9v03x_dma_int_num = 0;
		mt9v03x_finish_flag = 1;//һ��ͼ��Ӳɼ���ʼ���ɼ�������ʱ3.8MS����(50FPS��188*120�ֱ���)
		dma_stop(MT9V03X_DMA_CH);
	}
}






//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//���������񲿷�
/*!
  * @brief    MT9V034�Ĵ�����ʼ��
  *
  * @param    fps  :֡��
  *
  * @return   ��
  *
  * @note     һ��ʹ��50֡
  *
  * @see      MT9V034_Init(50);
  *
  * @date     2019/6/12 ������
  */
void MT9V034_Init(unsigned char fps)
{
    unsigned short data = 0;

    //����ͷSCCB�ӿڳ�ʼ��
    SCCB_Init();

    if(SCCB_RegRead(MT9V034_I2C_ADDR>>1,MT9V034_CHIP_VERSION,&data) == 0)  //��ȡ����ͷ�汾�Ĵ���
    {
        if(data != MT9V034_CHIP_ID)                                        //оƬID����ȷ��˵��û����ȷ��ȡ�����ݣ�������
        {
            oled_p6x8str(0, 0, "camera id error");
#pragma warning 557         // ���ξ���
            while (1);
#pragma warning default     // �򿪾���
        }
    }
    else
    {
        oled_p6x8str(0, 0, "camera sccb error");
#pragma warning 557         // ���ξ���
        while (1);          //����ͷʶ��ʧ�ܣ�ֹͣ����
#pragma warning default     // �򿪾���
    }

    /* ��������ͷͼ��4*4��Ƶ���PCLK 27/4 = 6.75M ,BIT4,5��������:�������Ҿ����� */
    MT9V034_SetFrameResolution(MT9V03X_H, MT9V03X_W);

    /* ����֡�� */
    MT9V034_SetFrameRate(fps);

    /* �ع����� */
    MT9V034_SetAutoExposure(CAMERA_AUTO_EXPOSURE);

    SCCB_RegWrite(MT9V034_I2C_ADDR, 0x2C, 0x0004);  //�ο���ѹ����   1.4v
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_ANALOG_CTRL, MT9V034_ANTI_ECLIPSE_ENABLE);  //����ʴ
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_ADC_RES_CTRL_REG, 0x0303);      //0x1C  here is the way to regulate darkness :)
    ////
    SCCB_RegWrite(MT9V034_I2C_ADDR,0x13,0x2D2E);//We also recommended using R0x13 = 0x2D2E with this setting for better column FPN.
    SCCB_RegWrite(MT9V034_I2C_ADDR,0x20,CAMERA_CONTRAST);//0x01C7�ԱȶȲ���ף�0x03C7�Աȶ���� Recommended by design to improve performance in HDR mode and when frame rate is low.
    SCCB_RegWrite(MT9V034_I2C_ADDR,0x24,0x0010);//Corrects pixel negative dark offset when global reset in R0x20[9] is enabled.
    SCCB_RegWrite(MT9V034_I2C_ADDR,0x2B,0x0003);//Improves column FPN.
    SCCB_RegWrite(MT9V034_I2C_ADDR,0x2F,0x0003);//Improves FPN at near-saturation.

    SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_V2_CTRL_REG_A, 0x001A);        //0x32   0x001A
    SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_HDR_ENABLE_REG,0x0103);        //0x0F High Dynamic Range enable,bit is set (R0x0F[1]=1), the sensor uses black level correction values from one green plane, which are applied to all colors.



    /* 0xA5  ͼ������  60  1-64  */
    SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_AGC_AEC_DESIRED_BIN_REG, CAMERA_AUTO_EXPOSURE_BRIGHTNESS);

}

/*!
  * @brief    ֡������
  *
  * @param    fps : ֡��
  *
  * @return
  *
  * @note     ֡����ʵ��ͨ�����ӿհ���ʵ�ֵģ� ������ʵ���֡����������֮��Ĺ�ϵ
  * @note     ֡��        200    150    100    70.9    50    20    10
  * @note     ������    39     92     199    330     515   1450  3000
  *
  * @see      MT9V034_SetFrameRate(50);
  *
  * @date     2019/7/26 ������
  */
void MT9V034_SetFrameRate(unsigned char fps)
{
    float vertical = 0;
    if(fps > 200)
    {
        vertical = 2;
    }
    else if(fps >= 150)
    {
        vertical = -1.06 * fps + 251;
    }
    else if(fps > 100)
    {
        vertical = -2.14 * fps + 413;
    }
    else if(fps >= 71)
    {
        vertical = -4.5017 * fps + 649.17;
    }
    else if(fps > 50)
    {
        vertical = -8.8517 * fps + 957.58;
    }
    else if(fps > 20)
    {
        vertical = -31.167 * fps + 2073.3;
    }
    else
    {
        vertical = -155 * fps + 4550;
    }
    /* ������ ����֡��ʱ�� 2�C32288 */
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_VERTICAL_BLANKING, (unsigned short)vertical);
}

/*!
  * @brief    ����ͼ�񴰿ڴ�С
  *
  * @param    fps : ֡��
  *
  * @return   ��
  *
  * @note     һ��ʹ��Ĭ�ϼ���
  *
  * @see      MT9V034_SetFrameResolution(MT9V034_MT9V03X_H, MT9V034_MT9V03X_W);
  *
  * @date     2019/7/26 ������
  */
static void MT9V034_SetFrameResolution(unsigned short height,unsigned short width)
{
    unsigned short data = 0;

    if((height*4) <= MAX_IMAGE_HEIGHT)   //�ж����Ƿ�4��Ƶ
    {
        height *= 4;
        data |= MT9V034_READ_MODE_ROW_BIN_4;
    }
    else if((height*2)<=MAX_IMAGE_HEIGHT )  //�ж����Ƿ�2��Ƶ
    {
        height *= 2;
        data |= MT9V034_READ_MODE_ROW_BIN_2;
    }

    if((width*4)<=MAX_IMAGE_WIDTH )   //�ж����Ƿ�4��Ƶ
    {
        width *= 4;
        data |= MT9V034_READ_MODE_COL_BIN_4;
    }
    else if((width*2)<=MAX_IMAGE_WIDTH )   //�ж����Ƿ�2��Ƶ
    {
        width *= 2;
        data |= MT9V034_READ_MODE_COL_BIN_2;
    }

    //         ˮƽ��ת                     ��ֱ��ת
    data |= (MT9V034_READ_MODE_ROW_FLIP|MT9V034_READ_MODE_COLUMN_FLIP);  //��Ҫ��ת�Ŀ��Դ�ע��
    MTV_IICWriteReg16(MT9V034_READ_MODE, data);       //д�Ĵ����������з�Ƶ

    MTV_IICWriteReg16(MT9V034_WINDOW_WIDTH,  width);  //��ȡͼ�������  �ı�˴�Ҳ�ɸı�ͼ�������С�������ᶪʧ�ӽ�
    MTV_IICWriteReg16(MT9V034_WINDOW_HEIGHT, height); //��ȡͼ�������  �ı�˴�Ҳ�ɸı�ͼ�������С�������ᶪʧ�ӽ�

    MTV_IICWriteReg16(MT9V034_COLUMN_START, MT9V034_COLUMN_START_MIN);  //�п�ʼ
    MTV_IICWriteReg16(MT9V034_ROW_START, MT9V034_ROW_START_MIN);        //�п�ʼ

}

/*!
  * @brief    �����Զ��ع�ʱ��
  *
  * @param    enable �� 1 ʹ���Զ��ع�  �Զ��ع�ʱ��ͨ������ĺ궨���޸�
  *
  * @return   ��
  *
  * @note     ��
  *
  * @see      MT9V034_SetAutoExposure(CAMERA_AUTO_EXPOSURE);
  *
  * @date     2019/7/26 ������
  */
void MT9V034_SetAutoExposure(char enable)
{
    unsigned short reg =0;
    SCCB_RegRead(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE,&reg);
    if(enable)
    {
        /* �����Զ��ع��Զ����� */
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE, reg|MT9V034_AEC_ENABLE|MT9V034_AGC_ENABLE);
        /* ����ع�ʱ�� �޸���������޸ıȽϰ�ʱ��ͼ����������*/
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_EXPOSURE_REG, CAMERA_MAX_EXPOSURE_TIME);
        /* ��С�ع�ʱ�� �޸���������޸�����ǿ��ʱ��ͼ����������*/
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MIN_EXPOSURE_REG, CAMERA_MIN_EXPOSURE_TIME);
        /* ������� �������� ͼ��ƫ��������±��������ϸ�� ���ǿ��ܲ������ 0-60*/
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_GAIN_REG, 20);
        /* 0xB0  ����AEC/AGCֱ��ͼ������Ŀ,22560 ���44000  */
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AGC_AEC_PIXEL_COUNT_REG, 22560);

    }
    else
    {
        /* �ر��Զ��ع� */
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE, reg&~(MT9V034_AEC_ENABLE|MT9V034_AGC_ENABLE));

        /* 0xAB  ���ģ������     64 */
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_GAIN_REG, 30);

        /* 0x0B �����ع�ʱ�� 0�C32765 */
        SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_TOTAL_SHUTTER_WIDTH,CAMERA_EXPOSURE_TIME);

    }

}






/*********************************************************************
 *����ͷSCCB�ײ�����
 *
 ***********************************************************************/


/*!
  * @brief    SCCBд16λ�Ĵ���
  *
  * @param    reg �� �Ĵ���
  * @param    val �� д���ֵ
  *
  * @return   ��
  *
  * @note     ��
  *
  * @see      MTV_IICWriteReg16(0x20, 0x0003); //��MT9V034��0x20�Ĵ���д��0x0003
  *
  * @date     2019/7/26 ������
  */
void MTV_IICWriteReg16(unsigned char reg, unsigned short val)
{
    SCCB_RegWrite(MT9V034_I2C_ADDR, reg, val);
}


/*!
  * @brief    SCCB ��ʼ��
  *
  * @param    ��
  *
  * @return   ��
  *
  * @note     SCCB IO��LQ_MT9V034.h��ͨ���궨��ѡ��
  *
  * @see      SCCB_Init(); //��ʼ��SCCB  IO
  *
  * @date     2019/7/26 ������
  */
void SCCB_Init(void)
{
    gpio_init(MT9V034_SCL_PIN, GPO, 1, PULLDOWN);
    gpio_init(MT9V034_SDA_PIN, GPO, 1, PULLDOWN);
    gpio_pin_driver(MT9V034_SCL_PIN, IfxPort_PadDriver_cmosAutomotiveSpeed1);
    gpio_pin_driver(MT9V034_SDA_PIN, IfxPort_PadDriver_cmosAutomotiveSpeed1);
    MT9V034_SCL_High;
    MT9V034_SDA_High;
}



/*!
  * @brief    SCCB ��ʱ
  *
  * @param    ��
  *
  * @return   ��
  *
  * @note     ��
  *
  * @see      SCCB_Wait();
  *
  * @date     2019/7/26 ������
  */
void SCCB_Wait(void)
{
    systick_delay_us(STM0, 500);
//  int i;
//  for (i = 0; i < 500; ++i)
//  {
//      __asm("NOP"); /* delay */
//  }
}

/*!
  * @brief    SCCB ��ʼʱ��
  *
  * @param    ��
  *
  * @return   ��
  *
  * @note     ��
  *
  * @see      SCCB_Star();
  *
  * @date     2019/7/26 ������
  */
void SCCB_Star(void)
{
    MT9V034_SCL_Out;
    MT9V034_SDA_Out;
    SCCB_Wait();
    MT9V034_SDA_High;
    MT9V034_SCL_High;
    SCCB_Wait();
    MT9V034_SDA_Low;
    SCCB_Wait();
    MT9V034_SCL_Low;
    SCCB_Wait();
}

/*!
  * @brief    SCCB ֹͣʱ��
  *
  * @param    ��
  *
  * @return   ��
  *
  * @note     ��
  *
  * @see      SCCB_Star();
  *
  * @date     2019/7/26 ������
  */
void SCCB_Stop(void)
{
    MT9V034_SCL_Out;
    MT9V034_SDA_Out;
    SCCB_Wait();
    MT9V034_SDA_Low;
    SCCB_Wait();
    MT9V034_SCL_High;
    SCCB_Wait();
    MT9V034_SDA_High;
    SCCB_Wait();
}

/*!
  * @brief    SCCB ����һ���ֽ�
  *
  * @param    Data  �������ֽ�
  *
  * @return   ack Ӧ���ź�
  *
  * @note     ��
  *
  * @see      SCCB_SendByte(0x01);
  *
  * @date     2019/7/26 ������
  */
unsigned char SCCB_SendByte(unsigned char Data)
{
  unsigned char i;
  unsigned char Ack;
  MT9V034_SDA_Out;
  for( i=0; i<8; i++)
  {
    if(Data & 0x80)
    {
        MT9V034_SDA_High;
    }
    else
    {
        MT9V034_SDA_Low;
    }
    Data <<= 1;
    SCCB_Wait();
    MT9V034_SCL_High;
    SCCB_Wait();
    MT9V034_SCL_Low;
    SCCB_Wait();
  }
  MT9V034_SDA_High;
  MT9V034_SDA_In;
  SCCB_Wait();

  MT9V034_SCL_High;
  SCCB_Wait();
  Ack = MT9V034_SDA_Data;
  MT9V034_SCL_Low;
  SCCB_Wait();
  return Ack;
}

/*!
  * @brief    SCCB ����һ���ֽ�
  *
  * @param    ��
  *
  * @return   ���յ����ֽ�
  *
  * @note     ��
  *
  * @see      unsigned char data = SCCB_ReadByte();
  *
  * @date     2019/7/26 ������
  */
unsigned char SCCB_ReadByte(void)
{
  unsigned char i;
  volatile unsigned char byte;
  MT9V034_SCL_Out;
  MT9V034_SDA_In; //ʹ������
  for( i=0; i<8; i++)
  {
    MT9V034_SCL_Low;
    SCCB_Wait();
    MT9V034_SCL_High;
    SCCB_Wait();
    byte = (byte<<1)|(MT9V034_SDA_Data & 1);
  }
  MT9V034_SCL_Low;
  MT9V034_SDA_Out;
  SCCB_Wait();
  return byte;
}

/*!
  * @brief    SCCB Ӧ��
  *
  * @param    ��
  *
  * @return   ��
  *
  * @note     ��
  *
  * @see      SCCB_Ack();
  *
  * @date     2019/7/26 ������
  */
static void SCCB_Ack(void)
{
    MT9V034_SCL_Out;
    MT9V034_SDA_Out;
    MT9V034_SCL_Low;
    MT9V034_SDA_Low;
    SCCB_Wait();
    MT9V034_SCL_High;
    SCCB_Wait();
    MT9V034_SCL_Low;
    SCCB_Wait();
}

/*!
  * @brief    SCCB ��Ӧ��
  *
  * @param    ��
  *
  * @return   ��
  *
  * @note     ��
  *
  * @see      SCCB_NAck();
  *
  * @date     2019/7/26 ������
  */
static void SCCB_NAck(void)
{
    MT9V034_SCL_Out;
    MT9V034_SDA_Out;
    MT9V034_SCL_Low;
    SCCB_Wait();
    MT9V034_SDA_High;
    SCCB_Wait();
    MT9V034_SCL_High;
    SCCB_Wait();
    MT9V034_SCL_Low;
    SCCB_Wait();
}

/*!
  * @brief    SCCB д�Ĵ���
  *
  * @param    Device  : ��ַ
  * @param    reg     : �Ĵ���
  * @param    Data    : ����
  *
  * @return   ack
  *
  * @note     ��
  *
  * @see      SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_GAIN_REG, 30);
  *
  * @date     2019/7/26 ������
  */
void SCCB_RegWrite(unsigned char Device,unsigned char reg,unsigned short Data)
{
  unsigned char i;
  unsigned char Ack;

  for( i=0; i<3; i++)
  {
    SCCB_Star();
    Ack = SCCB_SendByte(Device);
    if( Ack == 1 )
    {
      continue;
    }

    Ack = SCCB_SendByte(reg);
    if( Ack == 1 )
    {
      continue;
    }

    SCCB_SendByte((unsigned char)(Data>>8));
    Ack = SCCB_SendByte((unsigned char)Data);
    if( Ack == 1 )
    {
      continue;
    }

    SCCB_Stop();
    if( Ack == 0 ) break;
  }
}

/*!
  * @brief    SCCB ���Ĵ���
  *
  * @param    Device  : ��ַ
  * @param    reg     : �Ĵ���
  * @param    Data    : ��������
  *
  * @return   ack
  *
  * @note     ��
  *
  * @see      SCCB_RegRead(MT9V034_I2C_ADDR>>1,MT9V034_CHIP_VERSION,&data)
  *
  * @date     2019/7/26 ������
  */
unsigned char SCCB_RegRead(unsigned char Device,unsigned char reg,unsigned short *Data)
{
  unsigned char Ack = 0;
  Device = Device<<1;
  SCCB_Star();
  Ack += SCCB_SendByte(Device);

  Ack += SCCB_SendByte(reg);

  SCCB_Star();
  Ack += SCCB_SendByte(Device + 1);

  *Data = SCCB_ReadByte();
  SCCB_Ack();
  *Data = *Data<<8;

  *Data += SCCB_ReadByte();
  SCCB_NAck();

  SCCB_Stop();

  return  Ack;
}






