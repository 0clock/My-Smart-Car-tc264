/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		总钻风
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		ADS v1.2.2
 * @Target core		TC264D
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-3-23
 * @note		
					接线定义：
					------------------------------------ 
					模块管脚            			单片机管脚
					SDA(51的RX)         		查看SEEKFREE_MT9V03X.h文件中的MT9V03X_COF_UART_TX宏定义
					SCL(51的TX)         		查看SEEKFREE_MT9V03X.h文件中的MT9V03X_COF_UART_RX宏定义
					场中断(VSY)         		查看SEEKFREE_MT9V03X.h文件中的MT9V03X_VSYNC_PIN宏定义
					行中断(HREF)				程序没有使用，因此不连接
					像素中断(PCLK)      		查看SEEKFREE_MT9V03X.h文件中的MT9V03X_PCLK_PIN宏定义
					数据口(D0-D7)			查看SEEKFREE_MT9V03X.h文件中的MT9V03X_DATA_PIN宏定义
					------------------------------------ 
	
					默认分辨率是           			188*120
					默认FPS                 50帧
 ********************************************************************************************************************/



#ifndef _SEEKFREE_MT9V03X_h
#define _SEEKFREE_MT9V03X_h

#include "common.h"
#include "IfxDma_cfg.h"
#include "zf_uart.h"
#include "SEEKFREE_OLED.h"

//配置摄像头参数
#define MT9V03X_W               188             	//图像宽度 	范围1-188
#define MT9V03X_H               120             	//图像高度	范围1-120



//--------------------------------------------------------------------------------------------------
//引脚配置
//--------------------------------------------------------------------------------------------------
#define MT9V03X_VSYNC_PIN       ERU_CH7_REQ16_P15_1  //场中断引脚	可选范围参考ERU_PIN_enum枚举 不可与像素引脚选择为同一个通道，或者是共用中断的通道
													//例如场中断选择ERU_CH3 那么像素引脚就不能选择ERU_CH7，因为3和7的中断是共用的。

#define MT9V03X_DATA_PIN        P02_0  				//定义D0数据引脚  假设D0定义为P00_0 那么D1所使用的引脚则为P00_1，依次类推
													//可设置参数P00_0、P02_0、P15_0，不能设置为其他参数

#define MT9V03X_PCLK_PIN        ERU_CH2_REQ7_P00_4 //定义像素时钟引脚 可选范围参考ERU_PIN_enum枚举 不可与场中断引脚选择为同一个通道，或者是共用中断的通道
													//例如场中断选择ERU_CH3 那么像素引脚就不能选择ERU_CH7，因为3和7的中断是共用的。

#define MT9V03X_DMA_CH			IfxDma_ChannelId_5	//定义使用的DMA通道 0-47可选  通道号越大优先级越低

//摄像头命令枚举
//typedef enum
//{
//    INIT = 0,               //摄像头初始化命令
//    AUTO_EXP,               //自动曝光命令
//    EXP_TIME,               //曝光时间命令
//    FPS,                    //摄像头帧率命令
//    SET_COL,                //图像列命令
//    SET_ROW,                //图像行命令
//    LR_OFFSET,              //图像左右偏移命令
//    UD_OFFSET,              //图像上下偏移命令
//    GAIN,                   //图像偏移命令
//    CONFIG_FINISH,          //非命令位，主要用来占位计数
//
//    COLOR_GET_WHO_AM_I = 0xEF,
//    SET_EXP_TIME = 0XF0,    //单独设置曝光时间命令
//    GET_STATUS,             //获取摄像头配置命令
//    GET_VERSION,            //固件版本号命令
//
//    SET_ADDR = 0XFE,        //寄存器地址命令
//    SET_DATA                //寄存器数据命令
//}CMD;
     
     

extern uint8    mt9v03x_finish_flag;//一场图像采集完成标志位
//extern uint8    mt9v03x_image[MT9V03X_H][MT9V03X_W];

void mt9v03x_init(void);
void mt9v03x_vsync(void);
void mt9v03x_dma(void);

//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//以下是龙邱部分


/*! 对比度 高对比度0x03c7  低对比度0x01c7  注意 高对比度会使图像变暗 */
#define CAMERA_CONTRAST          0x01c7

/*! 自动曝光开关 默认打开  设置为0 关闭自动曝光 */
#define CAMERA_AUTO_EXPOSURE     1

/*! 自动曝光模式下 亮度 调节范围 1-64 */
#define CAMERA_AUTO_EXPOSURE_BRIGHTNESS  60

/*! 可以在自动曝光的情况下设置固定帧率 */
/*! 自动曝光模式需要设置曝光时间上限 调节范围 1–32765 */
/*! 注意 当帧率过高时 如果设置的曝光时间过长 帧率可能会自适应下调 */
/*! 例如使用100帧时 最大曝光时间超过317 会导致帧率下降 */
/*! 曝光时间越长 图像越亮 */
#define CAMERA_MAX_EXPOSURE_TIME 250
#define CAMERA_MIN_EXPOSURE_TIME  1

/*! 非自动曝光模式下 可以调节曝光时间来调节图像整体亮度 调节范围 0–32765 */
/*! 注意 当帧率过高时 如果设置的曝光时间过长 帧率可能会自适应下调 */
/*! 曝光时间越长 图像越亮 */
#define CAMERA_EXPOSURE_TIME  180


/* 摄像头使用SCCB通信 SCCB和IIC基本类似 */
#define MT9V034_SCL_PIN   P11_2   /*!< SCCB SCL 管脚 */
#define MT9V034_SDA_PIN   P11_3   /*!< SCCB SDA 管脚 */


#define MT9V034_SCL_Out   gpio_dir(MT9V034_SCL_PIN, GPO, PULLDOWN);      //配置输出作为SCL_Out
#define MT9V034_SDA_Out   gpio_dir(MT9V034_SDA_PIN, GPO, PULLDOWN);      //配置作为输出作为SDA_Out
#define MT9V034_SDA_In    gpio_dir(MT9V034_SDA_PIN, GPI, PULLDOWN);      //配置作为输入作为SDA_In
#define MT9V034_SCL_High  gpio_set(MT9V034_SCL_PIN, 1);      //配置输出高电平
#define MT9V034_SCL_Low   gpio_set(MT9V034_SCL_PIN, 0);      //配置输出低电平
#define MT9V034_SDA_High  gpio_set(MT9V034_SDA_PIN, 1);      //配置输出高电平
#define MT9V034_SDA_Low   gpio_set(MT9V034_SDA_PIN, 0);      //配置输出低电平
#define MT9V034_SDA_Data  gpio_get(MT9V034_SDA_PIN)           //读取引脚上的引脚状态



void MT9V034_Init(unsigned char fps);
/*!
  * @brief    帧率设置
  *
  * @param    fps : 帧率
  *
  * @return
  *
  * @note     帧率其实是通过增加空白行实现的， 下面是实测的帧率与虚拟行之间的关系
  * @note     帧率        200    150    100    70.9    50    20    10
  * @note     虚拟行    39     92     199    330     515   1450  3000
  *
  * @see      MT9V034_SetFrameRate(50);
  *
  * @date     2019/7/26 星期五
  */
void MT9V034_SetFrameRate(unsigned char fps);
static void MT9V034_SetFrameResolution(unsigned short height,unsigned short width);
void MT9V034_SetAutoExposure(char enable);





/*********************************************************************
 *摄像头SCCB底层驱动
 *内部调用
 ***********************************************************************/
void SCCB_Init(void);
void SCCB_Wait(void);
void SCCB_Stop(void);
void SCCB_Star(void);
unsigned char SCCB_SendByte(unsigned char Data);
void SCCB_RegWrite(unsigned char Device,unsigned char Address,unsigned short Data);
unsigned char SCCB_RegRead(unsigned char Device,unsigned char Address,unsigned short *Data) ;
void MTV_IICWriteReg16(unsigned char reg, unsigned short val);


#define MT9V034_I2C_ADDR                        0xB8 //(0xB8 >> 1)=0x5C
#define MAX_IMAGE_HEIGHT            480
#define MAX_IMAGE_WIDTH             752
#define MT9V034_PIXEL_ARRAY_HEIGHT              492
#define MT9V034_PIXEL_ARRAY_WIDTH               782
#define MT9V034_CHIP_VERSION                0x00
#define MT9V034_CHIP_ID                     0x1324

#define MT9V034_COLUMN_START                        0x01
#define MT9V034_COLUMN_START_MIN                1
#define MT9V034_COLUMN_START_DEF                1
#define MT9V034_COLUMN_START_MAX                752

#define MT9V034_ROW_START                       0x02
#define MT9V034_ROW_START_MIN               4
#define MT9V034_ROW_START_DEF               4
#define MT9V034_ROW_START_MAX               482

#define MT9V034_WINDOW_HEIGHT               0x03
#define MT9V034_WINDOW_HEIGHT_MIN               1
#define MT9V034_WINDOW_HEIGHT_DEF               64
#define MT9V034_WINDOW_HEIGHT_MAX               480

#define MT9V034_WINDOW_WIDTH                        0x04
#define MT9V034_WINDOW_WIDTH_MIN                1
#define MT9V034_WINDOW_WIDTH_DEF                64
#define MT9V034_WINDOW_WIDTH_MAX                752

#define MINIMUM_HORIZONTAL_BLANKING     91 // see datasheet

#define MT9V034_HORIZONTAL_BLANKING             0x05
#define MT9V034_HORIZONTAL_BLANKING_MIN     43
#define MT9V034_HORIZONTAL_BLANKING_MAX     1023

#define MT9V034_VERTICAL_BLANKING               0x06
#define MT9V034_VERTICAL_BLANKING_MIN       4
#define MT9V034_VERTICAL_BLANKING_MAX       3000

#define MT9V034_CHIP_CONTROL                        0x07
#define MT9V034_CHIP_CONTROL_MASTER_MODE         (1 << 3)
#define MT9V034_CHIP_CONTROL_DOUT_ENABLE         (1 << 7)
#define MT9V034_CHIP_CONTROL_SEQUENTIAL     (1 << 8)

#define MT9V034_SHUTTER_WIDTH1              0x08
#define MT9V034_SHUTTER_WIDTH2              0x09
#define MT9V034_SHUTTER_WIDTH_CONTROL       0x0A
#define MT9V034_TOTAL_SHUTTER_WIDTH     0x0B
#define MT9V034_TOTAL_SHUTTER_WIDTH_MIN     1
#define MT9V034_TOTAL_SHUTTER_WIDTH_DEF     480
#define MT9V034_TOTAL_SHUTTER_WIDTH_MAX     32767

#define MT9V034_RESET                       0x0C

#define MT9V034_READ_MODE                       0x0D
#define MT9V034_READ_MODE_ROW_BIN_MASK      (3 << 0)
#define MT9V034_READ_MODE_ROW_BIN_SHIFT     0
#define MT9V034_READ_MODE_COLUMN_BIN_MASK        (3 << 2)
#define MT9V034_READ_MODE_COLUMN_BIN_SHIFT       2
#define MT9V034_READ_MODE_ROW_BIN_2         (1<<0)
#define MT9V034_READ_MODE_ROW_BIN_4         (1<<1)
#define MT9V034_READ_MODE_COL_BIN_2         (1<<2)
#define MT9V034_READ_MODE_COL_BIN_4         (1<<3)
#define MT9V034_READ_MODE_ROW_FLIP              (1 << 4)
#define MT9V034_READ_MODE_COLUMN_FLIP       (1 << 5)
#define MT9V034_READ_MODE_DARK_COLUMNS      (1 << 6)
#define MT9V034_READ_MODE_DARK_ROWS             (1 << 7)

#define MT9V034_PIXEL_OPERATION_MODE                0x0F
#define MT9V034_PIXEL_OPERATION_MODE_COLOR       (1 << 2)
#define MT9V034_PIXEL_OPERATION_MODE_HDR         (1 << 6)

#define MT9V034_V1_CTRL_REG_A           0x31
#define MT9V034_V2_CTRL_REG_A           0x32
#define MT9V034_V3_CTRL_REG_A           0x33
#define MT9V034_V4_CTRL_REG_A           0x34

#define MT9V034_ANALOG_GAIN                     0x35
#define MT9V034_ANALOG_GAIN_MIN             16
#define MT9V034_ANALOG_GAIN_DEF             16
#define MT9V034_ANALOG_GAIN_MAX             64

#define MT9V034_MAX_ANALOG_GAIN             0x36
#define MT9V034_MAX_ANALOG_GAIN_MAX             127

#define MT9V034_FRAME_DARK_AVERAGE              0x42
#define MT9V034_DARK_AVG_THRESH             0x46
#define MT9V034_DARK_AVG_LOW_THRESH_MASK         (255 << 0)
#define MT9V034_DARK_AVG_LOW_THRESH_SHIFT        0
#define MT9V034_DARK_AVG_HIGH_THRESH_MASK       (255 << 8)
#define MT9V034_DARK_AVG_HIGH_THRESH_SHIFT      8

#define MT9V034_ROW_NOISE_CORR_CONTROL      0x70
#define MT9V034_ROW_NOISE_CORR_ENABLE       (1 << 5)
#define MT9V034_ROW_NOISE_CORR_USE_BLK_AVG      (1 << 7)

#define MT9V034_PIXEL_CLOCK                 0x74
#define MT9V034_PIXEL_CLOCK_INV_LINE            (1 << 0)
#define MT9V034_PIXEL_CLOCK_INV_FRAME       (1 << 1)
#define MT9V034_PIXEL_CLOCK_XOR_LINE            (1 << 2)
#define MT9V034_PIXEL_CLOCK_CONT_LINE       (1 << 3)
#define MT9V034_PIXEL_CLOCK_INV_PXL_CLK     (1 << 4)

#define MT9V034_TEST_PATTERN                    0x7f
#define MT9V034_TEST_PATTERN_DATA_MASK      (1023 << 0)
#define MT9V034_TEST_PATTERN_DATA_SHIFT     0
#define MT9V034_TEST_PATTERN_USE_DATA       (1 << 10)
#define MT9V034_TEST_PATTERN_GRAY_MASK      (3 << 11)
#define MT9V034_TEST_PATTERN_GRAY_NONE      (0 << 11)
#define MT9V034_TEST_PATTERN_GRAY_VERTICAL      (1 << 11)
#define MT9V034_TEST_PATTERN_GRAY_HORIZONTAL        (2 << 11)
#define MT9V034_TEST_PATTERN_GRAY_DIAGONAL      (3 << 11)
#define MT9V034_TEST_PATTERN_ENABLE         (1 << 13)
#define MT9V034_TEST_PATTERN_FLIP           (1 << 14)

#define MT9V034_AEC_AGC_ENABLE          0xAF
#define MT9V034_AEC_ENABLE                  (1 << 0)
#define MT9V034_AGC_ENABLE                  (1 << 1)
#define MT9V034_THERMAL_INFO                    0xc1
#define MT9V034_ANALOG_CTRL                     (0xC2)
#define MT9V034_ANTI_ECLIPSE_ENABLE                 (1<<7)
#define MT9V034_MAX_GAIN                        (0xAB)
#define MT9V034_FINE_SHUTTER_WIDTH_TOTAL_A      (0xD5)
#define MT9V034_HDR_ENABLE_REG          0x0F
#define MT9V034_ADC_RES_CTRL_REG            0x1C
#define MT9V034_ROW_NOISE_CORR_CTRL_REG 0x70
#define MT9V034_TEST_PATTERN_REG        0x7F
#define MT9V034_TILED_DIGITAL_GAIN_REG  0x80
#define MT9V034_AGC_AEC_DESIRED_BIN_REG 0xA5
#define MT9V034_MAX_GAIN_REG            0xAB
#define MT9V034_MIN_EXPOSURE_REG        0xAC  // datasheet min coarse shutter width
#define MT9V034_MAX_EXPOSURE_REG        0xAD  // datasheet max coarse shutter width
#define MT9V034_AEC_AGC_ENABLE_REG  0xAF
#define MT9V034_AGC_AEC_PIXEL_COUNT_REG 0xB0




#endif

