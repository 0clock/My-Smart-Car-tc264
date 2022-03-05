/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		spi
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		ADS v1.2.2
 * @Target core		TC264D
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-3-23
 ********************************************************************************************************************/
 
#ifndef _zf_spi_h
#define _zf_spi_h

#include "common.h"



typedef enum    // SPIģ���
{
	SPI_0,
    SPI_1,
    SPI_2,
    SPI_3,
}SPIN_enum;

typedef enum    // ö��SPI����
{

	SPI0_SCLK_P20_11 = 0*102+0*6 , SPI0_SCLK_P20_13,//SPI0 CLK ���ſ�ѡ��Χ
	SPI0_MOSI_P20_12 = 0*102+1*6 , SPI0_MOSI_P20_14,//SPI0 MOSI���ſ�ѡ��Χ
	SPI0_MISO_P20_12 = 0*102+2*6 ,					//SPI0 MISO���ſ�ѡ��Χ

	SPI0_CS0_P20_8   = 0*102+3*6 ,					//SPI0 CS0 ���ſ�ѡ��Χ
	SPI0_CS1_P20_9   = 0*102+4*6 ,
	SPI0_CS2_P20_13  = 0*102+5*6 ,
	SPI0_CS3_P11_10  = 0*102+6*6 ,
	SPI0_CS4_P11_11  = 0*102+7*6 ,
	SPI0_CS5_P11_2   = 0*102+8*6 ,
	SPI0_CS6_P20_10  = 0*102+9*6 ,
	SPI0_CS7_P33_5   = 0*102+10*6,
	SPI0_CS8_P20_6   = 0*102+11*6,
	SPI0_CS9_P20_3   = 0*102+12*6,
	SPI0_CS13_P15_0  = 0*102+16*6,
	//----------------------------------------------------------------------------------

	//��LQFP144��װ��û��P10_4��P10_0
	SPI1_SCLK_P10_2  = 1*102+0*6 , SPI1_SCLK_P11_6,
	SPI1_MOSI_P10_1  = 1*102+1*6 , SPI1_MOSI_P10_3, SPI1_MOSI_P10_4, SPI1_MOSI_P11_9,
	SPI1_MISO_P10_1  = 1*102+2*6 , SPI1_MISO_P11_3,

	SPI1_CS0_P20_8   = 1*102+3*6 ,
	SPI1_CS1_P20_9   = 1*102+4*6 ,
	SPI1_CS2_P20_13  = 1*102+5*6 ,
	SPI1_CS3_P11_10  = 1*102+6*6 ,
	SPI1_CS4_P11_11  = 1*102+7*6 ,
	SPI1_CS5_P11_2   = 1*102+8*6 ,
	SPI1_CS6_P33_10  = 1*102+9*6 ,
	SPI1_CS7_P33_5   = 1*102+10*6,
	SPI1_CS8_P10_4   = 1*102+11*6,
	SPI1_CS9_P10_5   = 1*102+12*6,
	SPI1_CS10_P10_0  = 1*102+13*6,
	//----------------------------------------------------------------------------------

	//��LQFP144��װ��û��P14_7
	SPI2_SCLK_P13_0  = 2*102+0*6 , SPI2_SCLK_P13_1, SPI2_SCLK_P15_3, SPI2_SCLK_P15_6, SPI2_SCLK_P15_8,
	SPI2_MOSI_P13_2  = 2*102+1*6 , SPI2_MOSI_P13_3, SPI2_MOSI_P15_5, SPI2_MOSI_P15_6,
	SPI2_MISO_P15_2  = 2*102+2*6 , SPI2_MISO_P15_4, SPI2_MISO_P15_7, SPI2_MISO_P21_2, SPI2_MISO_P21_3,

	SPI2_CS0_P15_2   = 2*102+3*6 ,
	SPI2_CS1_P14_2   = 2*102+4*6 ,
	SPI2_CS2_P14_6   = 2*102+5*6 ,
	SPI2_CS3_P14_3   = 2*102+6*6 ,
	SPI2_CS4_P14_7   = 2*102+7*6 ,
	SPI2_CS5_P15_1   = 2*102+8*6 ,
	SPI2_CS6_P33_13  = 2*102+9*6 ,
	SPI2_CS7_P20_10  = 2*102+10*6,
	SPI2_CS8_P20_6   = 2*102+11*6,
	SPI2_CS9_P20_3   = 2*102+12*6,
	//----------------------------------------------------------------------------------

	//��LQFP144��װ��û��P10_8��P10_7��P23_5��P23_4
	SPI3_SCLK_P02_7  = 3*102+0*6 , SPI3_SCLK_P10_8, SPI3_SCLK_P22_0, SPI3_SCLK_P22_1, SPI3_SCLK_P22_3, SPI3_SCLK_P33_11,
	SPI3_MOSI_P02_6  = 3*102+1*6 , SPI3_MOSI_P10_6, SPI3_MOSI_P22_0, SPI3_MOSI_P22_2, SPI3_MOSI_P22_3, SPI3_MOSI_P33_12,
	SPI3_MISO_P02_5  = 3*102+2*6 , SPI3_MISO_P10_7, SPI3_MISO_P22_1, SPI3_MISO_P21_2, SPI3_MISO_P21_3, SPI3_MISO_P33_13,

	SPI3_CS0_P02_4   = 3*102+3*6 ,
	SPI3_CS1_P02_0   = 3*102+4*6 , SPI3_CS1_P33_9,
	SPI3_CS2_P02_1   = 3*102+5*6 , SPI3_CS2_P33_8,
	SPI3_CS3_P02_2   = 3*102+6*6 ,
	SPI3_CS4_P02_3   = 3*102+7*6 , SPI3_CS4_P23_5,
	SPI3_CS5_P02_8   = 3*102+8*6 , SPI3_CS5_P23_4,
	SPI3_CS6_P00_8   = 3*102+9*6 ,
	SPI3_CS7_P00_9   = 3*102+10*6, SPI3_CS7_P33_7,
	SPI3_CS8_P10_5   = 3*102+11*6,
	SPI3_CS11_P33_10 = 3*102+14*6,
	SPI3_CS12_P22_2  = 3*102+15*6,
	SPI3_CS13_P23_1  = 3*102+16*6,

	SPI_CS_NOT_USE,
}SPI_PIN_enum;



void spi_init(SPIN_enum spi_n, SPI_PIN_enum sck_pin, SPI_PIN_enum mosi_pin, SPI_PIN_enum miso_pin, SPI_PIN_enum cs_pin, uint8 mode, uint32 baud);
void spi_mosi(SPIN_enum spi_n, SPI_PIN_enum cs_pin, uint8 *modata, uint8 *midata, uint32 len, uint8 continuous);




#endif
