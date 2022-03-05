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

//#include "zf_iic.h"
#include "zf_stm_systick.h"
#include "SEEKFREE_IIC.h"
#include "SEEKFREE_MPU6050.h"

int16 mpu_gyro_x,mpu_gyro_y,mpu_gyro_z;
int16 mpu_acc_x,mpu_acc_y,mpu_acc_z;





//-------------------------------------------------------------------------------------------------------------------
//  @brief      MPU6050�Լ캯��
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:				���øú���ǰ�����ȵ���ģ��IIC�ĳ�ʼ��
//-------------------------------------------------------------------------------------------------------------------
void mpu6050_self1_check(void)
{
    simiic_write_reg(MPU6050_DEV_ADDR, PWR_MGMT_1, 0x00);	//�������״̬
    simiic_write_reg(MPU6050_DEV_ADDR, SMPLRT_DIV, 0x07);   //125HZ������
    while(0x07 != simiic_read_reg(MPU6050_DEV_ADDR, SMPLRT_DIV,SIMIIC))
    {
        //��������ԭ�������¼���
        //1 MPU6050���ˣ�������µ������ĸ��ʼ���
        //2 ���ߴ������û�нӺ�
        //3 ��������Ҫ����������裬������3.3V
		//4 ����û�е���ģ��IIC�ĳ�ʼ������
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ʼ��MPU6050
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:				���øú���ǰ�����ȵ���ģ��IIC�ĳ�ʼ��
//-------------------------------------------------------------------------------------------------------------------
void mpu6050_init(void)
{
	simiic_init();
    systick_delay_ms(STM0, 100);                                   //�ϵ���ʱ

    mpu6050_self1_check();
    simiic_write_reg(MPU6050_DEV_ADDR, PWR_MGMT_1, 0x00);	//�������״̬
    simiic_write_reg(MPU6050_DEV_ADDR, SMPLRT_DIV, 0x07);   //125HZ������
    simiic_write_reg(MPU6050_DEV_ADDR, MPU6050_CONFIG, 0x04);       //
    simiic_write_reg(MPU6050_DEV_ADDR, GYRO_CONFIG, 0x18);  //2000
    simiic_write_reg(MPU6050_DEV_ADDR, ACCEL_CONFIG, 0x10); //8g
	simiic_write_reg(MPU6050_DEV_ADDR, User_Control, 0x00);
    simiic_write_reg(MPU6050_DEV_ADDR, INT_PIN_CFG, 0x02);
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡMPU6050���ٶȼ�����
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
//-------------------------------------------------------------------------------------------------------------------
void get_accdata(void)
{
    uint8 dat[6];

    simiic_read_regs(MPU6050_DEV_ADDR, ACCEL_XOUT_H, dat, 6, SIMIIC);  
    mpu_acc_x = (int16)(((uint16)dat[0]<<8 | dat[1]));
    mpu_acc_y = (int16)(((uint16)dat[2]<<8 | dat[3]));
    mpu_acc_z = (int16)(((uint16)dat[4]<<8 | dat[5]));
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡMPU6050����������
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
//-------------------------------------------------------------------------------------------------------------------
void get_gyro(void)
{
    uint8 dat[6];

    simiic_read_regs(MPU6050_DEV_ADDR, GYRO_XOUT_H, dat, 6, SIMIIC);  
    mpu_gyro_x = (int16)(((uint16)dat[0]<<8 | dat[1]));
    mpu_gyro_y = (int16)(((uint16)dat[2]<<8 | dat[3]));
    mpu_gyro_z = (int16)(((uint16)dat[4]<<8 | dat[5]));
}

//-------------------------------------------------------------------------------------------------------------------
//  ���Ϻ�����ʹ�����IICͨ�ţ���Ƚ�Ӳ��IIC�����IIC���Ÿ���������ʹ��������ͨIO
//-------------------------------------------------------------------------------------------------------------------





////-------------------------------------------------------------------------------------------------------------------
////  ���º�����ʹ��Ӳ��IICͨ�ţ���Ƚ����IIC��Ӳ��IIC�ٶȿ����������졣
////-------------------------------------------------------------------------------------------------------------------
//
//#define IIC_NUM         IIC_1
//#define IIC_SDA_PIN     IIC1_SDA_B17
//#define IIC_SCL_PIN     IIC1_SCL_B16
////-------------------------------------------------------------------------------------------------------------------
////  @brief      MPU6050�Լ캯��
////  @param      NULL
////  @return     void
////  @since      v1.0
////  Sample usage:
////-------------------------------------------------------------------------------------------------------------------
//void mpu6050_self2_check(void)
//{
//    uint8 dat=0;
//
//    iic_write_reg(IIC_NUM, MPU6050_DEV_ADDR, PWR_MGMT_1, 0x00);	//�������״̬
//    iic_write_reg(IIC_NUM, MPU6050_DEV_ADDR, SMPLRT_DIV, 0x07); //125HZ������
//    while(0x07 != dat)
//    {
//        iic_read_reg(IIC_NUM, MPU6050_DEV_ADDR, SMPLRT_DIV, &dat);
//        systick_delay_ms(STM0, 10);
//        //��������ԭ�������¼���
//        //1 MPU6050���ˣ�������µ������ĸ��ʼ���
//        //2 ���ߴ������û�нӺ�
//        //3 ��������Ҫ����������裬������3.3V
//    }
//}
//
////-------------------------------------------------------------------------------------------------------------------
////  @brief      ��ʼ��MPU6050
////  @param      NULL
////  @return     void
////  @since      v1.0
////  Sample usage:
////-------------------------------------------------------------------------------------------------------------------
//void mpu6050_init_hardware(void)
//{
//    systick_delay_ms(STM0, 100);                                      //�ϵ���ʱ
//    iic_init(IIC_NUM, IIC_SDA_PIN, IIC_SCL_PIN,400*1000);       //Ӳ��IIC��ʼ��     ������400K
//
//    mpu6050_self2_check();
//    iic_write_reg(IIC_NUM,MPU6050_DEV_ADDR, PWR_MGMT_1, 0x00);	//�������״̬
//    iic_write_reg(IIC_NUM,MPU6050_DEV_ADDR, SMPLRT_DIV, 0x07);    //125HZ������
//    iic_write_reg(IIC_NUM,MPU6050_DEV_ADDR, MPU6050_CONFIG, 0x04);        //
//    iic_write_reg(IIC_NUM,MPU6050_DEV_ADDR, GYRO_CONFIG, 0x18);   //2000
//    iic_write_reg(IIC_NUM,MPU6050_DEV_ADDR, ACCEL_CONFIG, 0x10);  //8g
//	iic_write_reg(IIC_NUM,MPU6050_DEV_ADDR, User_Control, 0x00);
//    iic_write_reg(IIC_NUM,MPU6050_DEV_ADDR, INT_PIN_CFG, 0x02);
//}
//
////-------------------------------------------------------------------------------------------------------------------
////  @brief      ��ȡMPU6050���ٶȼ�����
////  @param      NULL
////  @return     void
////  @since      v1.0
////  Sample usage:				ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
////-------------------------------------------------------------------------------------------------------------------
//void get_accdata_hardware(void)
//{
//    uint8 dat[6];
//
//    iic_read_reg_bytes(IIC_NUM,MPU6050_DEV_ADDR, ACCEL_XOUT_H, dat, 6);
//    mpu_acc_x = (int16)(((uint16)dat[0]<<8 | dat[1]));
//    mpu_acc_y = (int16)(((uint16)dat[2]<<8 | dat[3]));
//    mpu_acc_z = (int16)(((uint16)dat[4]<<8 | dat[5]));
//}
//
////-------------------------------------------------------------------------------------------------------------------
////  @brief      ��ȡMPU6050����������
////  @param      NULL
////  @return     void
////  @since      v1.0
////  Sample usage:				ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
////-------------------------------------------------------------------------------------------------------------------
//void get_gyro_hardware(void)
//{
//    uint8 dat[6];
//
//    iic_read_reg_bytes(IIC_NUM,MPU6050_DEV_ADDR, GYRO_XOUT_H, dat, 6);
//    mpu_gyro_x = (int16)(((uint16)dat[0]<<8 | dat[1]));
//    mpu_gyro_y = (int16)(((uint16)dat[2]<<8 | dat[3]));
//    mpu_gyro_z = (int16)(((uint16)dat[4]<<8 | dat[5]));
//}








