/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		zf_vadc
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		ADS v1.2.2
 * @Target core		TC264D
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-3-23
 ********************************************************************************************************************/
 
#ifndef _zf_vadc_h
#define _zf_vadc_h

#include "common.h"



//��ö�ٶ��岻�����û��޸�
typedef enum    // ö��ADC���
{
	ADC_0,
	ADC_1,
	ADC_2,
}VADCN_enum;

typedef enum  // ö��ADCͨ��
{
    //ADC0��ѡ����
    ADC0_CH0_A0   = 0*16 + 0,
    ADC0_CH1_A1,
    ADC0_CH2_A2,
    ADC0_CH3_A3,
    ADC0_CH4_A4,
    ADC0_CH5_A5,
    ADC0_CH6_A6,
    ADC0_CH7_A7,
    ADC0_CH8_A8,
    ADC0_CH10_A10 = 0*16 + 10,
    ADC0_CH11_A11,
    ADC0_CH12_A12,
    ADC0_CH13_A13,
	
	//ADC1��ѡ����
	ADC1_CH0_A16  = 1*16 + 0,
	ADC1_CH1_A17  = 1*16 + 1,
	ADC1_CH4_A20  = 1*16 + 4,
	ADC1_CH5_A21  = 1*16 + 5,
	ADC1_CH8_A24  = 1*16 + 8,
	ADC1_CH9_A25  = 1*16 + 9,
	
	//ADC2��ѡ����
	ADC2_CH3_A35  = 2*16 + 3,
	ADC2_CH4_A36,
	ADC2_CH5_A37,
	ADC2_CH6_A38,
	ADC2_CH7_A39,
	ADC2_CH10_A44 = 2*16 + 10,
	ADC2_CH11_A45,
	ADC2_CH12_A46,
	ADC2_CH13_A47,
	ADC2_CH14_A48,
	ADC2_CH15_A49,
}VADC_CHN_enum;

//��ö�ٶ��岻�����û��޸�
typedef enum    // ö��ADCͨ��
{
    ADC_8BIT,     //8λ�ֱ���
    ADC_10BIT,    //10λ�ֱ���
    ADC_12BIT,    //12λ�ֱ���
}VADC_RES_enum;

void adc_init(VADCN_enum vadc_n, VADC_CHN_enum vadc_chn);
uint16 adc_convert(VADCN_enum vadc_n, VADC_CHN_enum vadc_chn, VADC_RES_enum vadc_res);
uint16 adc_mean_filter(VADCN_enum vadc_n, VADC_CHN_enum vadc_chn, VADC_RES_enum vadc_res, uint8 count);
#endif
