/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		isr_config
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		ADS v1.2.2
 * @Target core		TC264D
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-3-23
 ********************************************************************************************************************/

#ifndef _isr_config_h
#define _isr_config_h


//ISR_PRIORITY�� TC264����255���ж����ȼ��������� 1-255��0���ȼ���ʾ�������жϣ�255Ϊ������ȼ�
//�ر�ע��
//�ж����ȼ���������һ���������ж����ȼ�����������Ϊ��һ����ֵ
//�ر�ע��



//INT_SERVICE��    �궨������ж���˭����Ҳ��Ϊ�����ṩ�ߣ���TC264�У��жϱ��������񣩣������÷�ΧIfxSrc_Tos_cpu0 IfxSrc_Tos_cpu1 IfxSrc_Tos_dma  ��������Ϊ����ֵ


//���INT_SERVICE����ΪIfxSrc_Tos_dma�Ļ���ISR_PRIORITY�Ŀ����÷�Χ����0-47��








//------------PIT�жϲ�����ض���------------
#define CCU6_0_CH0_INT_SERVICE	IfxSrc_Tos_cpu0	//����CCU6_0 PITͨ��0�жϷ������ͣ����ж�����˭��Ӧ���� 0:CPU0 1:CPU1 3:DMA  ��������Ϊ����ֵ
#define CCU6_0_CH0_ISR_PRIORITY 30	//����CCU6_0 PITͨ��0�ж����ȼ� ���ȼ���Χ1-255 Խ�����ȼ�Խ�� ��ƽʱʹ�õĵ�Ƭ����һ��

#define CCU6_0_CH1_INT_SERVICE	IfxSrc_Tos_cpu0
#define CCU6_0_CH1_ISR_PRIORITY 31

#define CCU6_1_CH0_INT_SERVICE	IfxSrc_Tos_cpu1
#define CCU6_1_CH0_ISR_PRIORITY 32

#define CCU6_1_CH1_INT_SERVICE	IfxSrc_Tos_cpu0
#define CCU6_1_CH1_ISR_PRIORITY 33



//------------GPIO�жϲ�����ض���------------
//ͨ��0��ͨ��4�ǹ���һ���жϺ��� ���ж��ڲ�ͨ����־λ�ж���˭�������ж�
#define ERU_CH0_CH4_INT_SERVICE IfxSrc_Tos_cpu0	//����ERUͨ��0��ͨ��4�жϷ������ͣ����ж�����˭��Ӧ���� 0:CPU0 1:CPU1 3:DMA  ��������Ϊ����ֵ
#define ERU_CH0_CH4_INT_PRIO  	40	//����ERUͨ��0��ͨ��4�ж����ȼ� ���ȼ���Χ1-255 Խ�����ȼ�Խ�� ��ƽʱʹ�õĵ�Ƭ����һ��

//ͨ��1��ͨ��5�ǹ���һ���жϺ��� ���ж��ڲ�ͨ����־λ�ж���˭�������ж�
#define ERU_CH1_CH5_INT_SERVICE IfxSrc_Tos_cpu0	//����ERUͨ��1��ͨ��5�жϷ������ͣ�ͬ��
#define ERU_CH1_CH5_INT_PRIO  	41	//����ERUͨ��1��ͨ��5�ж����ȼ� ͬ��

//ͨ��2��ͨ��6�ǹ���һ���жϺ��� ���ж��ڲ�ͨ����־λ�ж���˭�������ж�
#define ERU_CH2_CH6_INT_SERVICE IfxSrc_Tos_dma	//����ERUͨ��2��ͨ��6�жϷ������ͣ�ͬ��
#define ERU_CH2_CH6_INT_PRIO  	5	//����ERUͨ��2��ͨ��6�ж����ȼ� ͬ��

//ͨ��3��ͨ��7�ǹ���һ���жϺ��� ���ж��ڲ�ͨ����־λ�ж���˭�������ж�
#define ERU_CH3_CH7_INT_SERVICE IfxSrc_Tos_cpu0	//����ERUͨ��3��ͨ��7�жϷ������ͣ�ͬ��
#define ERU_CH3_CH7_INT_PRIO  	43	//����ERUͨ��3��ͨ��7�ж����ȼ� ͬ��


//------------DMA�жϲ�����ض���------------
#define	ERU_DMA_INT_SERVICE     IfxSrc_Tos_cpu0	//ERU����DMA�жϷ������ͣ����ж�����˭��Ӧ���� 0:CPU0 1:CPU1 3:DMA  ��������Ϊ����ֵ
#define ERU_DMA_INT_PRIO  	    60	//ERU����DMA�ж����ȼ� ���ȼ���Χ1-255 Խ�����ȼ�Խ�� ��ƽʱʹ�õĵ�Ƭ����һ��


//------------�����жϲ�����ض���------------
#define	UART0_INT_SERVICE       IfxSrc_Tos_cpu0	//���崮��0�жϷ������ͣ����ж�����˭��Ӧ���� 0:CPU0 1:CPU1 3:DMA  ��������Ϊ����ֵ
#define UART0_TX_INT_PRIO       10	//���崮��0�����ж����ȼ� ���ȼ���Χ1-255 Խ�����ȼ�Խ�� ��ƽʱʹ�õĵ�Ƭ����һ��
#define UART0_RX_INT_PRIO       11	//���崮��0�����ж����ȼ� ���ȼ���Χ1-255 Խ�����ȼ�Խ�� ��ƽʱʹ�õĵ�Ƭ����һ��
#define UART0_ER_INT_PRIO       12	//���崮��0�����ж����ȼ� ���ȼ���Χ1-255 Խ�����ȼ�Խ�� ��ƽʱʹ�õĵ�Ƭ����һ��

#define	UART1_INT_SERVICE       IfxSrc_Tos_cpu0
#define UART1_TX_INT_PRIO       13
#define UART1_RX_INT_PRIO       14
#define UART1_ER_INT_PRIO       15

#define	UART2_INT_SERVICE       IfxSrc_Tos_cpu0
#define UART2_TX_INT_PRIO       16
#define UART2_RX_INT_PRIO       17
#define UART2_ER_INT_PRIO       18

#define	UART3_INT_SERVICE       IfxSrc_Tos_cpu0
#define UART3_TX_INT_PRIO       19
#define UART3_RX_INT_PRIO       20
#define UART3_ER_INT_PRIO       21


#endif
