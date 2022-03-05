/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		eru
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		ADS v1.2.2
 * @Target core		TC264D
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-3-23
 ********************************************************************************************************************/
 
#ifndef _zf_eru_h
#define _zf_eru_h

#include "common.h"



typedef enum  // ö�ٴ�����ʽ
{
    RISING,
    FALLING,
    BOTH,
}TRIGGER_enum;

typedef enum  // ö��ERUͨ��
{
	//һ��ͨ��ֻ��ѡ������һ��������Ϊ �ⲿ�жϵ�����
	//����ͨ��0 ��ѡ����ΪP10_7 �� P15_4��
	//��LQFP144��װ��û��P10_7
	ERU_CH0_REQ4_P10_7   = 0*3,  ERU_CH0_REQ0_P15_4,						//ͨ��0��ѡ����   LQFPû��P10_7����
	//��LQFP144��װ��û��P10_8
	ERU_CH1_REQ5_P10_8   = 1*3,  ERU_CH1_REQ10_P14_3,						//ͨ��1��ѡ����   LQFPû��P10_8����
	ERU_CH2_REQ7_P00_4   = 2*3,  ERU_CH2_REQ14_P02_1, ERU_CH2_REQ2_P10_2,	//ͨ��2��ѡ����
	ERU_CH3_REQ6_P02_0   = 3*3,  ERU_CH3_REQ3_P10_3,  ERU_CH3_REQ15_P14_1,	//ͨ��3��ѡ����

	//ͨ��4��ͨ��0 �����жϺ��� ���ж���ͨ���жϱ�־λ��ʶ�����ĸ�ͨ���������ж�
	ERU_CH4_REQ13_P15_5  = 4*3,  ERU_CH4_REQ8_P33_7,						//ͨ��4��ѡ����
	//ͨ��5��ͨ��1 �����жϺ���
	ERU_CH5_REQ1_P15_8   = 5*3,												//ͨ��5��ѡ����
	//ͨ��6��ͨ��2 �����жϺ���
	ERU_CH6_REQ12_P11_10 = 6*3,  ERU_CH6_REQ9_P20_0,						//ͨ��6��ѡ����
	//ͨ��7��ͨ��3 �����жϺ���
	ERU_CH7_REQ16_P15_1  = 7*3,  ERU_CH7_REQ11_P20_9,						//ͨ��7��ѡ����
}ERU_PIN_enum;

//�ر�ע��ͨ��2 �� ͨ��3��������ͷռ�ã���ζ��ֻ�������ⲿ�жϿɹ��û�ʹ��





//�жϱ�־λ��ȡ
#define GET_GPIO_FLAG(eru_pin)   	IfxScuEru_getEventFlagStatus((IfxScuEru_InputChannel)(eru_pin/3))
//�жϱ�־λ���
#define CLEAR_GPIO_FLAG(eru_pin) 	IfxScuEru_clearEventFlag((IfxScuEru_InputChannel)(eru_pin/3))





void eru_init(ERU_PIN_enum eru_pin, TRIGGER_enum trigger);
void eru_enable_interrupt(ERU_PIN_enum eru_pin);
void eru_disable_interrupt(ERU_PIN_enum eru_pin);
#endif
