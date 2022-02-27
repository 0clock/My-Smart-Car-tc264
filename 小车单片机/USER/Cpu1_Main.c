/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		ADS v1.2.2
 * @Target core		TC264D
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-3-23
 ********************************************************************************************************************/

#include "headfile.h"
#include "KEY.h"//按键扫描相关
#include "CAMERA.h"//摄像头、图像处理相关
#include "MOTOR.h"//直流电机相关
#include "OLED.h"//显示屏相关
#include "STEERING.h"//舵机相关
#include "UART.h"
#include "fastlz.h"


#pragma section all "cpu1_dsram"
//将本语句与#pragma section all restore语句之间的全局变量都放在CPU1的RAM中



void core1_main(void)
{
	disableInterrupts();
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    //用户在此处调用各种初始化函数等





	//等待所有核心初始化完毕
	IfxCpu_emitEvent(&g_cpuSyncEvent);
	IfxCpu_waitEvent(&g_cpuSyncEvent, 0xFFFF);
    enableInterrupts();
    while (TRUE)
    {
        //来自摄像机的图像到达后，进行图像处理
        if (mt9v03x_finish_flag == 1 && (UART_Flag_TX == FALSE || UART_EN == FALSE))
        {
            Get_Cutted_Image();//裁剪图像到188*40
            mt9v03x_finish_flag = 0;//表示可以更新mt9v03x_image了
            if (UART_EN == TRUE)
            {
                UART_Flag_TX = TRUE;
            }
            Get_Thresholding_Image();
            Get_Inverse_Perspective_Image();
        }

        //由处理后的图像等信息，获取速度、转向角度的目标值

        //低速目标且低速时，开环
        if (speed_Target < 0.5 && speed_Target > -0.5 && speed_Measured < 0.5 && speed_Measured > -0.5)
        {
            PID_mode = OPEN_LOOP;
        }
        else
        {
            PID_mode = PID_CLOSED_LOOP;
        }



    }
}



#pragma section all restore
