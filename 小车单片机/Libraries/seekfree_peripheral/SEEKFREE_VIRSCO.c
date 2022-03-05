/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		����ʾ����Э��
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 * @note		    ʾ�������ص�ַ��https://pan.baidu.com/s/198CMXTZsbI3HAEqNXDngBw�������廪ֱ���������ϣ�
 ********************************************************************************************************************/



#include "zf_uart.h"
#include "SEEKFREE_VIRSCO.h"


uint8 virtual_scope_data[10];

//-------------------------------------------------------------------------------------------------------------------
//  @brief      CRCУ�飨�ڲ�ʹ�ã��û�������ģ�
//  @param      Buf             ��Ҫ����CRC��������ݵ�ַ
//  @param      CRC_CNT         ��Ҫ����CRC��������ݸ���
//  @return     void		
//  @since      v1.0		
//  Sample usage:			
//-------------------------------------------------------------------------------------------------------------------
uint16 CRC_CHECK(uint8 *Buf, uint8 CRC_CNT)
{
    uint16 CRC_Temp;
    uint8 i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++){      
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++) {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ʾ��������ת������
//  @param      data1       Ҫ���͵ĵ�һ������
//  @param      data2       Ҫ���͵ĵڶ�������
//  @param      data3       Ҫ���͵ĵ���������
//  @param      data4       Ҫ���͵ĵ��ĸ�����
//  @param      *dat        ת��֮�������ݵĵ�ַ
//  @return     void		
//  @since      v1.0		
//  Sample usage:			
//-------------------------------------------------------------------------------------------------------------------
void data_conversion(int16 data1, int16 data2, int16 data3, int16 data4, uint8 *dat)
{
    uint16 CRC16 = 0;
    
    dat[0] = (uint8)((uint16)data1&0xff);
    dat[1] = (uint8)((uint16)data1>>8);
    
    dat[2] = (uint8)((uint16)data2&0xff);
    dat[3] = (uint8)((uint16)data2>>8);
    
    dat[4] = (uint8)((uint16)data3&0xff);
    dat[5] = (uint8)((uint16)data3>>8);
    
    dat[6] = (uint8)((uint16)data4&0xff);
    dat[7] = (uint8)((uint16)data4>>8);

    CRC16  = CRC_CHECK(dat,8);
    dat[8] = (uint8)(CRC16&0xff);
    dat[9] = (uint8)(CRC16>>8);
    
    //uart_putbuff(UART_0,dat,10);  //����ת����ɺ�ʹ�ô��ڷ��ͽ���������ݷ��ͳ�ȥ
}

