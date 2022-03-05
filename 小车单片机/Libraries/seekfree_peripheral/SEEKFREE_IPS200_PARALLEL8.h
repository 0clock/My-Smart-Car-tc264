
/*/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		2.0��IPS��Ļ
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
						ģ��ܽ�            	��Ƭ���ܽ�
						D0       		�鿴SEEKFREE_IPS200_PARALLEL8.H�ļ��ڵ�IPS200_D0_PIN �궨��
						D1             	�鿴SEEKFREE_IPS200_PARALLEL8.H�ļ��ڵ�IPS200_D1_PIN �궨��
						D2            	�鿴SEEKFREE_IPS200_PARALLEL8.H�ļ��ڵ�IPS200_D2_PIN �궨��
						D3             	�鿴SEEKFREE_IPS200_PARALLEL8.H�ļ��ڵ�IPS200_D3_PIN �궨��
						D4            	�鿴SEEKFREE_IPS200_PARALLEL8.H�ļ��ڵ�IPS200_D4_PIN �궨��
						D5         		�鿴SEEKFREE_IPS200_PARALLEL8.H�ļ��ڵ�IPS200_D5_PIN �궨��
						D6           	�鿴SEEKFREE_IPS200_PARALLEL8.H�ļ��ڵ�IPS200_D6_PIN �궨��
						D7           	�鿴SEEKFREE_IPS200_PARALLEL8.H�ļ��ڵ�IPS200_D7_PIN �궨��
                                                                                            
						BL          	�鿴SEEKFREE_IPS200_PARALLEL8.H�ļ��ڵ�IPS200_BL_PIN �궨��
						CS            	�鿴SEEKFREE_IPS200_PARALLEL8.H�ļ��ڵ�IPS200_CS_PIN �궨��
						RD            	�鿴SEEKFREE_IPS200_PARALLEL8.H�ļ��ڵ�IPS200_RD_PIN �궨��
						WR           	�鿴SEEKFREE_IPS200_PARALLEL8.H�ļ��ڵ�IPS200_WR_PIN �궨��
						RS          	�鿴SEEKFREE_IPS200_PARALLEL8.H�ļ��ڵ�IPS200_RS_PIN �궨��
						RST            	�鿴SEEKFREE_IPS200_PARALLEL8.H�ļ��ڵ�IPS200_RST_PIN �궨��

						��Դ����
						VCC 3.3V��Դ
						GND ��Դ��
						���ֱ���240*320
					------------------------------------ 

 ********************************************************************************************************************/

#ifndef _SEEKFREE_IPS200PARALLEL8_H
#define _SEEKFREE_IPS200PARALLEL8_H

#include "common.h"
#include "SEEKFREE_FONT.h"

     
//-------������ɫ��SEEKFREE_FONT.h�ļ��ж���----------
//#define RED          	    0xF800	//��ɫ
//#define BLUE         	    0x001F  //��ɫ
//#define YELLOW       	    0xFFE0	//��ɫ
//#define GREEN        	    0x07E0	//��ɫ
//#define WHITE        	    0xFFFF	//��ɫ
//#define BLACK        	    0x0000	//��ɫ 
//#define GRAY  			0X8430 	//��ɫ
//#define BROWN 			0XBC40 	//��ɫ
//#define PURPLE    		0XF81F	//��ɫ
//#define PINK    		    0XFE19	//��ɫ


#define IPS200_BGCOLOR      WHITE	//������ɫ
#define IPS200_PENCOLOR     RED		//������ɫ


//����ֻ��ѡ��B C D�������˿��е����ţ�����ʹ�ÿ���GPIO�������ʾ�ٶ�
#define IPS200_RD_PIN  		P15_3
#define IPS200_WR_PIN  		P15_5
#define IPS200_RS_PIN  		P15_1
#define IPS200_RST_PIN 		P15_0
#define IPS200_CS_PIN  		P15_2
#define IPS200_BL_PIN  		P15_4





//D0-D3�ĸ��������ű������� ����C0-C3,C1-C4�ȵȣ�
//D4-D7�ĸ��������ű������� ����B0-B3,B1-B4�ȵȡ�
//�������ӵ���ͬ�˿ڵ���˼������Ļ��D0-D3��C1-C4���ӣ�D4-D7��B2-B5���ӡ�
//�л����ź�ע���޸�IPS200_DATA_PORT1��IPS200_DATA_PORT2�궨��
#define IPS200_D0_PIN 		P11_9
#define IPS200_D1_PIN 		P11_10
#define IPS200_D2_PIN 		P11_11
#define IPS200_D3_PIN 		P11_12
#define IPS200_D4_PIN 		P13_0
#define IPS200_D5_PIN 		P13_1
#define IPS200_D6_PIN 		P13_2
#define IPS200_D7_PIN 		P13_3

//�������ݶ˿�����PORT���л����ź���ظ�����������PORT���и���
#define IPS200_DATA_PORT1    3       //0��P00�˿�  1��P02�˿�  2��P10�˿�  3��P11�˿�  4��P13�˿�  5��P14�˿�  6��P15�˿�  7��P20�˿�  8��P21�˿�  9��P22�˿�  10��P23�˿�  11��P32�˿�  12��P33�˿�

#define IPS200_DATAPORT1 	(((Ifx_P *)IfxPort_cfg_indexMap[IPS200_DATA_PORT1].module)->OUT.U)	    //�궨�����ݿ�GPIO������Ĵ���
#define DATA_START_NUM1		(IPS200_D0_PIN&0x1f)  //�궨���������ŵ���ʼ���


#define IPS200_DATA_PORT2    4       //0��P00�˿�  1��P02�˿�  2��P10�˿�  3��P11�˿�  4��P13�˿�  5��P14�˿�  6��P15�˿�  7��P20�˿�  8��P21�˿�  9��P22�˿�  10��P23�˿�  11��P32�˿�  12��P33�˿�
                            
#define IPS200_DATAPORT2 	(((Ifx_P *)IfxPort_cfg_indexMap[IPS200_DATA_PORT2].module)->OUT.U)		//�궨�����ݿ�GPIO������Ĵ���
#define DATA_START_NUM2		(IPS200_D4_PIN&0x1f)  //�궨���������ŵ���ʼ���









#define IPS200_BL(x)		gpio_set(IPS200_BL_PIN,x)
#define IPS200_CS(x)       	gpio_set(IPS200_CS_PIN,x)
#define IPS200_RD(x)       	gpio_set(IPS200_RD_PIN,x)
#define IPS200_WR(x)       	gpio_set(IPS200_WR_PIN,x)
#define IPS200_RS(x)       	gpio_set(IPS200_RS_PIN,x)
#define IPS200_RST(x)      	gpio_set(IPS200_RST_PIN,x)


#define IPS200_W 240
#define IPS200_H 320


//������ʾ����
//0 ����ģʽ
//1 ����ģʽ  ��ת180��
//2 ����ģʽ
//3 ����ģʽ  ��ת180��
#define IPS200_DISPLAY_DIR 0

#if (0==IPS200_DISPLAY_DIR || 1==IPS200_DISPLAY_DIR)
#define	IPS200_X_MAX	IPS200_W	//Һ��X�����
#define IPS200_Y_MAX	IPS200_H   //Һ��Y�����
     
#elif (2==IPS200_DISPLAY_DIR || 3==IPS200_DISPLAY_DIR)
#define	IPS200_X_MAX	IPS200_H	//Һ��X�����
#define IPS200_Y_MAX	IPS200_W   //Һ��Y�����
     
#else
#error "IPS200_DISPLAY_DIR �������"
     
#endif


void ips200_init(void); //��ʼ��Ӳ��
void ips200_w_data(uint8 dat);
void ips200_wr_reg(uint8 com);
void ips200_wr_data(uint8 dat);
void ips200_wr_data16(uint16 dat);
void ips200_w_reg(uint8 com,uint8 dat);
void ips200_address_set(uint16 x1,uint16 y1,uint16 x2,uint16 y2);
void ips200_clear(uint16 color);
void ips200_drawpoint(uint16 x,uint16 y,uint16 color);
void ips200_showchar(uint16 x,uint16 y,const int8 dat);
void ips200_showstr(uint16 x,uint16 y,const int8 dat[]);


void ips200_showint8(uint16 x,uint16 y,int8 dat);
void ips200_showuint8(uint16 x,uint16 y,uint8 dat);
void ips200_showint16(uint16 x,uint16 y,int16 dat);
void ips200_showuint16(uint16 x,uint16 y,uint16 dat);
void ips200_showint32(uint16 x,uint16 y,int dat,uint8 num);
void ips200_showfloat(uint16 x,uint16 y,double dat,int8 num,int8 pointnum);
void ips200_showimage(uint16 x,uint16 y,uint16 w,uint16 l,const unsigned char *p);

void ips200_displayimage032(uint8 *p, uint16 width, uint16 height);
void ips200_displayimage032_zoom(uint8 *p, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height);
void ips200_displayimage032_zoom1(uint8 *p, uint16 width, uint16 height, uint16 start_x, uint16 start_y, uint16 dis_width, uint16 dis_height);
void ips200_displayimage8660_zoom(uint16 *p, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height);
void ips200_displayimage8660_zoom1(uint16 *p, uint16 width, uint16 height, uint16 start_x, uint16 start_y, uint16 dis_width, uint16 dis_height);
void ips200_displayimage8660_grayscale_zoom(uint16 *p, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height);
void ips200_displayimage7725(uint8 *p, uint16 width, uint16 height);
void ips200_display_chinese(uint16 x, uint16 y, uint8 size, const uint8 *p, uint8 number, uint16 color);

#endif

