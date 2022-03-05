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


#ifndef _FM5807_H
#define _FM5807_H
#include "common.h"
#include "SEEKFREE_FONT.h"






#define RAD5807_DELAY_TIME 200


void rda5807_init(float freq);
void rda5807_set_transimt(void);
void rda5807_set_idle(void);
void rda5807_set_channel(float freq);
void rda5807_channel_config(uint16 reg_chn);
uint8 rda5807_read_rssi(void);


#endif


