/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		MPU6050
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
						���IIC
                        SCL                 �鿴SEEKFREE_IIC�ļ��ڵ�SEEKFREE_SCL�궨��
						SDA                 �鿴SEEKFREE_IIC�ļ��ڵ�SEEKFREE_SDA�궨��

                        Ӳ��IIC
                        SCL                 �鿴init_mpu6050_hardware������IIC��ʼ��ʱ��������
						SDA                 �鿴init_mpu6050_hardware������IIC��ʼ��ʱ��������
					------------------------------------
					ע�����RDA5807��SCL��SDA����Ҫ����1K����Ȼ�����뵥Ƭ�����ӣ���������޷�ͨ��
 ********************************************************************************************************************/



#include "SEEKFREE_IIC.h"
#include "zf_stm_systick.h"
#include "SEEKFREE_RDA5807.h"


#define ack 1      //��Ӧ��
#define no_ack 0   //��Ӧ��


#define FM_ADDRESS	0x20>>1

uint8 rda5807_config_reg[] = {
    0xc0, 0x03,                             // Register 0x2
    0x00, 0x00,                             // Register 0x3
    0x0a, 0x00,        	                    // Register 0x4
    0x88, 0x0f,                             // Register 0x5
    0x00, 0x00,                             // Register 0x6
    0x42, 0x02,                             // Register 0x7
};

//-------------------------------------------------------------------------------------------------------------------
//  @brief      rda5807ģ��IIC��ȡ���ֽ�����
//  @param      dev_add			�豸��ַ(����λ��ַ)
//  @param      data[]			���ݱ���ĵ�ַָ��
//  @param      num				��ȡ�ֽ�����
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void rad5807m_simiic_write(uint8 dev_add, uint8 data[], uint8 num)
{
   	uint8 i;

    uint16 temp_delay_time = 0;

    temp_delay_time = simiic_delay_time;
	simiic_delay_set(RAD5807_DELAY_TIME);//����ģ��IIC��ʱ����ΪĬ�����ʽϸ�

	simiic_start();
    send_ch( (dev_add<<1) | 0x00);   		//����������ַ��дλ

	for(i=0;i<num;i++)
		send_ch(data[i]);   				 //������Ҫд�������

	simiic_stop();

    simiic_delay_set(temp_delay_time);  //��ԭģ��IICĬ������
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ģ��IIC��ȡ���ֽ�����
//  @param      dev_add			�豸��ַ(����λ��ַ)
//  @param      dat_add			���ݱ���ĵ�ַָ��
//  @param      num				��ȡ�ֽ�����
//  @return     uint8			���ؼĴ���������
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void rda5807m_simiic_read(uint8 dev_add,uint8 *dat_add,uint8 num)
{

    uint16 temp_delay_time = 0;
    temp_delay_time = simiic_delay_time;
	simiic_delay_set(RAD5807_DELAY_TIME);//����ģ��IIC��ʱ����ΪĬ�����ʽϸ�

    simiic_start();

	send_ch( (dev_add<<1) | 0x01);  //����������ַ�Ӷ�λ
    while(--num)
    {
        *dat_add = read_ch(ack); //��ȡ����
        dat_add++;

    }
    *dat_add = read_ch(no_ack); //��ȡ����

	simiic_stop();

    simiic_delay_set(temp_delay_time);  //��ԭģ��IICĬ������
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      Ƶ�ʼĴ������ã��ڲ�ʹ�ã��û�������ģ�
//  @param      channel         	Ƶ�ʼĴ���ֵ
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void rda5807_channel_config(uint16 reg_chn)
{
	rda5807_config_reg[0] = 0xc0;
	rda5807_config_reg[1] = 0x01;
	rda5807_config_reg[2] = (uint8)(reg_chn >> 2);
	rda5807_config_reg[3] = (uint8)(((reg_chn & 0x3) << 6) | 0x18);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      rda5807����Ƶ��	���ڲ�ʹ�ã��û�������ģ�
//  @param      freq         Ƶ��ֵ
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void rda5807_set_channel(float freq)
{
	uint16 reg_chn=0;
	reg_chn = (int)((freq - 76.0) * 10.0 + 0.5);
    rda5807_channel_config(reg_chn);
    rad5807m_simiic_write(FM_ADDRESS,rda5807_config_reg, 4);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      rda5807����Ϊ����ģʽ
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void rda5807_set_idle(void)
{
	rda5807_config_reg[1] &= ~(1<<0);//Power Up Disabled.
	rad5807m_simiic_write(FM_ADDRESS,rda5807_config_reg, 4);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      rda5807����Ϊ����ģʽ
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void rda5807_set_transimt(void)
{
	rda5807_config_reg[1] |= 1<<0;//Power Up Enable.
	rad5807m_simiic_write(FM_ADDRESS,rda5807_config_reg, 4);
}






uint8  rda5807_read_reg[10];
//-------------------------------------------------------------------------------------------------------------------
//  @brief      rda5807��ȡID���ڲ�ʹ�ã��û�������ģ�
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void rda5807_read_id(void)
{
	rda5807m_simiic_read(FM_ADDRESS,rda5807_read_reg,10);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      rda5807��ȡRSSI(�ź�ǿ��)
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint8 rda5807_read_rssi(void)
{
	uint8 rssi;

	rda5807m_simiic_read(FM_ADDRESS,rda5807_read_reg,3);
	rssi = rda5807_read_reg[2] >> 1;

	return rssi;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      rda5807��ʼ��
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void rda5807_init(float freq)
{
	uint8 dat[2] = {0x00,0x02};


	//iic��ʼ��
	simiic_init();

	systick_delay_ms(STM0, 1000);//��Ҫ��֤�ϵ��1S�ڲ��� FMģ��

	while(rda5807_read_reg[8] != 0x58)
	{
		//��λ
		rad5807m_simiic_write(FM_ADDRESS,dat,2);
		systick_delay_ms(STM0, 10);
		//��ȡ�Ĵ������Լ졣
		rda5807m_simiic_read(FM_ADDRESS,rda5807_read_reg,10);
		//���һֱ��������鿴SCL��SDA�Ƿ����Ӻá�
		//Ҳ���Գ���  �ʵ�����5807.h�ļ��е�RAD5807_DELAY_TIME�궨�����ֵ
		//���SDA SCLû�д������裨����������Ŷ������Գ����޸ĵײ���ļ�
		//��gpio_init�����е�IfxPort_PadDriver_cmosAutomotiveSpeed1��ΪIfxPort_PadDriver_cmosAutomotiveSpeed4
	}

	//����
	rad5807m_simiic_write(FM_ADDRESS,rda5807_config_reg,sizeof(rda5807_config_reg));
	//Ƶ������
	rda5807_set_channel(freq);


}




