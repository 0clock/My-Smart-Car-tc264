/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		����ת����ģ��
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		ADS v1.2.2
 * @Target core		TC264D
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-3-23
 * @note		
					���߶��壺
					------------------------------------ 
					    ����ת����       ��Ƭ��                        
    					RX              �鿴SEEKFREE_WIRELESS.h�ļ��е�WIRELESS_UART_TX�궨��
    					TX              �鿴SEEKFREE_WIRELESS.h�ļ��е�WIRELESS_UART_RX�궨��
    					RTS             �鿴SEEKFREE_WIRELESS.h�ļ��е�RTS_PIN�궨��
    					CMD             ���ջ�������
					------------------------------------ 
 ********************************************************************************************************************/

#ifndef _SEEKFREE_WIRELESS
#define _SEEKFREE_WIRELESS


#include "common.h"


#define WIRELESS_UART        UART_2         //����ת����ģ�� ��ʹ�õ��Ĵ���
#define WIRELESS_UART_TX     UART2_TX_P10_5
#define WIRELESS_UART_RX     UART2_RX_P10_6
#define WIRELESS_UART_BAUD   115200

// ------------------------------------ �Զ������� ------------------------------------
// ע������1������ת����ģ��汾��V2.0���µ����޷������Զ������ʵġ�
// ע������2�������Զ��������������RTS���ţ�����Ὺ��ʧ�ܡ�
// ע������3��ģ���Զ�������ʧ�ܵĻ������Գ��Զϵ�����

// �����Զ�����������Ķ��������� ע������
// �����Զ�����������Ķ��������� ע������
// �����Զ�����������Ķ��������� ע������

// 0���ر��Զ�������
// 1�������Զ������� �Զ������ʵ��������޸�WIRELESS_UART_BAUD֮����Ҫ��ģ��������ã�ģ����Զ�����Ϊ��Ӧ�Ĳ�����

#define WIRELESS_AUTO_UART_BAUD 0
// ------------------------------------ �Զ������� ------------------------------------

#define RTS_PIN P10_2 //��������λ����  ָʾ��ǰģ���Ƿ���Խ�������  0���Լ�������  1�����Լ�������
#define WIRELESS_BUFFER_SIZE    16

extern uint8 wireless_send_buffer[WIRELESS_BUFFER_SIZE];
extern uint32 wireless_rx_index;

void 	wireless_uart_callback(void);
void    seekfree_wireless_init(void);
uint32  seekfree_wireless_send_buff(uint8 *buff, uint32 len);

#endif 
