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
 
 


#include "zf_gpio.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡGPIO����ַ
//  @param      pin         ѡ������� (��ѡ��Χ�� common.h ��PIN_enumö��ֵȷ��)
//  @return     void
//  Sample usage:           �ļ��ڲ�ʹ�ã��û��������
//-------------------------------------------------------------------------------------------------------------------
volatile Ifx_P* get_port(PIN_enum pin)
{
	volatile Ifx_P *port;

	switch(pin&0xffe0)
	{
		case P00_0:	port = &MODULE_P00;	break;
		case P02_0:	port = &MODULE_P02;	break;
		case P10_0:	port = &MODULE_P10;	break;
		case P11_0:	port = &MODULE_P11;	break;
		case P13_0:	port = &MODULE_P13;	break;
		case P14_0:	port = &MODULE_P14;	break;
		case P15_0:	port = &MODULE_P15;	break;
		case P20_0:	port = &MODULE_P20;	break;
		case P21_0:	port = &MODULE_P21;	break;
		case P22_0:	port = &MODULE_P22;	break;
		case P23_0:	port = &MODULE_P23;	break;
		case P32_0:	port = &MODULE_P32;	break;
		case P33_0:	port = &MODULE_P33;	break;
		default:break;
	}
#pragma warning 507
	return port;
#pragma warning default
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO��ʼ��
//  @param      pin         ѡ������� (��ѡ��Χ�� common.h ��PIN_enumö��ֵȷ��)
//  @param      dir         ���ŵķ���   �����GPO   ���룺GPI
//  @param      dat         ���ų�ʼ��ʱ���õĵ�ƽ״̬�����ʱ��Ч 0���͵�ƽ 1���ߵ�ƽ
//  @param      pinmode     �������ã������ò�����zf_gpio.h�ļ���GPIOMODE_enumö��ֵȷ����
//  @return     void
//  Sample usage:           gpio_init(P00_0, GPO, 1, PUSHPULL);//P00_0��ʼ��ΪGPIO���ܡ����ģʽ������ߵ�ƽ���������
//  @note					��Ҫ�ر�ע��P20_2�ǲ�����������ģ�����ֻ������Ĺ���
//-------------------------------------------------------------------------------------------------------------------
void gpio_init(PIN_enum pin, GPIODIR_enum dir, uint8 dat, GPIOMODE_enum pinmode)
{
	IfxPort_Mode port_mode;

	if(dir == GPI)
	{
		switch(pinmode)
		{
			case NO_PULL:  	port_mode = IfxPort_Mode_inputNoPullDevice; break;
			case PULLUP:   	port_mode = IfxPort_Mode_inputPullUp; 		break;
			case PULLDOWN: 	port_mode = IfxPort_Mode_inputPullDown; 	break;
			default:		port_mode = IfxPort_Mode_inputPullUp; 		break;
		}
	}
	else
	{
		switch(pinmode)
		{
			case OPENDRAIN:	port_mode = IfxPort_Mode_outputOpenDrainGeneral; break;//��©���
			default:		port_mode = IfxPort_Mode_outputPushPullGeneral;	 break;//Ĭ��Ϊ������
		}
	}

	IfxPort_setPinMode(get_port(pin), pin&0x1f, port_mode);

	IfxPort_setPinPadDriver(get_port(pin), pin&0x1f, IfxPort_PadDriver_cmosAutomotiveSpeed1);

	if(dir == GPO)
	{
		if(dat) IfxPort_setPinHigh(get_port(pin), pin&0x1f);
		else    IfxPort_setPinLow(get_port(pin), pin&0x1f);
	}

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO��������ģʽ
//  @param      pin         ѡ������� (��ѡ��Χ�� common.h ��PIN_enumö��ֵȷ��)
//  @param      pin_driver 	��������ģʽ����
//  @return     void
//  Sample usage:           gpio_pin_driver(P00_0, IfxPort_PadDriver_cmosAutomotiveSpeed1)
//-------------------------------------------------------------------------------------------------------------------
void gpio_pin_driver(PIN_enum pin, IfxPort_PadDriver pin_driver)
{
	IfxPort_setPinPadDriver(get_port(pin), pin&0x1f, pin_driver);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO�������
//  @param      pin         ѡ������� (��ѡ��Χ�� common.h ��PIN_enumö��ֵȷ��)
//  @param      dat         0���͵�ƽ 1���ߵ�ƽ
//  @return     void
//  Sample usage:           gpio_set(P00_0, 1);//P00_0 ����ߵ�ƽ
//-------------------------------------------------------------------------------------------------------------------
void gpio_set(PIN_enum pin, uint8 dat)
{
    if(dat) IfxPort_setPinHigh(get_port(pin), pin&0x1f);
    else    IfxPort_setPinLow(get_port(pin), pin&0x1f);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO״̬��ȡ
//  @param      pin         ѡ������� (��ѡ��Χ�� common.h ��PIN_enumö��ֵȷ��)
//  @return     uint8       0���͵�ƽ 1���ߵ�ƽ
//  Sample usage:           uint8 status = gpio_get(P00_0);//��ȡP00_0���ŵ�ƽ
//-------------------------------------------------------------------------------------------------------------------
uint8 gpio_get(PIN_enum pin)
{
    return IfxPort_getPinState(get_port(pin), pin&0x1f);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO��������
//  @param      pin         ѡ������� (��ѡ��Χ�� common.h ��PIN_enumö��ֵȷ��)
//  @param      dir         ���ŵķ���   �����GPO   ���룺GPI
//  @param      pinmode     �������ã������ò�����zf_gpio.h�ļ���GPIOMODE_enumö��ֵȷ����
//  @return     void        
//  Sample usage:           gpio_dir(P00_0, GPO, PUSHPULL);//����P00_0Ϊ�������ģʽ
//-------------------------------------------------------------------------------------------------------------------
void gpio_dir(PIN_enum pin, GPIODIR_enum dir, GPIOMODE_enum pinmode)
{
	IfxPort_Mode port_mode;

	if(dir == GPI)
	{
		switch(pinmode)
		{
			case NO_PULL:  	port_mode = IfxPort_Mode_inputNoPullDevice; break;
			case PULLUP:   	port_mode = IfxPort_Mode_inputPullUp; 		break;
			case PULLDOWN: 	port_mode = IfxPort_Mode_inputPullDown; 	break;
			default:		port_mode = IfxPort_Mode_inputPullUp; 		break;
		}
	}
	else
	{
		switch(pinmode)
		{
			case OPENDRAIN:	port_mode = IfxPort_Mode_outputOpenDrainGeneral; break;//��©���
			default:		port_mode = IfxPort_Mode_outputPushPullGeneral;	 break;//Ĭ��Ϊ������
		}
	}
	
	IfxPort_setPinMode(get_port(pin), pin&0x1f, port_mode);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      GPIO ��ת
//  @param      pin         ѡ������� (��ѡ��Χ�� common.h ��PIN_enumö��ֵȷ��)
//  @return     void        
//  Sample usage:           gpio_toggle(P00_0);//P00_0���ŵ�ƽ��ת
//-------------------------------------------------------------------------------------------------------------------
void gpio_toggle(PIN_enum pin)
{
	IfxPort_togglePin(get_port(pin), pin&0x1f);
}



