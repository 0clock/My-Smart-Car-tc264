/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		eeprom(Dflash)
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		ADS v1.2.2
 * @Target core		TC264D
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-3-23
 ********************************************************************************************************************/
 
#ifndef _zf_eeprom_h
#define _zf_eeprom_h

#include "common.h"
#include "IfxFlash_cfg.h"


#define EEPROM_BASE_ADDR             IFXFLASH_DFLASH_START					
#define EEPROM_SIZE                  IFXFLASH_DFLASH_SIZE					//һ����96KB
#define EEPROM_SECTOR_SIZE           (EEPROM_SIZE / IFXFLASH_DFLASH_NUM_LOG_SECTORS)
#define EEPROM_SECTOR_NUM            (IFXFLASH_DFLASH_NUM_LOG_SECTORS)		//һ����12������

#define EEPROM_PAGE_SIZE             IFXFLASH_DFLASH_PAGE_LENGTH
#define EEPROM_SECTOR_PAGE_NUM       (EEPROM_SECTOR_SIZE/EEPROM_PAGE_SIZE)	//ÿ��������1024ҳ


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ʹ�ú궨���flash�������ݶ�ȡ
//  @param      sector_num 		��Ҫд����������
//  @param      page_num	 	��Ҫ��ȡ��ҳ
//  @param      type		 	��ȡ����������
//  @return     				���ظ�����ַ������
//  @since      v1.0
//  Sample usage:               flash_read(0,0,uint32);//��ȡ0������  ��0Ҳ���� ����Ϊuint32
//-------------------------------------------------------------------------------------------------------------------
#define     flash_read(sector_num,page_num,type)      (*(type *)((uint32)((EEPROM_BASE_ADDR + (sector_num)*EEPROM_SECTOR_SIZE) + (page_num*8))))

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��������ת��Ϊ����������
//  @param      float_data 		��Ҫת���ĸ�����
//  @return     				������������
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
#define     float_conversion_uint32(float_data)     (*(uint32 *)&float_data)




uint8 flash_check(uint32 sector_num, uint32 page_num);
void eeprom_erase_sector(uint32 sector_num);
void eeprom_page_program(uint32 sector_num, uint32 page_num, uint32 *buf);

#endif
