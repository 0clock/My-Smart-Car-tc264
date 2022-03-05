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
 
#include "IfxGpt12_IncrEnc.h"
#include "zf_assert.h"
#include "zf_gpt12.h"

void gpt12_mux(GPTN_enum gptn, GPT_PIN_enum count_pin, GPT_PIN_enum dir_pin)
{
	IfxGpt12_TxIn_In *count;
	IfxGpt12_TxEud_In *dir;

	switch(gptn)
	{
		case GPT12_T2:
		{
			if		(GPT12_T2INA_P00_7 == count_pin)	count = &IfxGpt120_T2INA_P00_7_IN;
			else if	(GPT12_T2INB_P33_7 == count_pin)	count = &IfxGpt120_T2INB_P33_7_IN;
			else	ZF_ASSERT(FALSE);

			if		(GPT12_T2EUDA_P00_8 == dir_pin)		dir = &IfxGpt120_T2EUDA_P00_8_IN;
			else if	(GPT12_T2EUDB_P33_6 == dir_pin)		dir = &IfxGpt120_T2EUDB_P33_6_IN;
			else	ZF_ASSERT(FALSE);
		}break;

		case GPT12_T3:
		{
			if		(GPT12_T3INA_P02_6 == count_pin)	count = &IfxGpt120_T3INA_P02_6_IN;
			else if	(GPT12_T3INB_P10_4 == count_pin)	count = &IfxGpt120_T3INB_P10_4_IN;
			else	ZF_ASSERT(FALSE);

			if		(GPT12_T3EUDA_P02_7 == dir_pin)		dir = &IfxGpt120_T3EUDA_P02_7_IN;
			else if	(GPT12_T3EUDB_P10_7 == dir_pin)		dir = &IfxGpt120_T3EUDB_P10_7_IN;
			else	ZF_ASSERT(FALSE);
		}break;

		case GPT12_T4:
		{
			if		(GPT12_T4INA_P02_8 == count_pin)	count = &IfxGpt120_T4INA_P02_8_IN;
			else if	(GPT12_T4INB_P10_8 == count_pin)	count = &IfxGpt120_T4INB_P10_8_IN;
			else	ZF_ASSERT(FALSE);

			if		(GPT12_T4EUDA_P00_9 == dir_pin)		dir = &IfxGpt120_T4EUDA_P00_9_IN;
			else if	(GPT12_T4EUDB_P33_5 == dir_pin)		dir = &IfxGpt120_T4EUDB_P33_5_IN;
			else	ZF_ASSERT(FALSE);
		}break;

		case GPT12_T5:
		{
			if		(GPT12_T5INA_P21_7 == count_pin)	count = &IfxGpt120_T5INA_P21_7_IN;
			else if	(GPT12_T5INB_P10_3 == count_pin)	count = &IfxGpt120_T5INB_P10_3_IN;
			else	ZF_ASSERT(FALSE);

			if		(GPT12_T5EUDA_P21_6 == dir_pin)		dir = &IfxGpt120_T5EUDA_P21_6_IN;
			else if	(GPT12_T5EUDB_P10_1 == dir_pin)		dir = &IfxGpt120_T5EUDB_P10_1_IN;
			else	ZF_ASSERT(FALSE);
		}break;

		case GPT12_T6:
		{
			if		(GPT12_T6INA_P20_3 == count_pin)	count = &IfxGpt120_T6INA_P20_3_IN;
			else if	(GPT12_T6INB_P10_2 == count_pin)	count = &IfxGpt120_T6INB_P10_2_IN;
			else	ZF_ASSERT(FALSE);

			if		(GPT12_T6EUDA_P20_0 == dir_pin)		dir = &IfxGpt120_T6EUDA_P20_0_IN;
			else if	(GPT12_T6EUDB_P10_0 == dir_pin)		dir = &IfxGpt120_T6EUDB_P10_0_IN;
			else	ZF_ASSERT(FALSE);
		}break;
	}
#pragma warning 507
	IfxGpt12_initTxInPinWithPadLevel(count, IfxPort_InputMode_pullUp, IfxPort_PadDriver_cmosAutomotiveSpeed1);
	IfxGpt12_initTxEudInPinWithPadLevel(dir, IfxPort_InputMode_pullUp, IfxPort_PadDriver_cmosAutomotiveSpeed1);
#pragma warning default
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPT12��ʼ�����������ɼ���
//  @param      gptn         	ѡ����ʹ�õ�GPT12��ʱ��
//  @param      count_pin 		���ü�������
//  @param      dir_pin     	���ü�����������
//  @return     void
//  Sample usage:           	gpt12_init(GPT12_T2, GPT12_T2INA_P00_7, GPT12_T2EUDA_P00_8);//ʹ��T2��ʱ��   P00_7���Ž��м���    ��������ʹ��P00_8����
//-------------------------------------------------------------------------------------------------------------------
void gpt12_init(GPTN_enum gptn, GPT_PIN_enum count_pin, GPT_PIN_enum dir_pin)
{
	IfxGpt12_enableModule(&MODULE_GPT120);
    IfxGpt12_setGpt1BlockPrescaler(&MODULE_GPT120, IfxGpt12_Gpt1BlockPrescaler_4);
    IfxGpt12_setGpt2BlockPrescaler(&MODULE_GPT120, IfxGpt12_Gpt2BlockPrescaler_4);
	gpt12_mux(gptn, count_pin, dir_pin);

	switch(gptn)
	{
		case GPT12_T2:
		{
			IfxGpt12_T2_setCounterInputMode(&MODULE_GPT120, IfxGpt12_CounterInputMode_risingEdgeTxIN);
			IfxGpt12_T2_setDirectionSource(&MODULE_GPT120, IfxGpt12_TimerDirectionSource_external);
			IfxGpt12_T2_setMode(&MODULE_GPT120, IfxGpt12_Mode_counter);
			IfxGpt12_T2_run(&MODULE_GPT120, IfxGpt12_TimerRun_start);
		}break;

		case GPT12_T3:
		{
			IfxGpt12_T3_setCounterInputMode(&MODULE_GPT120, IfxGpt12_CounterInputMode_risingEdgeTxIN);
			IfxGpt12_T3_setDirectionSource(&MODULE_GPT120, IfxGpt12_TimerDirectionSource_external);
			IfxGpt12_T3_setMode(&MODULE_GPT120, IfxGpt12_Mode_counter);
			IfxGpt12_T3_run(&MODULE_GPT120, IfxGpt12_TimerRun_start);
		}break;

		case GPT12_T4:
		{
			IfxGpt12_T4_setCounterInputMode(&MODULE_GPT120, IfxGpt12_CounterInputMode_risingEdgeTxIN);
			IfxGpt12_T4_setDirectionSource(&MODULE_GPT120, IfxGpt12_TimerDirectionSource_external);
			IfxGpt12_T4_setMode(&MODULE_GPT120, IfxGpt12_Mode_counter);
			IfxGpt12_T4_run(&MODULE_GPT120, IfxGpt12_TimerRun_start);
		}break;

		case GPT12_T5:
		{
			IfxGpt12_T5_setCounterInputMode(&MODULE_GPT120, IfxGpt12_CounterInputMode_risingEdgeTxIN);
			IfxGpt12_T5_setDirectionSource(&MODULE_GPT120, IfxGpt12_TimerDirectionSource_external);
			IfxGpt12_T5_setMode(&MODULE_GPT120, IfxGpt12_Mode_counter);
			IfxGpt12_T5_run(&MODULE_GPT120, IfxGpt12_TimerRun_start);
		}break;

		case GPT12_T6:
		{
			IfxGpt12_T6_setCounterInputMode(&MODULE_GPT120, IfxGpt12_CounterInputMode_risingEdgeTxIN);
			IfxGpt12_T6_setDirectionSource(&MODULE_GPT120, IfxGpt12_TimerDirectionSource_external);
			IfxGpt12_T6_setMode(&MODULE_GPT120, IfxGpt12_Mode_counter);
			IfxGpt12_T6_run(&MODULE_GPT120, IfxGpt12_TimerRun_start);
		}break;

	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPT12������ȡ���������ɼ���
//  @param      gptn         	ѡ����ʹ�õ�GPT12��ʱ��
//  @return     void
//  Sample usage:           	speed = gpt12_get(GPT12_T2);//ʹ��T2��ʱ��
//-------------------------------------------------------------------------------------------------------------------
int16 gpt12_get(GPTN_enum gptn)
{
	switch(gptn)
	{
		case GPT12_T2: return (int16)IfxGpt12_T2_getTimerValue(&MODULE_GPT120);
		case GPT12_T3: return (int16)IfxGpt12_T3_getTimerValue(&MODULE_GPT120);
		case GPT12_T4: return (int16)IfxGpt12_T4_getTimerValue(&MODULE_GPT120);
		case GPT12_T5: return (int16)IfxGpt12_T5_getTimerValue(&MODULE_GPT120);
		case GPT12_T6: return (int16)IfxGpt12_T6_getTimerValue(&MODULE_GPT120);
		default: return 0;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���GPT12�������������ɼ���
//  @param      gptn         	ѡ����ʹ�õ�GPT12��ʱ��
//  @return     void
//  Sample usage:           	gpt12_clear(GPT12_T2);//ʹ��T2��ʱ��
//-------------------------------------------------------------------------------------------------------------------
void gpt12_clear(GPTN_enum gptn)
{
	switch(gptn)
	{
		case GPT12_T2: IfxGpt12_T2_setTimerValue(&MODULE_GPT120, 0); break;
		case GPT12_T3: IfxGpt12_T3_setTimerValue(&MODULE_GPT120, 0); break;
		case GPT12_T4: IfxGpt12_T4_setTimerValue(&MODULE_GPT120, 0); break;
		case GPT12_T5: IfxGpt12_T5_setTimerValue(&MODULE_GPT120, 0); break;
		case GPT12_T6: IfxGpt12_T6_setTimerValue(&MODULE_GPT120, 0); break;
	}
}
