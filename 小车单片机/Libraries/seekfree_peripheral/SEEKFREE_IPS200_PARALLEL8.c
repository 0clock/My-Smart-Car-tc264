/*********************************************************************************************************************
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

#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "zf_stm_systick.h"
#include "zf_gpio.h"
#include "zf_assert.h"
#include "SEEKFREE_PRINTF.h"
#include "SEEKFREE_IPS200_PARALLEL8.h"


//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ڲ����ã��û��������
//  @param      void 		    
//  @return     				
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void ips200_w_data(uint8 dat)			//д����
{
    IPS200_DATAPORT1 = (dat << DATA_START_NUM1) | (IPS200_DATAPORT1 & ~((uint32)(0x0F << DATA_START_NUM1)) );
    IPS200_DATAPORT2 = ((dat>>4) << DATA_START_NUM2) | (IPS200_DATAPORT2 & ~((uint32)(0x0F << DATA_START_NUM2)) );
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ڲ����ã��û��������
//  @param      void 		    
//  @return     				
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void ips200_wr_reg(uint8 command)			//д����
{
	IPS200_CS(0); 
	IPS200_RS(0);
	IPS200_RD(1);
	IPS200_WR(0);
	ips200_w_data(command);
	IPS200_WR(1);	
	IPS200_CS(1); 
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ڲ����ã��û��������
//  @param      void 		    
//  @return     				
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void ips200_wr_data(uint8 dat)			//��Һ����дһ��8λ����
{
	IPS200_CS(0); 
	IPS200_RS(1);
	IPS200_RD(1);
	IPS200_WR(0);
	ips200_w_data(dat);
	IPS200_WR(1);
	IPS200_CS(1); 	
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ڲ����ã��û��������
//  @param      void 		    
//  @return     				
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void ips200_wr_data16(uint16 dat)		//��Һ����дһ��16λ����
{
	IPS200_CS(0); 
	IPS200_RS(1);
	IPS200_RD(1);
	IPS200_WR(0);
	ips200_w_data((uint8)(dat>>8));
	IPS200_WR(1);
	IPS200_WR(0);
	ips200_w_data((uint8)(dat));
	IPS200_WR(1);
	IPS200_CS(1); 	 	
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ڲ����ã��û��������
//  @param      void 		    
//  @return     				
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void ips200_w_reg(uint8 com,uint8 dat)		//д�Ĵ���
{
	ips200_wr_reg(com);
	ips200_wr_data(dat);
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ڲ����ã��û��������
//  @param      void 		    
//  @return     				
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void ips200_address_set(uint16 x1,uint16 y1,uint16 x2,uint16 y2)
{ 
	ips200_wr_reg(0x2a);
	ips200_wr_data16(x1);
	ips200_wr_data16(x2);
	
	ips200_wr_reg(0x2b);
	ips200_wr_data16(y1);
	ips200_wr_data16(y2);
	
	ips200_wr_reg(0x2c);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      2.0�� IPSҺ����ʼ��
//  @return     void
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void ips200_init(void)
{
	gpio_init(IPS200_D0_PIN, GPO, 0, PUSHPULL);
	gpio_init(IPS200_D1_PIN, GPO, 0, PUSHPULL);
	gpio_init(IPS200_D2_PIN, GPO, 0, PUSHPULL);
	gpio_init(IPS200_D3_PIN, GPO, 0, PUSHPULL);
	gpio_init(IPS200_D4_PIN, GPO, 0, PUSHPULL);
	gpio_init(IPS200_D5_PIN, GPO, 0, PUSHPULL);
	gpio_init(IPS200_D6_PIN, GPO, 0, PUSHPULL);
	gpio_init(IPS200_D7_PIN, GPO, 0, PUSHPULL);
	
	gpio_init(IPS200_BL_PIN, GPO, 0, PUSHPULL);
	gpio_init(IPS200_CS_PIN, GPO, 0, PUSHPULL);
	gpio_init(IPS200_RST_PIN, GPO, 0, PUSHPULL);
	gpio_init(IPS200_RS_PIN, GPO, 0, PUSHPULL);
	gpio_init(IPS200_WR_PIN, GPO, 0, PUSHPULL);
	gpio_init(IPS200_RD_PIN, GPO, 0, PUSHPULL);
	
	IPS200_BL(1);
	IPS200_RST(0);	
	systick_delay_ms(STM0, 5);
	IPS200_RST(1);		
	systick_delay_ms(STM0, 5);
	
	ips200_wr_reg(0x11);
	systick_delay_ms(STM0, 120);
	
	ips200_wr_reg(0x36);			
    if      (IPS200_DISPLAY_DIR==0)    ips200_wr_data(0x00);
    else if (IPS200_DISPLAY_DIR==1)    ips200_wr_data(0xC0);
    else if (IPS200_DISPLAY_DIR==2)    ips200_wr_data(0x70);
    else                            ips200_wr_data(0xA0);
    




	ips200_wr_reg(0x3A);			
	ips200_wr_data(0x05);
	
	ips200_wr_reg(0xB2);			
	ips200_wr_data(0x0C);
	ips200_wr_data(0x0C); 
	ips200_wr_data(0x00); 
	ips200_wr_data(0x33); 
	ips200_wr_data(0x33); 			

	ips200_wr_reg(0xB7);			
	ips200_wr_data(0x35);

	ips200_wr_reg(0xBB);			
	ips200_wr_data(0x29); //32 Vcom=1.35V
															
	ips200_wr_reg(0xC2);			
	ips200_wr_data(0x01);

	ips200_wr_reg(0xC3);			
	ips200_wr_data(0x19); //GVDD=4.8V 
															
	ips200_wr_reg(0xC4);			
	ips200_wr_data(0x20); //VDV, 0x20:0v

	ips200_wr_reg(0xC5);			
	ips200_wr_data(0x1A);//VCOM Offset Set

	ips200_wr_reg(0xC6);			
	ips200_wr_data(0x01F); //0x0F:60Hz        	

	ips200_wr_reg(0xD0);			
	ips200_wr_data(0xA4);
	ips200_wr_data(0xA1); 											  												  																								
				
	ips200_wr_reg(0xE0);     
	ips200_wr_data(0xD0);   
	ips200_wr_data(0x08);   
	ips200_wr_data(0x0E);   
	ips200_wr_data(0x09);   
	ips200_wr_data(0x09);   
	ips200_wr_data(0x05);   
	ips200_wr_data(0x31);   
	ips200_wr_data(0x33);   
	ips200_wr_data(0x48);   
	ips200_wr_data(0x17);   
	ips200_wr_data(0x14);   
	ips200_wr_data(0x15);   
	ips200_wr_data(0x31);   
	ips200_wr_data(0x34);   

	ips200_wr_reg(0xE1);     
	ips200_wr_data(0xD0);   
	ips200_wr_data(0x08);   
	ips200_wr_data(0x0E);   
	ips200_wr_data(0x09);   
	ips200_wr_data(0x09); 
	ips200_wr_data(0x15);   
	ips200_wr_data(0x31);   
	ips200_wr_data(0x33);   
	ips200_wr_data(0x48);   
	ips200_wr_data(0x17);   
	ips200_wr_data(0x14);   
	ips200_wr_data(0x15);   
	ips200_wr_data(0x31);   
	ips200_wr_data(0x34);   

	ips200_wr_reg(0x21);
	
	ips200_wr_reg(0x29);
	ips200_clear(IPS200_BGCOLOR);	//��ʼ��Ϊ����	
} 


//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����������
//  @param      color     		��ɫ����
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_clear(YELLOW);// ȫ������Ϊ��ɫ
//-------------------------------------------------------------------------------------------------------------------
void ips200_clear(uint16 color) 
{ 
	uint16 i,j;  	
	ips200_address_set(0,0,IPS200_X_MAX-1,IPS200_Y_MAX-1);
	for(i=0;i<IPS200_X_MAX;i++)
	{
		for (j=0;j<IPS200_Y_MAX;j++)
		{
			ips200_wr_data16(color);	 			 
		}
	}
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ������
//  @param      x     	        ����x��������
//  @param      y     	        ����y��������
//  @param      dat       	    ��Ҫ��ʾ����ɫ
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_drawpoint(0,0,RED);  //����0,0��һ����ɫ�ĵ�
//-------------------------------------------------------------------------------------------------------------------
void ips200_drawpoint(uint16 x,uint16 y,uint16 color)
{
    ips200_address_set(x,y,x,y);
    ips200_wr_data16(color);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����ʾ�ַ�
//  @param      x     	        ����x�������� ������Χ 0 - (IPS200_X_MAX-1)
//  @param      y     	        ����y�������� ������Χ 0 - (IPS200_Y_MAX/16-1)
//  @param      dat       	    ��Ҫ��ʾ���ַ�
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_showchar(0,0,'x');//����0,0дһ���ַ�x
//-------------------------------------------------------------------------------------------------------------------
void ips200_showchar(uint16 x,uint16 y,const int8 dat)
{
	uint8 i,j;
	uint8 temp;
    
	for(i=0; i<16; i++)
	{
		ips200_address_set(x,y+i,x+7,y+i);
		temp = tft_ascii[(uint16)dat-32][i];//��32��Ϊ��ȡģ�Ǵӿո�ʼȡ�� �ո���ascii�������32
		for(j=0; j<8; j++)
		{
			if(temp&0x01)	ips200_wr_data16(IPS200_PENCOLOR);
			else			ips200_wr_data16(IPS200_BGCOLOR);
			temp>>=1;
		}
	}
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����ʾ�ַ���
//  @param      x     	        ����x��������  ������Χ 0 - (IPS200_X_MAX-1)
//  @param      y     	        ����y��������  ������Χ 0 - (IPS200_Y_MAX/16-1)
//  @param      dat       	    ��Ҫ��ʾ���ַ���
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_showstr(0,0,"seekfree");
//-------------------------------------------------------------------------------------------------------------------
void ips200_showstr(uint16 x,uint16 y,const int8 dat[])
{         
	uint16 j;
	
	j = 0;
	while(dat[j] != '\0')
	{
		ips200_showchar(x+8*j,y*16,dat[j]);
		j++;
	}
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����ʾ8λ�з���
//  @param      x     	        ����x��������  ������Χ 0 -��IPS200_X_MAX-1��
//  @param      y     	        ����y��������  ������Χ 0 -��IPS200_Y_MAX/16-1��
//  @param      dat       	    ��Ҫ��ʾ�ı�������������int8
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_showint8(0,0,x);//xΪint8����
//-------------------------------------------------------------------------------------------------------------------
void ips200_showint8(uint16 x,uint16 y,int8 dat)
{
	uint8 a[3];
	uint8 i;
	if(dat<0)
	{
		ips200_showchar(x,y*16,'-');
		dat = -dat;
	}
	else	ips200_showchar(x,y*16,' ');
	
	a[0] = dat/100;
	a[1] = dat/10%10;
	a[2] = dat%10;
	i = 0;
	while(i<3)
	{
		ips200_showchar(x+(8*(i+1)),y*16,'0' + a[i]);
		i++;
	}
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����ʾ8λ�޷���
//  @param      x     	        ����x�������� ������Χ 0 -��IPS200_X_MAX-1��
//  @param      y     	        ����y�������� ������Χ 0 -��IPS200_Y_MAX/16-1��
//  @param      dat       	    ��Ҫ��ʾ�ı�������������uint8
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_showuint8(0,0,x);//xΪuint8����
//-------------------------------------------------------------------------------------------------------------------
void ips200_showuint8(uint16 x,uint16 y,uint8 dat)
{
	uint8 a[3];
	uint8 i;
	
	a[0] = dat/100;
	a[1] = dat/10%10;
	a[2] = dat%10;
	i = 0;
	while(i<3)
	{
		ips200_showchar(x+(8*i),y*16,'0' + a[i]);
		i++;
	}
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����ʾ16λ�з���
//  @param      x     	        ����x�������� ������Χ 0 -��IPS200_X_MAX-1��
//  @param      y     	        ����y�������� ������Χ 0 -��IPS200_Y_MAX/16-1��
//  @param      dat       	    ��Ҫ��ʾ�ı�������������int16
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_showint16(0,0,x);//xΪint16����
//-------------------------------------------------------------------------------------------------------------------
void ips200_showint16(uint16 x,uint16 y,int16 dat)
{
	uint8 a[5];
	uint8 i;
	if(dat<0)
	{
		ips200_showchar(x,y*16,'-');
		dat = -dat;
	}
	else	ips200_showchar(x,y*16,' ');

	a[0] = dat/10000;
	a[1] = dat/1000%10;
	a[2] = dat/100%10;
	a[3] = dat/10%10;
	a[4] = dat%10;
	
	i = 0;
	while(i<5)
	{
		ips200_showchar(x+(8*(i+1)),y*16,'0' + a[i]);
		i++;
	}
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����ʾ16λ�޷���
//  @param      x     	        ����x�������� ������Χ 0 -��IPS200_X_MAX-1��
//  @param      y     	        ����y�������� ������Χ 0 -��IPS200_Y_MAX/16-1��
//  @param      dat       	    ��Ҫ��ʾ�ı�������������uint16
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_showuint16(0,0,x);//xΪuint16����
//-------------------------------------------------------------------------------------------------------------------
void ips200_showuint16(uint16 x,uint16 y,uint16 dat)
{
	uint8 a[5];
	uint8 i;

	a[0] = dat/10000;
	a[1] = dat/1000%10;
	a[2] = dat/100%10;
	a[3] = dat/10%10;
	a[4] = dat%10;
	
	i = 0;
	while(i<5)
	{
		ips200_showchar(x+(8*i),y*16,'0' + a[i]);
		i++;
	}
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����ʾ32λ�з���(ȥ������������Ч��0)
//  @param      x     	        ����x�������� ������Χ 0 -��IPS200_X_MAX-1��
//  @param      y     	        ����y�������� ������Χ 0 -��IPS200_Y_MAX/16-1��
//  @param      dat       	    ��Ҫ��ʾ�ı�������������uint32
//  @param      num       	    ��Ҫ��ʾ��λ�� ���10λ  ������������
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_showint32(0,0,x,3);//x����Ϊint32 uint16 int16 uint8 int8����
//  Sample usage:               ��������ʾһ�� ��-����   ������ʾһ���ո�
//-------------------------------------------------------------------------------------------------------------------
void ips200_showint32(uint16 x,uint16 y,int dat,uint8 num)
{
    int8    buff[34];
    uint32   length;
    
    if(10<num)      num = 10;
    
	
    num++;
    if(0>dat)   length = zf_sprintf(&buff[0],"%d",dat);//����
    else
    {
        buff[0] = ' ';
        length = zf_sprintf(&buff[1],"%d",dat);
        length++;
    }
    while(length < num)
    {
        buff[length] = ' ';
        length++;
    }
    buff[num] = '\0';

    ips200_showstr(x, y, buff);	//��ʾ����
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����ʾ������(ȥ������������Ч��0)
//  @param      x     	        ����x�������� ������Χ 0 -��IPS200_X_MAX-1��
//  @param      y     	        ����y�������� ������Χ 0 -��IPS200_Y_MAX/16-1��
//  @param      dat       	    ��Ҫ��ʾ�ı�������������float��double
//  @param      num       	    ����λ��ʾ����   ���10λ  
//  @param      pointnum        С��λ��ʾ����   ���6λ
//  @return     void
//  @since      v1.0
//  Sample usage:               ips_showfloat(0,0,x,2,3);//��ʾ������   ������ʾ2λ   С����ʾ��λ
//  @note                       �ر�ע�⵱����С��������ʾ��ֵ����д���ֵ��һ����ʱ��
//                              ���������ڸ��������ȶ�ʧ���⵼�µģ��Ⲣ������ʾ���������⣬
//                              �й���������飬�����аٶ�ѧϰ   ���������ȶ�ʧ���⡣
//                              ��������ʾһ�� ��-����   ������ʾһ���ո�
//-------------------------------------------------------------------------------------------------------------------
void ips200_showfloat(uint16 x,uint16 y,double dat,int8 num,int8 pointnum)
{
    uint32   length;
	int8    buff[34];
	int8    start,end,point;

	if(6<pointnum)  pointnum = 6;
    if(10<num)      num = 10;
        
    if(0>dat)   length = zf_sprintf( &buff[0],"%f",dat);//����
    else
    {
        length = zf_sprintf( &buff[1],"%f",dat);
        length++;
    }
    point = (int8)(length - 7);         //����С����λ��
    start = point - num - 1;    //������ʼλ
    end = point + pointnum + 1; //�������λ
    while(0>start)//����λ����  ĩβӦ�����ո�
    {
        buff[end] = ' ';
        end++;
        start++;
    }
    
    if(0>dat)   buff[start] = '-';
    else        buff[start] = ' ';
    
    buff[end] = '\0';

    ips200_showstr(x, y, buff);	//��ʾ����
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����ʾ8λ�޷���
//  @param      x     	        ����x�������� ������Χ 0 -��IPS200_X_MAX-1��
//  @param      y     	        ����y�������� ������Χ 0 -��IPS200_Y_MAX/16-1��
//  @param      w       	    ͼ����
//  @param      l       	    ͼ��߶�
//  @param      *p       	    ͼ�������ַ
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_showimage(0,0,10,20,&image);//ͼ�����(0,0)����10����20��
//-------------------------------------------------------------------------------------------------------------------
void ips200_showimage(uint16 x,uint16 y,uint16 w,uint16 l,const unsigned char *p)
{
  int i;
  unsigned char picH,picL;
  ips200_address_set(x,y,x+w-1,y+l-1);
  for(i=0;i<w*l;i++)
  {
    picL=*(p+i*2);
    picH=*(p+i*2+1);
    ips200_wr_data16(picH<<8|picL);  
  }
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      �����(�Ҷ�����ͷ)Һ����ʾ����
//  @param      *p     			ͼ�������ַ
//  @param      width     	    ͼ����
//  @param      height     	    ͼ��߶�
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_displayimage032(mt9v03x_csi_image[0], MT9V03X_CSI_W, MT9V03X_CSI_H)//��ʾ�Ҷ�����ͷ ͼ��
//  @note       ͼ��Ŀ���������Һ���Ŀ�ȣ����Զ�����������ʾ������������ʾȫ��Ұ
//-------------------------------------------------------------------------------------------------------------------
void ips200_displayimage032(uint8 *p, uint16 width, uint16 height) 
{
    uint32 i,j;
                
    uint16 color = 0;
	uint16 temp = 0;
	
    uint16 coord_x = 0;
    uint16 coord_y = 0;
    
    coord_x = width>IPS200_X_MAX?IPS200_X_MAX:width;
    coord_y = height>IPS200_Y_MAX?IPS200_Y_MAX:height;
    ips200_address_set(0,0,coord_x-1,coord_y-1);
    
    for(j=0;j<coord_y;j++)
    {
        for(i=0;i<coord_x;i++)
        {
            temp = *(p+j*width+i*width/coord_x);//��ȡ���ص�
            color=(0x001f&((temp)>>3))<<11;
            color=color|(((0x003f)&((temp)>>2))<<5);
            color=color|(0x001f&((temp)>>3));
            ips200_wr_data16(color); 
        }
    }
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      �����(�Ҷ�����ͷ)Һ��������ʾ����
//  @param      *p     			ͼ�������ַ
//  @param      width     	    ͼ����
//  @param      height     	    ͼ��߶�
//  @param      dis_width       ͼ����ʾ���  0 -��IPS200_X_MAX-1��
//  @param      dis_height      ͼ����ʾ�߶�  0 -��IPS200_Y_MAX-1��
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_displayimage032_zoom(mt9v03x_csi_image[0], MT9V03X_CSI_W, MT9V03X_CSI_H, MT9V03X_CSI_W, MT9V03X_CSI_H)//��ʾ�Ҷ�����ͷ ͼ��
//  @note       ͼ��Ŀ���������Һ���Ŀ�ȣ����Զ�����������ʾ������������ʾȫ��Ұ
//-------------------------------------------------------------------------------------------------------------------
void ips200_displayimage032_zoom(uint8 *p, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height)
{
    uint32 i,j;
                
    uint16 color = 0;
	uint16 temp = 0;

    ips200_address_set(0,0,dis_width-1,dis_height-1);//������ʾ���� 
    
    for(j=0;j<dis_height;j++)
    {
        for(i=0;i<dis_width;i++)
        {
            temp = *(p+(j*height/dis_height)*width+i*width/dis_width);//��ȡ���ص�
            color=(0x001f&((temp)>>3))<<11;
            color=color|(((0x003f)&((temp)>>2))<<5);
            color=color|(0x001f&((temp)>>3));
            ips200_wr_data16(color); 
        }
    }
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      �����(�Ҷ�����ͷ)Һ��������ʾ����
//  @param      *p     			ͼ�������ַ
//  @param      width     	    ͼ����
//  @param      height     	    ͼ��߶�
//  @param      start_x         ������ʾ����x������
//  @param      start_y     	������ʾ����y������
//  @param      dis_width       ͼ����ʾ���  1 -��TFT_X_MAX��
//  @param      dis_height      ͼ����ʾ�߶�  1 -��TFT_Y_MAX��
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_displayimage032_zoom1(mt9v03x_csi_image[0], MT9V03X_CSI_W, MT9V03X_CSI_H, 0, 0, MT9V03X_CSI_W, MT9V03X_CSI_H)//��ʾ�Ҷ�����ͷ ͼ��
//-------------------------------------------------------------------------------------------------------------------
void ips200_displayimage032_zoom1(uint8 *p, uint16 width, uint16 height, uint16 start_x, uint16 start_y, uint16 dis_width, uint16 dis_height)
{
    uint32 i,j;
                
    uint16 color = 0;
	uint16 temp = 0;

	//������õĲ����Ƿ񳬹���Ļ�ķֱ���
	if((start_x+dis_width)>IPS200_X_MAX)	ZF_ASSERT(0);
	if((start_y+dis_height)>IPS200_Y_MAX)	ZF_ASSERT(0);

    ips200_address_set(start_x,start_y,start_x+dis_width-1,start_y+dis_height-1);//������ʾ���� 
    
    for(j=0;j<dis_height;j++)
    {
        for(i=0;i<dis_width;i++)
        {
            temp = *(p+(j*height/dis_height)*width+i*width/dis_width);//��ȡ���ص�
            color=(0x001f&((temp)>>3))<<11;
            color=color|(((0x003f)&((temp)>>2))<<5);
            color=color|(0x001f&((temp)>>3));
            ips200_wr_data16(color); 
        }
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ͫ(��ɫ����ͷ)Һ��������ʾ����
//  @param      *p     			ͼ�������ַ
//  @param      width     	    ͼ����
//  @param      height     	    ͼ��߶�
//  @param      dis_width       ͼ����ʾ���  0 -��TFT_X_MAX-1��
//  @param      dis_height      ͼ����ʾ�߶�  0 -��TFT_Y_MAX-1��
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_displayimage8660_zoom(scc8660_csi_image[0], SCC8660_CSI_PIC_W, SCC8660_CSI_PIC_H, 320, 240);//��ʾ��ɫ����ͷ ͼ��
//  @note       �˺�����֧������ͷ���ݸ�ʽ(SCC8660_DATA_FORMAT)����Ϊ0ʱ����ʾ��ɫͼ��
//              ͼ��Ŀ���������Һ���Ŀ�ȣ����Զ�����������ʾ������������ʾȫ��Ұ
//-------------------------------------------------------------------------------------------------------------------
void ips200_displayimage8660_zoom(uint16 *p, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height)
{
    uint32 i,j;
    uint16 color = 0;

    ips200_address_set(0,0,dis_width-1,dis_height-1);//������ʾ���� 
    
    for(j=0;j<dis_height;j++)
    {
        for(i=0;i<dis_width;i++)
        {
            color = *(p+(j*height/dis_height)*width+i*width/dis_width);//��ȡ���ص�
            color = ((color&0xff)<<8) | (color>>8);
            ips200_wr_data16(color); 
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ͫ(��ɫ����ͷ)Һ��������ʾ����
//  @param      *p     			ͼ�������ַ
//  @param      width     	    ͼ����
//  @param      height     	    ͼ��߶�
//  @param      start_x         ������ʾ����x������
//  @param      start_y     	������ʾ����y������
//  @param      dis_width       ͼ����ʾ���  1 -��TFT_X_MAX��
//  @param      dis_height      ͼ����ʾ�߶�  1 -��TFT_Y_MAX��
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_displayimage8660_zoom1(scc8660_csi_image[0], SCC8660_CSI_PIC_W, SCC8660_CSI_PIC_H, 0, 0, 320, 240);
//-------------------------------------------------------------------------------------------------------------------
void ips200_displayimage8660_zoom1(uint16 *p, uint16 width, uint16 height, uint16 start_x, uint16 start_y, uint16 dis_width, uint16 dis_height)
{
    uint32 i,j;
                
    uint16 color = 0;

	//������õĲ����Ƿ񳬹���Ļ�ķֱ���
	if((start_x+dis_width)>IPS200_X_MAX)	ZF_ASSERT(0);
	if((start_y+dis_height)>IPS200_Y_MAX)	ZF_ASSERT(0);

    ips200_address_set(start_x,start_y,start_x+dis_width-1,start_y+dis_height-1);//������ʾ���� 
    
	for(j=0;j<dis_height;j++)
    {
        for(i=0;i<dis_width;i++)
        {
            color = *(p+(j*height/dis_height)*width+i*width/dis_width);//��ȡ���ص�
            color = ((color&0xff)<<8) | (color>>8);
            ips200_wr_data16(color); 
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ͫ(��ɫ����ͷ)Һ��������ʾ����(��ʾ�Ҷ�)
//  @param      *p     			ͼ�������ַ
//  @param      width     	    ͼ����
//  @param      height     	    ͼ��߶�
//  @param      dis_width       ͼ����ʾ���  0 -��TFT_X_MAX-1��
//  @param      dis_height      ͼ����ʾ�߶�  0 -��TFT_Y_MAX-1��
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_displayimage8660_grayscale_zoom(scc8660_csi_image[0], SCC8660_CSI_PIC_W, SCC8660_CSI_PIC_H, 320, 240)//��ʾ��ɫ����ͷͼ��   ��ʾY����
//  @note       �˺�����֧������ͷ���ݸ�ʽ(SCC8660_DATA_FORMAT)����Ϊ2ʱ����ȡY������ʾ�Ҷ�ͼ��
//              ͼ��Ŀ���������Һ���Ŀ�ȣ����Զ�����������ʾ������������ʾȫ��Ұ
//-------------------------------------------------------------------------------------------------------------------
void ips200_displayimage8660_grayscale_zoom(uint16 *p, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height)
{
    uint32 i,j;
    uint16 color = 0;
    uint16 temp = 0;
    
    ips200_address_set(0,0,dis_width-1,dis_height-1);//������ʾ���� 
    
    for(j=0;j<dis_height;j++)
    {
        for(i=0;i<dis_width;i++)
        {
            temp = *(p+(j*height/dis_height)*width+i*width/dis_width);//��ȡ���ص�
            temp = temp&0xff;
            color=(0x001f&((temp)>>3))<<11;
            color=color|(((0x003f)&((temp)>>2))<<5);
            color=color|(0x001f&((temp)>>3));
            ips200_wr_data16(color); 
        }
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      С���(��ֵ������ͷ)Һ����ʾ����
//  @param      *p     			ͼ�������ַ
//  @param      width     	    ͼ����
//  @param      height     	    ͼ��߶�
//  @return     void
//  @since      v1.0
//  @note      	ͼ��ֱ�����Ҫ������120(��)*160(��)����       
//-------------------------------------------------------------------------------------------------------------------
void ips200_displayimage7725(uint8 *p, uint16 width, uint16 height) 
{
    int i,j; 
	
    uint16 temp = 0;
    ips200_address_set(0,0,width-1,height-1);
	for(i=0;i<height * (width/8);i++)
    {
        temp = *p;
        p++;
        for(j=0; j<8; j++)
        {
            if( (temp<<j)&0x80 )	ips200_wr_data16(WHITE);
            else					ips200_wr_data16(BLACK);
        }
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ������ʾ
//  @param      x       ������ 0-��IPS200_X_MAX-1��
//  @param      y       ������ 0-��IPS200_Y_MAX-1��
//  @param      size    ȡģ��ʱ�����õĺ��������С��Ҳ����һ������ռ�õĵ��󳤿�Ϊ���ٸ��㣬ȡģ��ʱ����Ҫ������һ���ġ�
//  @param      *p      ��Ҫ��ʾ�ĺ�������
//  @param      number  ��Ҫ��ʾ����λ
//  @param      color   ��ʾ��ɫ
//  @return     void
//  @since      v1.0
//  Sample usage:		ips200_display_chinese(0,0,16,chinese_test[0],4,RED);//��ʾfont�ļ������ ʾ��
//  @Note       ʹ��PCtoLCD2002���ȡģ		    ���롢����ʽ��˳��   16*16
//-------------------------------------------------------------------------------------------------------------------
void ips200_display_chinese(uint16 x, uint16 y, uint8 size, const uint8 *p, uint8 number, uint16 color)
{
    int i, j, k; 
    uint8 temp, temp1, temp2;
    const uint8 *p_data;
    
    temp2 = size/8;
    
    ips200_address_set(x,y,number*size-1+x,y+size-1);
    
    for(i=0;i<size;i++)
    {
        temp1 = number;
        p_data = p+i*temp2;
        while(temp1--)
        {
            for(k=0;k<temp2;k++)
            {
                for(j=8;j>0;j--)
                {
                    temp = (*p_data>>(j-1)) & 0x01;
                    if(temp)    ips200_wr_data16(color);
                    else        ips200_wr_data16(IPS200_BGCOLOR);
                }
                p_data++;
            }
            p_data = p_data - temp2 + temp2*size;
        }   
    }
}
