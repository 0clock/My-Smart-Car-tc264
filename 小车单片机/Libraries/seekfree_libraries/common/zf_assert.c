/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		zf_assert
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		ADS v1.2.2
 * @Target core		TC264D
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-3-23
 ********************************************************************************************************************/

#include "stdio.h"
#include "TC264_config.h"
#include "zf_assert.h"

void assert(pchar __file, unsigned int __line, pchar __function)
{
	//������Դ��� �����ļ�Ŀ¼ �����ļ��� ���ú�������
#if(DEBUG_PRINTF)
	printf("ASSERTION: %s %d %s\n",__file, __line, __function);
#endif
#pragma warning 507
	while(TRUE)
	{
		//�ȴ�����Ա��� ���Դ���

	}
#pragma warning default
}
