/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ����Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		����ת����ģ��
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
					    ����ת����       ��Ƭ��                        
    					RX              �鿴SEEKFREE_WIRELESS.h�ļ��е�WIRELESS_UART_TX�궨��
    					TX              �鿴SEEKFREE_WIRELESS.h�ļ��е�WIRELESS_UART_RX�궨��
    					RTS             �鿴SEEKFREE_WIRELESS.h�ļ��е�RTS_PIN�궨��
    					CMD             ���ջ�������
					------------------------------------ 
 ********************************************************************************************************************/


#include "zf_stm_systick.h"
#include "zf_gpio.h"
#include "zf_uart.h"
#include "SEEKFREE_WIRELESS.h"


uint8 wireless_send_buffer[WIRELESS_BUFFER_SIZE];
uint32 wireless_rx_index = 0;

vuint8 wireless_auto_baud_num;
vuint8 wireless_auto_baud_flag = 0;
vuint8 wireless_auto_baud_data[3] = {0x00, 0x01, 0x03};
uint8 wireless_rx_buffer;
//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ�� �����жϺ���
//  @param      void
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       �ú�����ISR�ļ� ����2�жϳ��򱻵���
//-------------------------------------------------------------------------------------------------------------------
void wireless_uart_callback(void)
{
    //��ȡ�յ�����������
	while(uart_query(WIRELESS_UART, &wireless_rx_buffer));
    if(wireless_auto_baud_flag == 1 && (wireless_auto_baud_num<3))
    {
        wireless_auto_baud_data[wireless_auto_baud_num] = wireless_rx_buffer;
        wireless_auto_baud_num++;
        if(3 == wireless_auto_baud_num)
        {
            wireless_auto_baud_flag = 2;
        }
    }
    else
    {
        wireless_send_buffer[wireless_rx_index++] = wireless_rx_buffer;
        if(wireless_rx_index == WIRELESS_BUFFER_SIZE)
            wireless_rx_index=0;
    }
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ���ʼ��
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       
//-------------------------------------------------------------------------------------------------------------------
void seekfree_wireless_init(void)
{
    //������ʹ�õĲ�����Ϊ115200��Ϊ����ת����ģ���Ĭ�ϲ����ʣ�������������������������ģ�鲢�޸Ĵ��ڵĲ�����
    wireless_type = WIRELESS_SI24R1;
    gpio_init(RTS_PIN, GPI, 0, PULLDOWN);//��ʼ����������
#if(0 == WIRELESS_AUTO_UART_BAUD)
    uart_init (WIRELESS_UART, WIRELESS_UART_BAUD, WIRELESS_UART_TX, WIRELESS_UART_RX);	//��ʼ������
#elif(1 == WIRELESS_AUTO_UART_BAUD)//�����Զ�������
    boolean temp_isr;
    gpio_init(RTS_PIN, GPI, 0, PULLDOWN);//��ʼ����������
    uint8 rts_init_status;
    rts_init_status = gpio_get(RTS_PIN);

    temp_isr = areInterruptsEnabled();
    enableInterrupts();
    gpio_init(RTS_PIN, GPO, rts_init_status, PUSHPULL);//��ʼ����������

    uart_init (WIRELESS_UART, WIRELESS_UART_BAUD, WIRELESS_UART_TX, WIRELESS_UART_RX);  //��ʼ������

    systick_delay_ms(STM0, 5);  //ģ���ϵ�֮����Ҫ��ʱ�ȴ�
    gpio_set(RTS_PIN, !rts_init_status);   //RTS�������ߣ������Զ�������ģʽ
    systick_delay_ms(STM0, 100);   //RTS����֮�������ʱ20ms
    gpio_toggle(RTS_PIN);    //RTS����ȡ��
    //gpio_set(RTS_PIN, 0);   //RTS����

    wireless_auto_baud_flag = 1;
    
    //�����ض����� ����ģ���Զ��жϲ�����
    uart_putchar(WIRELESS_UART, wireless_auto_baud_data[0]);
    uart_putchar(WIRELESS_UART, wireless_auto_baud_data[1]);
    uart_putchar(WIRELESS_UART, wireless_auto_baud_data[2]);

    systick_delay_ms(STM0, 20);
    //�����Զ��������Ƿ����
    if(2 != wireless_auto_baud_flag)
    {
        //���������뵽������ڣ�˵���Զ�������ʧ����
        systick_delay_ms(STM0, 500);
        while(1);
    }

    //�����Զ��������Ƿ���ȷ
    if(0xa5 != wireless_auto_baud_data[0] &&
       0xff != wireless_auto_baud_data[1] &&
       0xff != wireless_auto_baud_data[2] )
    {
        //���������뵽������ڣ�˵���Զ�������ʧ����
        systick_delay_ms(STM0, 500);
        while(1);
    }

    gpio_init(RTS_PIN, GPI, 0, PUSHPULL);//��ʼ����������
    systick_delay_ms(STM0, 10);//��ʱ�ȴ� ģ��׼������
    restoreInterrupts(temp_isr);
#endif

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ�� ���ͺ���
//  @param      buff        ��Ҫ���͵����ݵ�ַ
//  @param      len         ���ͳ���
//  @return     uint32      ʣ��δ���͵��ֽ���   
//  @since      v1.0
//  Sample usage:	
//  @note       
//-------------------------------------------------------------------------------------------------------------------
uint32 seekfree_wireless_send_buff(uint8 *buff, uint32 len)
{
    while(len>30)
    {
//        if(gpio_get(RTS_PIN))
//        {
//            return len;//ģ��æ,�������ǰ����ʹ��while�ȴ� �����ʹ�ú���ע�͵�while�ȴ�����滻��if���
//        }
        while(gpio_get(RTS_PIN));  //���RTSΪ�͵�ƽ���������������
        uart_putbuff(WIRELESS_UART,buff,30);

        buff += 30; //��ַƫ��
        len -= 30;//����
    }
    
//    if(gpio_get(RTS_PIN))
//    {
//        return len;//ģ��æ,�������ǰ����ʹ��while�ȴ� �����ʹ�ú���ע�͵�while�ȴ�����滻��if���
//    }
    while(gpio_get(RTS_PIN));  //���RTSΪ�͵�ƽ���������������
    uart_putbuff(WIRELESS_UART,buff,len);//������������
    
    return 0;
}
