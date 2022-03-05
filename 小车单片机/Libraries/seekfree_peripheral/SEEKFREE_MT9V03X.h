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



#ifndef _SEEKFREE_MT9V03X_h
#define _SEEKFREE_MT9V03X_h

#include "common.h"
#include "IfxDma_cfg.h"
#include "zf_uart.h"
#include "SEEKFREE_OLED.h"

//��������ͷ����
#define MT9V03X_W               188             	//ͼ���� 	��Χ1-188
#define MT9V03X_H               120             	//ͼ��߶�	��Χ1-120



//--------------------------------------------------------------------------------------------------
//��������
//--------------------------------------------------------------------------------------------------
#define MT9V03X_VSYNC_PIN       ERU_CH7_REQ16_P15_1  //���ж�����	��ѡ��Χ�ο�ERU_PIN_enumö�� ��������������ѡ��Ϊͬһ��ͨ���������ǹ����жϵ�ͨ��
													//���糡�ж�ѡ��ERU_CH3 ��ô�������žͲ���ѡ��ERU_CH7����Ϊ3��7���ж��ǹ��õġ�

#define MT9V03X_DATA_PIN        P02_0  				//����D0��������  ����D0����ΪP00_0 ��ôD1��ʹ�õ�������ΪP00_1����������
													//�����ò���P00_0��P02_0��P15_0����������Ϊ��������

#define MT9V03X_PCLK_PIN        ERU_CH2_REQ7_P00_4 //��������ʱ������ ��ѡ��Χ�ο�ERU_PIN_enumö�� �����볡�ж�����ѡ��Ϊͬһ��ͨ���������ǹ����жϵ�ͨ��
													//���糡�ж�ѡ��ERU_CH3 ��ô�������žͲ���ѡ��ERU_CH7����Ϊ3��7���ж��ǹ��õġ�

#define MT9V03X_DMA_CH			IfxDma_ChannelId_5	//����ʹ�õ�DMAͨ�� 0-47��ѡ  ͨ����Խ�����ȼ�Խ��

//����ͷ����ö��
//typedef enum
//{
//    INIT = 0,               //����ͷ��ʼ������
//    AUTO_EXP,               //�Զ��ع�����
//    EXP_TIME,               //�ع�ʱ������
//    FPS,                    //����ͷ֡������
//    SET_COL,                //ͼ��������
//    SET_ROW,                //ͼ��������
//    LR_OFFSET,              //ͼ������ƫ������
//    UD_OFFSET,              //ͼ������ƫ������
//    GAIN,                   //ͼ��ƫ������
//    CONFIG_FINISH,          //������λ����Ҫ����ռλ����
//
//    COLOR_GET_WHO_AM_I = 0xEF,
//    SET_EXP_TIME = 0XF0,    //���������ع�ʱ������
//    GET_STATUS,             //��ȡ����ͷ��������
//    GET_VERSION,            //�̼��汾������
//
//    SET_ADDR = 0XFE,        //�Ĵ�����ַ����
//    SET_DATA                //�Ĵ�����������
//}CMD;
     
     

extern uint8    mt9v03x_finish_flag;//һ��ͼ��ɼ���ɱ�־λ
//extern uint8    mt9v03x_image[MT9V03X_H][MT9V03X_W];

void mt9v03x_init(void);
void mt9v03x_vsync(void);
void mt9v03x_dma(void);

//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//���������񲿷�


/*! �Աȶ� �߶Աȶ�0x03c7  �ͶԱȶ�0x01c7  ע�� �߶ԱȶȻ�ʹͼ��䰵 */
#define CAMERA_CONTRAST          0x01c7

/*! �Զ��ع⿪�� Ĭ�ϴ�  ����Ϊ0 �ر��Զ��ع� */
#define CAMERA_AUTO_EXPOSURE     1

/*! �Զ��ع�ģʽ�� ���� ���ڷ�Χ 1-64 */
#define CAMERA_AUTO_EXPOSURE_BRIGHTNESS  60

/*! �������Զ��ع����������ù̶�֡�� */
/*! �Զ��ع�ģʽ��Ҫ�����ع�ʱ������ ���ڷ�Χ 1�C32765 */
/*! ע�� ��֡�ʹ���ʱ ������õ��ع�ʱ����� ֡�ʿ��ܻ�����Ӧ�µ� */
/*! ����ʹ��100֡ʱ ����ع�ʱ�䳬��317 �ᵼ��֡���½� */
/*! �ع�ʱ��Խ�� ͼ��Խ�� */
#define CAMERA_MAX_EXPOSURE_TIME 250
#define CAMERA_MIN_EXPOSURE_TIME  1

/*! ���Զ��ع�ģʽ�� ���Ե����ع�ʱ��������ͼ���������� ���ڷ�Χ 0�C32765 */
/*! ע�� ��֡�ʹ���ʱ ������õ��ع�ʱ����� ֡�ʿ��ܻ�����Ӧ�µ� */
/*! �ع�ʱ��Խ�� ͼ��Խ�� */
#define CAMERA_EXPOSURE_TIME  180


/* ����ͷʹ��SCCBͨ�� SCCB��IIC�������� */
#define MT9V034_SCL_PIN   P11_2   /*!< SCCB SCL �ܽ� */
#define MT9V034_SDA_PIN   P11_3   /*!< SCCB SDA �ܽ� */


#define MT9V034_SCL_Out   gpio_dir(MT9V034_SCL_PIN, GPO, PULLDOWN);      //���������ΪSCL_Out
#define MT9V034_SDA_Out   gpio_dir(MT9V034_SDA_PIN, GPO, PULLDOWN);      //������Ϊ�����ΪSDA_Out
#define MT9V034_SDA_In    gpio_dir(MT9V034_SDA_PIN, GPI, PULLDOWN);      //������Ϊ������ΪSDA_In
#define MT9V034_SCL_High  gpio_set(MT9V034_SCL_PIN, 1);      //��������ߵ�ƽ
#define MT9V034_SCL_Low   gpio_set(MT9V034_SCL_PIN, 0);      //��������͵�ƽ
#define MT9V034_SDA_High  gpio_set(MT9V034_SDA_PIN, 1);      //��������ߵ�ƽ
#define MT9V034_SDA_Low   gpio_set(MT9V034_SDA_PIN, 0);      //��������͵�ƽ
#define MT9V034_SDA_Data  gpio_get(MT9V034_SDA_PIN)           //��ȡ�����ϵ�����״̬



void MT9V034_Init(unsigned char fps);
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
void MT9V034_SetFrameRate(unsigned char fps);
static void MT9V034_SetFrameResolution(unsigned short height,unsigned short width);
void MT9V034_SetAutoExposure(char enable);





/*********************************************************************
 *����ͷSCCB�ײ�����
 *�ڲ�����
 ***********************************************************************/
void SCCB_Init(void);
void SCCB_Wait(void);
void SCCB_Stop(void);
void SCCB_Star(void);
unsigned char SCCB_SendByte(unsigned char Data);
void SCCB_RegWrite(unsigned char Device,unsigned char Address,unsigned short Data);
unsigned char SCCB_RegRead(unsigned char Device,unsigned char Address,unsigned short *Data) ;
void MTV_IICWriteReg16(unsigned char reg, unsigned short val);


#define MT9V034_I2C_ADDR                        0xB8 //(0xB8 >> 1)=0x5C
#define MAX_IMAGE_HEIGHT            480
#define MAX_IMAGE_WIDTH             752
#define MT9V034_PIXEL_ARRAY_HEIGHT              492
#define MT9V034_PIXEL_ARRAY_WIDTH               782
#define MT9V034_CHIP_VERSION                0x00
#define MT9V034_CHIP_ID                     0x1324

#define MT9V034_COLUMN_START                        0x01
#define MT9V034_COLUMN_START_MIN                1
#define MT9V034_COLUMN_START_DEF                1
#define MT9V034_COLUMN_START_MAX                752

#define MT9V034_ROW_START                       0x02
#define MT9V034_ROW_START_MIN               4
#define MT9V034_ROW_START_DEF               4
#define MT9V034_ROW_START_MAX               482

#define MT9V034_WINDOW_HEIGHT               0x03
#define MT9V034_WINDOW_HEIGHT_MIN               1
#define MT9V034_WINDOW_HEIGHT_DEF               64
#define MT9V034_WINDOW_HEIGHT_MAX               480

#define MT9V034_WINDOW_WIDTH                        0x04
#define MT9V034_WINDOW_WIDTH_MIN                1
#define MT9V034_WINDOW_WIDTH_DEF                64
#define MT9V034_WINDOW_WIDTH_MAX                752

#define MINIMUM_HORIZONTAL_BLANKING     91 // see datasheet

#define MT9V034_HORIZONTAL_BLANKING             0x05
#define MT9V034_HORIZONTAL_BLANKING_MIN     43
#define MT9V034_HORIZONTAL_BLANKING_MAX     1023

#define MT9V034_VERTICAL_BLANKING               0x06
#define MT9V034_VERTICAL_BLANKING_MIN       4
#define MT9V034_VERTICAL_BLANKING_MAX       3000

#define MT9V034_CHIP_CONTROL                        0x07
#define MT9V034_CHIP_CONTROL_MASTER_MODE         (1 << 3)
#define MT9V034_CHIP_CONTROL_DOUT_ENABLE         (1 << 7)
#define MT9V034_CHIP_CONTROL_SEQUENTIAL     (1 << 8)

#define MT9V034_SHUTTER_WIDTH1              0x08
#define MT9V034_SHUTTER_WIDTH2              0x09
#define MT9V034_SHUTTER_WIDTH_CONTROL       0x0A
#define MT9V034_TOTAL_SHUTTER_WIDTH     0x0B
#define MT9V034_TOTAL_SHUTTER_WIDTH_MIN     1
#define MT9V034_TOTAL_SHUTTER_WIDTH_DEF     480
#define MT9V034_TOTAL_SHUTTER_WIDTH_MAX     32767

#define MT9V034_RESET                       0x0C

#define MT9V034_READ_MODE                       0x0D
#define MT9V034_READ_MODE_ROW_BIN_MASK      (3 << 0)
#define MT9V034_READ_MODE_ROW_BIN_SHIFT     0
#define MT9V034_READ_MODE_COLUMN_BIN_MASK        (3 << 2)
#define MT9V034_READ_MODE_COLUMN_BIN_SHIFT       2
#define MT9V034_READ_MODE_ROW_BIN_2         (1<<0)
#define MT9V034_READ_MODE_ROW_BIN_4         (1<<1)
#define MT9V034_READ_MODE_COL_BIN_2         (1<<2)
#define MT9V034_READ_MODE_COL_BIN_4         (1<<3)
#define MT9V034_READ_MODE_ROW_FLIP              (1 << 4)
#define MT9V034_READ_MODE_COLUMN_FLIP       (1 << 5)
#define MT9V034_READ_MODE_DARK_COLUMNS      (1 << 6)
#define MT9V034_READ_MODE_DARK_ROWS             (1 << 7)

#define MT9V034_PIXEL_OPERATION_MODE                0x0F
#define MT9V034_PIXEL_OPERATION_MODE_COLOR       (1 << 2)
#define MT9V034_PIXEL_OPERATION_MODE_HDR         (1 << 6)

#define MT9V034_V1_CTRL_REG_A           0x31
#define MT9V034_V2_CTRL_REG_A           0x32
#define MT9V034_V3_CTRL_REG_A           0x33
#define MT9V034_V4_CTRL_REG_A           0x34

#define MT9V034_ANALOG_GAIN                     0x35
#define MT9V034_ANALOG_GAIN_MIN             16
#define MT9V034_ANALOG_GAIN_DEF             16
#define MT9V034_ANALOG_GAIN_MAX             64

#define MT9V034_MAX_ANALOG_GAIN             0x36
#define MT9V034_MAX_ANALOG_GAIN_MAX             127

#define MT9V034_FRAME_DARK_AVERAGE              0x42
#define MT9V034_DARK_AVG_THRESH             0x46
#define MT9V034_DARK_AVG_LOW_THRESH_MASK         (255 << 0)
#define MT9V034_DARK_AVG_LOW_THRESH_SHIFT        0
#define MT9V034_DARK_AVG_HIGH_THRESH_MASK       (255 << 8)
#define MT9V034_DARK_AVG_HIGH_THRESH_SHIFT      8

#define MT9V034_ROW_NOISE_CORR_CONTROL      0x70
#define MT9V034_ROW_NOISE_CORR_ENABLE       (1 << 5)
#define MT9V034_ROW_NOISE_CORR_USE_BLK_AVG      (1 << 7)

#define MT9V034_PIXEL_CLOCK                 0x74
#define MT9V034_PIXEL_CLOCK_INV_LINE            (1 << 0)
#define MT9V034_PIXEL_CLOCK_INV_FRAME       (1 << 1)
#define MT9V034_PIXEL_CLOCK_XOR_LINE            (1 << 2)
#define MT9V034_PIXEL_CLOCK_CONT_LINE       (1 << 3)
#define MT9V034_PIXEL_CLOCK_INV_PXL_CLK     (1 << 4)

#define MT9V034_TEST_PATTERN                    0x7f
#define MT9V034_TEST_PATTERN_DATA_MASK      (1023 << 0)
#define MT9V034_TEST_PATTERN_DATA_SHIFT     0
#define MT9V034_TEST_PATTERN_USE_DATA       (1 << 10)
#define MT9V034_TEST_PATTERN_GRAY_MASK      (3 << 11)
#define MT9V034_TEST_PATTERN_GRAY_NONE      (0 << 11)
#define MT9V034_TEST_PATTERN_GRAY_VERTICAL      (1 << 11)
#define MT9V034_TEST_PATTERN_GRAY_HORIZONTAL        (2 << 11)
#define MT9V034_TEST_PATTERN_GRAY_DIAGONAL      (3 << 11)
#define MT9V034_TEST_PATTERN_ENABLE         (1 << 13)
#define MT9V034_TEST_PATTERN_FLIP           (1 << 14)

#define MT9V034_AEC_AGC_ENABLE          0xAF
#define MT9V034_AEC_ENABLE                  (1 << 0)
#define MT9V034_AGC_ENABLE                  (1 << 1)
#define MT9V034_THERMAL_INFO                    0xc1
#define MT9V034_ANALOG_CTRL                     (0xC2)
#define MT9V034_ANTI_ECLIPSE_ENABLE                 (1<<7)
#define MT9V034_MAX_GAIN                        (0xAB)
#define MT9V034_FINE_SHUTTER_WIDTH_TOTAL_A      (0xD5)
#define MT9V034_HDR_ENABLE_REG          0x0F
#define MT9V034_ADC_RES_CTRL_REG            0x1C
#define MT9V034_ROW_NOISE_CORR_CTRL_REG 0x70
#define MT9V034_TEST_PATTERN_REG        0x7F
#define MT9V034_TILED_DIGITAL_GAIN_REG  0x80
#define MT9V034_AGC_AEC_DESIRED_BIN_REG 0xA5
#define MT9V034_MAX_GAIN_REG            0xAB
#define MT9V034_MIN_EXPOSURE_REG        0xAC  // datasheet min coarse shutter width
#define MT9V034_MAX_EXPOSURE_REG        0xAD  // datasheet max coarse shutter width
#define MT9V034_AEC_AGC_ENABLE_REG  0xAF
#define MT9V034_AGC_AEC_PIXEL_COUNT_REG 0xB0




#endif

