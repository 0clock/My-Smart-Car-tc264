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
 
#include "Src/Std/IfxSrc.h"
#include "SysSe/Bsp/Bsp.h"
#include "IfxScuEru.h"
#include "zf_assert.h"
#include "isr_config.h"
#include "zf_eru.h"





IfxScu_Req_In* eru_mux(ERU_PIN_enum eru_pin)
{
	switch(eru_pin)
	{
		case ERU_CH0_REQ4_P10_7:   return &IfxScu_REQ4_P10_7_IN;
		case ERU_CH0_REQ0_P15_4:   return &IfxScu_REQ0_P15_4_IN;

		case ERU_CH1_REQ5_P10_8:   return &IfxScu_REQ5_P10_8_IN;
		case ERU_CH1_REQ10_P14_3:  return &IfxScu_REQ10_P14_3_IN;

		case ERU_CH2_REQ7_P00_4:   return &IfxScu_REQ7_P00_4_IN;
		case ERU_CH2_REQ14_P02_1:  return &IfxScu_REQ14_P02_1_IN;
		case ERU_CH2_REQ2_P10_2:   return &IfxScu_REQ2_P10_2_IN;

		case ERU_CH3_REQ6_P02_0:   return &IfxScu_REQ6_P02_0_IN;
		case ERU_CH3_REQ3_P10_3:   return &IfxScu_REQ3_P10_3_IN;
		case ERU_CH3_REQ15_P14_1:  return &IfxScu_REQ15_P14_1_IN;

		case ERU_CH4_REQ13_P15_5:  return &IfxScu_REQ13_P15_5_IN;
		case ERU_CH4_REQ8_P33_7:   return &IfxScu_REQ8_P33_7_IN;

		case ERU_CH5_REQ1_P15_8:   return &IfxScu_REQ1_P15_8_IN;

		case ERU_CH6_REQ12_P11_10: return &IfxScu_REQ12_P11_10_IN;
		case ERU_CH6_REQ9_P20_0:   return &IfxScu_REQ9_P20_0_IN;

		case ERU_CH7_REQ16_P15_1:  return &IfxScu_REQ16_P15_1_IN;
		case ERU_CH7_REQ11_P20_9:  return &IfxScu_REQ11_P20_9_IN;

		default: ZF_ASSERT(FALSE); return NULL;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      eru��ʼ��(gpio�ж�)
//  @param      eru_pin         ����eruͨ��������
//  @param      trigger         ���ô�����ʽ
//	@return		void
//  Sample usage:				eru_init(ERU_CH0_REQ0_P15_4, RISING);//eruͨ��0 ʹ��P10_7���ţ������ش����ж�
//-------------------------------------------------------------------------------------------------------------------
void eru_init(ERU_PIN_enum eru_pin, TRIGGER_enum trigger)
{
    boolean interrupt_state = disableInterrupts();

    IfxScu_Req_In *reqPin;

	reqPin = eru_mux(eru_pin);

    IfxScuEru_initReqPin(reqPin, IfxPort_InputMode_pullUp);

    IfxScuEru_InputChannel inputChannel = (IfxScuEru_InputChannel)reqPin->channelId;

    IfxScuEru_InputNodePointer triggerSelect = (IfxScuEru_InputNodePointer)(eru_pin/3);
    IfxScuEru_OutputChannel    outputChannel = (IfxScuEru_OutputChannel)(eru_pin/3);

    switch(trigger)
    {
    	case RISING:
    	{
    		IfxScuEru_disableFallingEdgeDetection(inputChannel);
    		IfxScuEru_enableRisingEdgeDetection(inputChannel);
    	}break;

    	case FALLING:
    	{
    		IfxScuEru_enableFallingEdgeDetection(inputChannel);
    		IfxScuEru_disableRisingEdgeDetection(inputChannel);
    	}break;

    	case BOTH:
    	{
    		IfxScuEru_enableFallingEdgeDetection(inputChannel);
    		IfxScuEru_enableRisingEdgeDetection(inputChannel);
    	}break;

    	default: ZF_ASSERT(FALSE);
    }

    IfxScuEru_enableTriggerPulse(inputChannel);
    IfxScuEru_connectTrigger(inputChannel, triggerSelect);

    IfxScuEru_setFlagPatternDetection(outputChannel, inputChannel, TRUE);
    IfxScuEru_enablePatternDetectionTrigger(outputChannel);
    IfxScuEru_setInterruptGatingPattern(outputChannel, IfxScuEru_InterruptGatingPattern_alwaysActive);


	volatile Ifx_SRC_SRCR *src = &MODULE_SRC.SCU.SCU.ERU[(int)outputChannel % 4];
	IfxSrc_Tos eru_service;
	uint8 eru_priority;
 	switch((eru_pin/3)%4)
 	{
		case 0:
		{
			eru_service  = ERU_CH0_CH4_INT_SERVICE;
			eru_priority = ERU_CH0_CH4_INT_PRIO;
		}break;

		case 1:
		{
			eru_service  = ERU_CH1_CH5_INT_SERVICE;
			eru_priority = ERU_CH1_CH5_INT_PRIO;
		}break;

		case 2:
		{
			eru_service  = ERU_CH2_CH6_INT_SERVICE;
			eru_priority = ERU_CH2_CH6_INT_PRIO;
		}break;

		case 3:
		{
			eru_service  = ERU_CH3_CH7_INT_SERVICE;
			eru_priority = ERU_CH3_CH7_INT_PRIO;
		}break;

 	}
#pragma warning 507
 	IfxSrc_init(src, eru_service, eru_priority);
#pragma warning default
 	IfxSrc_enable(src);

    restoreInterrupts(interrupt_state);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����eru�ж�
//  @param      eru_pin         ����eruͨ��������
//	@return		void
//  Sample usage:				eru_enable_interrupt(ERU_CH0_REQ0_P15_4);
//-------------------------------------------------------------------------------------------------------------------
void eru_enable_interrupt(ERU_PIN_enum eru_pin)
{
	IfxScuEru_OutputChannel    outputChannel = (IfxScuEru_OutputChannel)(eru_pin/3);

	volatile Ifx_SRC_SRCR *src = &MODULE_SRC.SCU.SCU.ERU[(int)outputChannel % 4];
	IfxSrc_enable(src);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ر�eru�ж�
//  @param      eru_pin         ����eruͨ��������
//	@return		void
//  Sample usage:				eru_disable_interrupt(ERU_CH0_REQ0_P15_4);
//-------------------------------------------------------------------------------------------------------------------
void eru_disable_interrupt(ERU_PIN_enum eru_pin)
{
	IfxScuEru_OutputChannel    outputChannel = (IfxScuEru_OutputChannel)(eru_pin/3);

	volatile Ifx_SRC_SRCR *src = &MODULE_SRC.SCU.SCU.ERU[(int)outputChannel % 4];
	IfxSrc_disable(src);
}

