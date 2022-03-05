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
 
 
#include "IfxStm.h"
#include "IFXSTM_CFG.h"
#include "zf_stm_systick.h"

static uint32 systick_count[2];

//-------------------------------------------------------------------------------------------------------------------
//  @brief      systick��ʱ����
//  @param      time            ��Ҫ��ʱ��ʱ��
//  @return     void
//  Sample usage:               �����û����ã��û���ʹ��h�ļ��еĺ궨��
//-------------------------------------------------------------------------------------------------------------------
void systick_delay(STMN_enum stmn, uint32 time)
{
	uint32 stm_clk;
	stm_clk = IfxStm_getFrequency(IfxStm_getAddress((IfxStm_Index)stmn));

	IfxStm_waitTicks(IfxStm_getAddress((IfxStm_Index)stmn), (uint32)((uint64)stm_clk*time/1000000000));
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      systick��ʱ������
//  @param      stmn			ѡ��ʹ�õ�ģ��
//  @return     void
//  Sample usage:				systick_start(STM0);//��¼�µ�ǰ��ʱ��
//-------------------------------------------------------------------------------------------------------------------
void systick_start(STMN_enum stmn)
{

	systick_count[stmn] = IfxStm_getLower(IfxStm_getAddress((IfxStm_Index)stmn));
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief     ��õ�ǰSystem tick timer��ֵ
//  @param     stmn				ѡ��ʹ�õ�ģ��
//  @return    uint32 			���شӿ�ʼ�����ڵ�ʱ��(��λ10ns)
//  Sample usage:               uint32 tim = systick_getval(STM0);
//-------------------------------------------------------------------------------------------------------------------
uint32 systick_getval(STMN_enum stmn)
{
	uint32 time;
	uint32 stm_clk;

	stm_clk = IfxStm_getFrequency(IfxStm_getAddress((IfxStm_Index)stmn));

	time = IfxStm_getLower(IfxStm_getAddress((IfxStm_Index)stmn)) - systick_count[stmn];
	time = (uint32)((uint64)time * 100000000 / stm_clk);
	return time;
}
