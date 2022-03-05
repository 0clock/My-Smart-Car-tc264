/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		gpt12
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		ADS v1.2.2
 * @Target core		TC264D
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-3-23
 ********************************************************************************************************************/
 
#ifndef _zf_gpt12_h
#define _zf_gpt12_h

#include "common.h"



typedef enum  // ö��GPT12 ��ʱ�����
{
    GPT12_T2,
	GPT12_T3,
	GPT12_T4,
	GPT12_T5,
	GPT12_T6,
}GPTN_enum;


//��ö�ٶ��岻�����û��޸�
typedef enum //ö��GPT12����
{
	GPT12_T2INA_P00_7,  GPT12_T2INB_P33_7,	//T2��ʱ�� �������ſ�ѡ��Χ
	GPT12_T2EUDA_P00_8, GPT12_T2EUDB_P33_6,	//T2��ʱ�� �����������ſ�ѡ��Χ

	//��LQFP144��װ��û��P10_4 ��P10_7
	GPT12_T3INA_P02_6,  GPT12_T3INB_P10_4,	//T3��ʱ�� �������ſ�ѡ��Χ
	GPT12_T3EUDA_P02_7, GPT12_T3EUDB_P10_7, //T3��ʱ�� �����������ſ�ѡ��Χ

	//��LQFP144��װ��û��P10_8
	GPT12_T4INA_P02_8,  GPT12_T4INB_P10_8,	//T4��ʱ�� �������ſ�ѡ��Χ
	GPT12_T4EUDA_P00_9, GPT12_T4EUDB_P33_5, //T4��ʱ�� �����������ſ�ѡ��Χ

	GPT12_T5INB_P10_3,	GPT12_T5INA_P21_7,  //T5��ʱ�� �������ſ�ѡ��Χ
	GPT12_T5EUDB_P10_1, GPT12_T5EUDA_P21_6, //T5��ʱ�� �����������ſ�ѡ��Χ

	//��LQFP144��װ��û��P10_0
	GPT12_T6INB_P10_2,  GPT12_T6INA_P20_3,  //T6��ʱ�� �������ſ�ѡ��Χ
	GPT12_T6EUDB_P10_0, GPT12_T6EUDA_P20_0, //T6��ʱ�� �����������ſ�ѡ��Χ
}GPT_PIN_enum;


void gpt12_init(GPTN_enum gptn, GPT_PIN_enum count_pin, GPT_PIN_enum dir_pin);
int16 gpt12_get(GPTN_enum gptn);
void gpt12_clear(GPTN_enum gptn);

#endif
