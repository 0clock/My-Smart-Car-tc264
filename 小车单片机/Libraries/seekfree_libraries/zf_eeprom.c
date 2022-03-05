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
 

#include "IfxScuWdt.h"
#include "IfxFlash.h"
#include "zf_assert.h"
#include "zf_eeprom.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      У��FLASH�Ƿ�������
//  @param      sector_num 		��Ҫд����������   ������Χ0-11
//  @param      page_num	 	��ǰ����ҳ�ı��     ������Χ0-1023
//  @return     				����1�����ݣ�����0û�����ݣ������Ҫ�������ݵ�����д���µ�������Ӧ�ö������������в�������
//  @since      v1.0
//  Sample usage:               flash_check(0,0);//У��0����������0ҳ�Ƿ�������
//-------------------------------------------------------------------------------------------------------------------
uint8 flash_check(uint32 sector_num, uint32 page_num)
{
	ZF_ASSERT(EEPROM_SECTOR_NUM>sector_num);
	ZF_ASSERT(EEPROM_SECTOR_PAGE_NUM>page_num);
	uint32 sector_addr = IfxFlash_dFlashTableEepLog[sector_num].start;
	uint32 page_addr = sector_addr + page_num * IFXFLASH_DFLASH_PAGE_LENGTH;

	uint32 temp = *(uint32 *)page_addr;
	if(0 != temp)	return 1;

	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��������
//  @param      sector_num 		��Ҫд����������   ������Χ0-11
//  @return     void
//  @since      v1.0
//  Sample usage:               eeprom_erase_sector(0);
//-------------------------------------------------------------------------------------------------------------------
void eeprom_erase_sector(uint32 sector_num)
{
	ZF_ASSERT(EEPROM_SECTOR_NUM>sector_num);

	uint32 flash = 0;
	uint16 end_init_sfty_pw;
	uint32 sector_addr = IfxFlash_dFlashTableEepLog[sector_num].start;

	end_init_sfty_pw = IfxScuWdt_getSafetyWatchdogPassword();

	IfxScuWdt_clearSafetyEndinit(end_init_sfty_pw);
    IfxFlash_eraseSector(sector_addr);
    IfxScuWdt_setSafetyEndinit(end_init_sfty_pw);

    IfxFlash_waitUnbusy(flash, IfxFlash_FlashType_D0);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���һҳ
//  @param      sector_num 		��Ҫд����������   ������Χ0-11
//  @param      page_num 		��Ҫд���ҳ���     ������Χ0-1023
//  @param      buf 		   	��Ҫд������ݵ�ַ   ������������ͱ���Ϊuint32
//  @return     void
//  @since      v1.0
//  Sample usage:				eeprom_page_program(0,0, &buf);
//-------------------------------------------------------------------------------------------------------------------
void eeprom_page_program(uint32 sector_num, uint32 page_num, uint32 *buf)
{
	ZF_ASSERT(EEPROM_SECTOR_NUM>sector_num);
	ZF_ASSERT(EEPROM_SECTOR_PAGE_NUM>page_num);

	uint32 errors = 0;
	uint32 flash = 0;
	uint16 end_init_sfty_pw;
	uint32 sector_addr = IfxFlash_dFlashTableEepLog[sector_num].start;

	end_init_sfty_pw = IfxScuWdt_getSafetyWatchdogPassword();

	uint32 page_addr = sector_addr + page_num * IFXFLASH_DFLASH_PAGE_LENGTH;
	errors = IfxFlash_enterPageMode(page_addr);
	ZF_ASSERT(0 == errors);

	IfxFlash_waitUnbusy(flash, IfxFlash_FlashType_D0);

	IfxFlash_loadPage(page_addr, buf[0], 0);

	IfxScuWdt_clearSafetyEndinit(end_init_sfty_pw);
	IfxFlash_writePage(page_addr);
	IfxScuWdt_setSafetyEndinit(end_init_sfty_pw);

	IfxFlash_waitUnbusy(flash, IfxFlash_FlashType_D0);
}

