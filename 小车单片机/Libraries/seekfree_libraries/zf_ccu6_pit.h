/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		ccu6_pit
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		ADS v1.2.2
 * @Target core		TC264D
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-3-23
 ********************************************************************************************************************/
 
#ifndef _zf_ccu6_pit_h
#define _zf_ccu6_pit_h


#include "common.h"





//��ö�ٶ��岻�����û��޸�
typedef enum //ö��ģ���
{
    CCU6_0,
	CCU6_1,
}CCU6N_enum;

typedef enum //ö��ͨ����
{
    PIT_CH0,
	PIT_CH1,
}CCU6_CHN_enum;



#define PIT_CLEAR_FLAG(ccu6n,pit_ch) IfxCcu6_clearInterruptStatusFlag(IfxCcu6_getAddress((IfxCcu6_Index)ccu6n), (IfxCcu6_InterruptSource)(7+(pit_ch*2)))


void pit_init(CCU6N_enum ccu6n, CCU6_CHN_enum pit_ch, uint32 time);
void pit_close(CCU6N_enum ccu6n, CCU6_CHN_enum pit_ch);
void pit_start(CCU6N_enum ccu6n, CCU6_CHN_enum pit_ch);
void pit_disable_interrupt(CCU6N_enum ccu6n, CCU6_CHN_enum pit_ch);
void pit_enable_interrupt(CCU6N_enum ccu6n, CCU6_CHN_enum pit_ch);


//------------------------------------���´�������PIT�ж�------------------------------------
#define pit_interrupt_ms(ccu6n, pit_ch, time)  pit_init(ccu6n, pit_ch, time*1000)	//(��λΪ ����)
#define pit_interrupt_us(ccu6n, pit_ch, time)  pit_init(ccu6n, pit_ch, time)    	//(��λΪ ΢��)




#endif
