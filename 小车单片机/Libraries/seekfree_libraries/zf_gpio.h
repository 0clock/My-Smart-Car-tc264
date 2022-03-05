/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		gpio
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		ADS v1.2.2
 * @Target core		TC264D
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-3-23
 ********************************************************************************************************************/
 
#ifndef _zf_gpio_h
#define _zf_gpio_h

#include "common.h"
#include "IFXPORT.h"




typedef enum    // ö��������ѡ��
{
    NO_PULL,	//������������
    PULLUP,		//��������
	PULLDOWN,	//��������
	PUSHPULL,	//�������
	OPENDRAIN,	//��©���
}GPIOMODE_enum;


#define GET_PORT_IN_ADDR(pin)	(uint8 *)(&IfxPort_getAddress((IfxPort_Index)(pin/32))->IN + pin%32/8)



//��Ҫ�ر�ע��P20_2�ǲ�����������ģ�����ֻ������Ĺ���
//��Ҫ�ر�ע��P20_2�ǲ�����������ģ�����ֻ������Ĺ���
//��Ҫ�ر�ע��P20_2�ǲ�����������ģ�����ֻ������Ĺ���




//------------------------------------------------------
//ͨ��GPIO����
void gpio_init(PIN_enum pin, GPIODIR_enum dir, uint8 dat, GPIOMODE_enum pinconf);
void gpio_pin_driver(PIN_enum pin, IfxPort_PadDriver pin_driver);
void gpio_set(PIN_enum pin, uint8 dat);
uint8 gpio_get(PIN_enum pin);
void gpio_dir(PIN_enum pin, GPIODIR_enum dir, GPIOMODE_enum pinconf);
void gpio_toggle(PIN_enum pin);






#endif
