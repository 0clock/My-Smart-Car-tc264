/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		L3G4200D
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
						SCL                 �鿴SEEKFREE_SIMIIC�ļ��ڵ�SEEKFREE_SCL�궨��
						SDA                 �鿴SEEKFREE_SIMIIC�ļ��ڵ�SEEKFREE_SDA�궨��
					------------------------------------ 
 ********************************************************************************************************************/



#include "zf_stm_systick.h"
#include "SEEKFREE_IIC.h"
#include "SEEKFREE_L3G4200D.h"

int16 gyro_x = 0, gyro_y = 0, gyro_z = 0;

uint8 temperature = 0;


//-------------------------------------------------------------------------------------------------------------------
//  @brief      L3G4200D�Լ캯��
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:				���øú���ǰ�����ȵ���ģ��IIC�ĳ�ʼ��
//-------------------------------------------------------------------------------------------------------------------
void l3g4200d_self_check(void)
{
    simiic_write_reg(L3G4200_DEV_ADD, CTRL_REG1_4200, 0x4f);
    while(0x4f != simiic_read_reg(L3G4200_DEV_ADD, CTRL_REG1_4200,SIMIIC))
    {
        //��������ԭ�������¼���
        //1 ģ�黵�ˣ�������µ������ĸ��ʼ���
        //2 ���ߴ������û�нӺ�
        //3 ��������Ҫ����������裬������3.3V
		//4 ����û�е���ģ��IIC�ĳ�ʼ������
    }
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ʼ��L3G4200D������
//  @param      NULL
//  @return     void						
//  @since      v1.0
//  Sample usage:				���øú���ǰ�����ȵ���ģ��SIMIIC�ĳ�ʼ��
//-------------------------------------------------------------------------------------------------------------------
void l3g4200d_init(void)
{
	simiic_init();
    systick_delay_ms(STM0, 50);
    
    l3g4200d_self_check();
    simiic_write_reg(L3G4200_DEV_ADD, CTRL_REG1_4200, 0x4f);   //
    simiic_write_reg(L3G4200_DEV_ADD, CTRL_REG2_4200, 0x00);   //00
    simiic_write_reg(L3G4200_DEV_ADD, CTRL_REG3_4200, 0x08);   //
    simiic_write_reg(L3G4200_DEV_ADD, CTRL_REG4_4200, 0xB0);   //+-2000dps
    simiic_write_reg(L3G4200_DEV_ADD, CTRL_REG5_4200, 0x00);
    temperature = simiic_read_reg(L3G4200_DEV_ADD, OUT_TEMP_4200, SIMIIC);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡL3G4200D����������
//  @param      NULL
//  @return     void						
//  @since      v1.0
//  Sample usage:				ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
//-------------------------------------------------------------------------------------------------------------------
void get_l3g4200d(void)
{
    uint8 dat[6];
    uint8 status;
    status = simiic_read_reg(L3G4200_DEV_ADD, STATUS_REG_4200, SIMIIC);

    if( status&0x80 )//�鿴�����Ƿ�׼�����ˡ����û��׼���ã���ȡ����ܶ�ȡ���������ݡ���ȡƵ������Ǹ����������Ƶ��
    {
        simiic_read_regs(L3G4200_DEV_ADD, OUT_X_L_4200|0X80, dat, 6, SIMIIC);  //����0x80����Ϊ��Ҫ���ֽڶ�ȡ����
        gyro_x = (int16)((uint16)dat[1]<<8 | dat[0]);                   
        gyro_y = (int16)((uint16)dat[3]<<8 | dat[2]);
        gyro_z = (int16)((uint16)dat[5]<<8 | dat[4]);
    }
}

