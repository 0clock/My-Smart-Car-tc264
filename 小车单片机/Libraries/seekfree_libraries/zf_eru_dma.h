/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		eru_dma
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		ADS v1.2.2
 * @Target core		TC264D
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-3-23
 ********************************************************************************************************************/
 
#ifndef _zf_eru_dma_h
#define _zf_eru_dma_h

#include "common.h"
#include "zf_eru.h"
#include "IfxDma.h"



#define CLEAR_DMA_FLAG(dma_ch)	IfxDma_clearChannelInterrupt(&MODULE_DMA, dma_ch);


#define DMA_SET_DESTINATION(dma_ch, destination_addr)	IfxDma_setChannelDestinationAddress(&MODULE_DMA, dma_ch, (void *)IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), destination_addr))

uint8 eru_dma_init(IfxDma_ChannelId dma_ch, uint8 *source_addr, uint8 *destination_addr, ERU_PIN_enum eru_pin, TRIGGER_enum trigger, uint16 dma_count);
void dma_stop(IfxDma_ChannelId dma_ch);
void dma_start(IfxDma_ChannelId dma_ch);

#endif
