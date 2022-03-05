/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2021,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		��ɿƼ�����ת����ģ��
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.33
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2021-08-27
 * @note		
					���߶��壺
					------------------------------------ 
					    ����ת����      ��Ƭ��                        
    					RX              �鿴SEEKFREE_BLUETOOTH_CH9141.h�ļ��е�BLUETOOTH_CH9141_UART_TX�궨��
    					TX              �鿴SEEKFREE_BLUETOOTH_CH9141.h�ļ��е�BLUETOOTH_CH9141_UART_RX�궨��
    					RTS             �鿴SEEKFREE_BLUETOOTH_CH9141.h�ļ��е�BLUETOOTH_CH9141_RTS_PIN�궨��
                        CTS             ����
    					CMD             ���ջ�������
					------------------------------------ 
 ********************************************************************************************************************/
 
#ifndef SEEKFREE_BLUETOOTH_CH9141_H
#define SEEKFREE_BLUETOOTH_CH9141_H

#include "common.h"


#define BLUETOOTH_CH9141_UART           UART_2         //����ת����ģ�� ��ʹ�õ��Ĵ���
#define BLUETOOTH_CH9141_UART_TX        UART2_TX_P10_5    //������RX���ӵ�Ƭ����D16����
#define BLUETOOTH_CH9141_UART_RX        UART2_RX_P10_6    //������TX���ӵ�Ƭ����D17����
#define BLUETOOTH_CH9141_UART_BAUD      115200

#define BLUETOOTH_CH9141_RTS_PIN        P10_2             //��������λ����  ָʾ��ǰģ���Ƿ���Խ�������  0���Լ�������  1�����Լ�������


typedef enum
{
    TX_POWER_0DB = 0,   //0DB
    TX_POWER_1DB,       //1DB
    TX_POWER_2DB,       //2DB
    TX_POWER_3DB,       //3DB
    TX_POWER_4DB,       //4DB
    TX_POWER_MINUS_8DB, //-8DB
    TX_POWER_MINUS_14DB,//-14DB
    TX_POWER_MINUS_20DB,//-20DB
}CH9141_TX_POWEER_enum; 

typedef enum
{
    MASTER_NO_INIT = 0,        //����δ��ʼ��
    MASTER_SCANNING,           //����ɨ����
    MASTER_CONNECTING,         //����������
    MASTER_CONNECTED,          //����������
    MASTER_DISCONNECTED,       //�������ӶϿ�
    
    SLAVE_NO_INIT,         //�ӻ�δ��ʼ��
    SLAVE_OK_INIT,         //�ӻ���ɳ�ʼ��
    SLAVE_BROADCAST,       //�ӻ��㲥��
    SLAVE_BROADCAST_READY, //�ӻ�׼���㲥
    SLAVE_CONNECT_TIMEOUT, //�ӻ����ӳ�ʱ
    SLAVE_CONNECTED,       //�ӻ�������
    SLAVE_ERROR,           //�ӻ����ִ���
}CH9141_STATUS_enum; 

typedef enum
{
    BROADCAST_MODE = 0, //�㲥ģʽ
    MASTER_MODE,       //����ģʽ
    SLAVE_MODE,    //�ӻ�ģʽ
}CH9141_MODE_enum; 

extern uint8 uart_flag;
extern uint8 uart_data;
extern uint8 mac_address[17];      //����mac��ַ

void bluetooth_ch9141_uart_callback(void);
void bluetooth_ch9141_enter_at_mode(void);
void bluetooth_ch9141_exit_at_mode(void);
void bluetooth_ch9141_reset(void);
void bluetooth_ch9141_get_mac_address(void);
int16 bluetooth_ch9141_get_rssi(void);
uint32 bluetooth_ch9141_send_buff(uint8 *buff, uint32 len);
void bluetooth_ch9141_init(CH9141_MODE_enum mode, int8 *salve_mac_password);









#endif
