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
 ********************************************************************************************************************/



#ifndef _SEEKFREE_MPU6050_h
#define _SEEKFREE_MPU6050_h

#include "common.h"



// ����MPU6050�ڲ���ַ

#define	MPU6050_DEV_ADDR	    0xD0>>1	//IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ

#define	SMPLRT_DIV				0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	MPU6050_CONFIG          0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG				0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define	ACCEL_CONFIG	        0x1C	//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
#define INT_PIN_CFG	  			0x37    //����6050����I2CΪֱͨģʽ�Ĵ���
#define	ACCEL_XOUT_H	        0x3B
#define	ACCEL_XOUT_L	        0x3C
#define	ACCEL_YOUT_H	        0x3D
#define	ACCEL_YOUT_L	        0x3E
#define	ACCEL_ZOUT_H	        0x3F
#define	ACCEL_ZOUT_L	        0x40
#define	GYRO_XOUT_H				0x43
#define	GYRO_XOUT_L				0x44	
#define	GYRO_YOUT_H				0x45
#define	GYRO_YOUT_L				0x46
#define	GYRO_ZOUT_H				0x47
#define	GYRO_ZOUT_L				0x48
#define User_Control 			0x6A    //�ر�6050�Ը���I2C�豸�Ŀ���
#define	PWR_MGMT_1				0x6B	//��Դ��������ֵ��0x00(��������)
#define	WHO_AM_I				0x75	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)




extern int16 mpu_gyro_x,mpu_gyro_y,mpu_gyro_z;
extern int16 mpu_acc_x,mpu_acc_y,mpu_acc_z;

void  mpu6050_init(void);               //��ʼ��MPU6050
void  get_accdata(void);
void  get_gyro(void);

void  mpu6050_init_hardware(void);
void  get_accdata_hardware(void);
void  get_gyro_hardware(void);



#endif
