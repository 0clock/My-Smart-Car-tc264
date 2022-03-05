/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		stm_systick
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		ADS v1.2.2
 * @Target core		TC264D
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-3-23
 ********************************************************************************************************************/
 
#ifndef _zf_stm_systick_h
#define _zf_stm_systick_h

#include "common.h"



typedef enum  // ö��STMģ���
{
    STM0,
	STM1,
}STMN_enum;


void systick_delay(STMN_enum stmn, uint32 time);
void systick_start(STMN_enum stmn);
uint32 systick_getval(STMN_enum stmn);

//------------------------------------���º궨��������ʱ------------------------------------
#define systick_delay_ms(stmn, time)	systick_delay(stmn, time*1000000)   //������ʱʱ��  ��λms
#define systick_delay_us(stmn, time)	systick_delay(stmn, time*1000)      //������ʱʱ��  ��λus
#define systick_delay_ns(stmn, time)	systick_delay(stmn, time)   		//������ʱʱ��  ��λns

//------------------------------------���º궨�����ڻ�ȡ��ǰʱ��------------------------------------
#define systick_getval_ms(stmn)         systick_getval(stmn)/100000         //��ȡ��ǰ��ʱʱ��  ��λms
#define systick_getval_us(stmn)         systick_getval(stmn)/100            //��ȡ��ǰ��ʱʱ��  ��λus
#define systick_getval_ns(stmn)         systick_getval(stmn)*10    			//��ȡ��ǰ��ʱʱ��  ��λns


#endif
