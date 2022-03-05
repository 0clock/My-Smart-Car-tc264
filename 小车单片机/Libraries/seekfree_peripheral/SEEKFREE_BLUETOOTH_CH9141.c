/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2021,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		��ɿƼ�����ת����ģ��
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.33
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2021-08-27
 * @note		
					���߶��壺
					------------------------------------ 
					    ����ת����      ��Ƭ��                        
    					RX              �鿴SEEKFREE_BLUETOOTH_CH9141.h�ļ��е�BLUETOOTH_CH9141_UART_TX�궨��
    					TX              �鿴SEEKFREE_BLUETOOTH_CH9141.h�ļ��е�BLUETOOTH_CH9141_UART_RX�궨��
    					RTS             �鿴SEEKFREE_BLUETOOTH_CH9141.h�ļ��е�BLUETOOTH_CH9141_RTS_PIN�궨��
                        CTS             ����
    					CMD             ���ջ�������
					------------------------------------ 
 ********************************************************************************************************************/
#include "zf_stm_systick.h"
#include "zf_gpio.h"
#include "zf_uart.h"
#include "zf_assert.h"
#include "SEEKFREE_BLUETOOTH_CH9141.h"

uint8 uart_flag;
uint8 uart_data;

vuint8 at_mode = 0;         //0:����͸��ģʽ 1:ATģʽ 2:ģ�鸴λ��
vuint8 at_mode_num;         //atģʽʱ����ָʾ���ݽ��յ�����
vuint8 at_mode_data[30];    //����at����Ļ���
vuint8 at_mode_cmd_flag;    //OKӦ��������ճɹ��ı�־λ

uint8 mac_address[17];      //����mac��ַ


uint8   bluetooth_ch9141_rx_buffer;


void bluetooth_ch9141_check_response(void);

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ�� �����жϻص�����
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       �ú�����ISR�ļ� ����8�жϳ��򱻵���
//-------------------------------------------------------------------------------------------------------------------
void bluetooth_ch9141_uart_callback()
{
    while(uart_query(BLUETOOTH_CH9141_UART, &bluetooth_ch9141_rx_buffer))
    {
        if(1 == at_mode)
        {
            //����ATģʽ ����Ӧ���ź� �˴�if����ڴ����û���Ҫ�Ķ�
            at_mode_data[at_mode_num++] = bluetooth_ch9141_rx_buffer;
            bluetooth_ch9141_check_response();
        }
        else if(2 == at_mode)
        {
            //ģ�����ڸ�λ�� �˴�if����ڴ����û���Ҫ�Ķ�
            at_mode_num++;
        }
        else
        {
            //͸��ģʽ �û��ڴ˴�������Ե��������͹����Ķ�����
            //�ӵ�һ���ֽں�Ƭ���������˴���ͨ���ڴ˴���ȡbluetooth_ch9141_rx_buffer����ȡ������

            // ��ȡ���ߴ��ڵ����� ������λ���ձ�־
            uart_flag = 1;
            uart_data = bluetooth_ch9141_rx_buffer;
        }
        
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ����OKӦ���ź�
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       �û��������
//-------------------------------------------------------------------------------------------------------------------
void bluetooth_ch9141_check_response(void)
{
    if(4 <= at_mode_num)
    {
        if(0 == strncmp("OK\r\n", (int8 *)&at_mode_data[at_mode_num-4], 4))
        {
            at_mode_cmd_flag = 1;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ�鷢��һ��û�в���������ȴ�Ӧ���ź�
//  @param      *str    ��Ҫ���͵����� �����ַ���
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       �û��������
//-------------------------------------------------------------------------------------------------------------------
void bluetooth_ch9141_send_at_command(const int8 *str)
{
    at_mode_num = 0;        //������������
    uart_putstr(BLUETOOTH_CH9141_UART, str);
    uart_putstr(BLUETOOTH_CH9141_UART, "\r\n");
    
    //�ȴ��յ�Ӧ���ź�
    while(!at_mode_cmd_flag);
    at_mode_cmd_flag = 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ�鷢��һ�����в���������ȴ�Ӧ���ź�
//  @param      *cmd    ��Ҫ���͵���������
//  @param      *data   ��Ҫ���͵�����
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       �û��������
//-------------------------------------------------------------------------------------------------------------------
void bluetooth_ch9141_send_at_command_parameter(const int8 *cmd, const int8 *data)
{
    at_mode_num = 0;        //������������
    uart_putstr(BLUETOOTH_CH9141_UART, "AT+");
    uart_putstr(BLUETOOTH_CH9141_UART, cmd);
    uart_putstr(BLUETOOTH_CH9141_UART, "=");
    uart_putstr(BLUETOOTH_CH9141_UART, data);
    uart_putstr(BLUETOOTH_CH9141_UART, "\r\n");
    
    //�ȴ��յ�Ӧ���ź�
    while(!at_mode_cmd_flag);
    at_mode_cmd_flag = 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ�����ATģʽ
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       
//-------------------------------------------------------------------------------------------------------------------
void bluetooth_ch9141_enter_at_mode(void)
{
    systick_delay_ms(STM0, 550);  //���ͽ���ATģʽ������ǰ��Ҫ��֤ģ����550ms��û�н��չ��κ�����
    at_mode = 1;            //����ATģʽ
    bluetooth_ch9141_send_at_command("AT...");
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ���˳�ATģʽ
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       
//-------------------------------------------------------------------------------------------------------------------
void bluetooth_ch9141_exit_at_mode(void)
{
    bluetooth_ch9141_send_at_command("AT+EXIT");
    at_mode = 0;            //����͸��ģʽ
    systick_delay_ms(STM0, 300);  //�ȴ��ɹ�����ATģʽ
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ�鸴λ
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       
//-------------------------------------------------------------------------------------------------------------------
void bluetooth_ch9141_reset(void)
{
    bluetooth_ch9141_send_at_command("AT+RESET");
    at_mode = 2;            //���������ɹ����
    at_mode_num = 0;
    while(7 > at_mode_num); //�ȴ�����ģ����ɸ�λ
    at_mode = 0;            //��λ֮��ģ���Զ�����͸��ģʽ
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ���ȡ����MAC��ַ
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       ���ô˺�����Ҫ�ȵ���bluetooth_ch9141_enter_at_mode��������CMD���� ����ATģʽ
//              ��Ҫ�ر�ע��bluetooth_ch9141_enter_at_mode�����ڲ���500ms����ʱ
//-------------------------------------------------------------------------------------------------------------------
void bluetooth_ch9141_get_mac_address(void)
{
    bluetooth_ch9141_send_at_command("AT+MAC?");
    
    //mac��ַΪС�θ�ʽ��mac_address[0]�������mac��ַ���λ
    memcpy(mac_address, (uint8 *)at_mode_data, 17);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ�����÷��͹���
//  @param      tx_power    ���÷��͹��ʣ�������ѡ��鿴CH9141_TX_POWEER_enumö�ٳ�Ա
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       
//-------------------------------------------------------------------------------------------------------------------
void bluetooth_ch9141_set_tx_power(CH9141_TX_POWEER_enum tx_power)
{
    int8 tx_power_data;
    
    tx_power_data = (uint8)tx_power + '0';
    bluetooth_ch9141_send_at_command_parameter("TPL", &tx_power_data);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ������ģʽ
//  @param      mode    ģʽ���ã�������ѡ��鿴CH9141_MODE_enumö�ٳ�Ա
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       
//-------------------------------------------------------------------------------------------------------------------
void bluetooth_ch9141_set_mode(CH9141_MODE_enum mode)
{
    int8 mode_data;
    
    mode_data = (uint8)mode + '0';
    bluetooth_ch9141_send_at_command_parameter("BLEMODE", &mode_data);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ���ȡ״̬
//  @param      mode    ģʽ���ã�������ѡ��鿴CH9141_MODE_enumö�ٳ�Ա
//  @return     CH9141_STATUS_enum  ����״̬��Ϣ
//  @since      v1.0
//  Sample usage:	
//  @note       
//-------------------------------------------------------------------------------------------------------------------
CH9141_STATUS_enum bluetooth_ch9141_get_status(CH9141_MODE_enum mode)
{
    CH9141_STATUS_enum ch9141_status;
    int8 mode_data;
    
    mode_data = (uint8)mode + '0';
    bluetooth_ch9141_send_at_command_parameter("BLEMODE", &mode_data);
    
    bluetooth_ch9141_send_at_command("AT+BLESTA?");
    
    ch9141_status = (at_mode_data[0] - '0') * 10 + (at_mode_data[1] - '0');
    if(SLAVE_MODE == mode)
    {
        ch9141_status += SLAVE_NO_INIT;
    }
    
    return ch9141_status;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ�������豸����
//  @param      *str    ��������
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       ���Ƴ��Ȳ��ܳ���18���ַ� ��ֻ��ΪӢ��������
//-------------------------------------------------------------------------------------------------------------------
void bluetooth_ch9141_set_name(const int8 *str)
{
    bluetooth_ch9141_send_at_command_parameter("NAME", str);
    bluetooth_ch9141_send_at_command_parameter("PNAME", str);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ����������
//  @param      enable      ʹ������ 0����ʹ�����룬1��ʹ������������ӱ��豸
//  @param      *password   ������ַ��� ����Ϊ6���ַ�
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       
//-------------------------------------------------------------------------------------------------------------------
void bluetooth_ch9141_set_password(uint8 enable, const int8 *password)
{
    if(0 == enable)
    {
        //�ر�����
        bluetooth_ch9141_send_at_command_parameter("PASEN", "OFF");
    }
    else
    {
        //�������벢ʹ��
        bluetooth_ch9141_send_at_command_parameter("PASEN", "ON");
        bluetooth_ch9141_send_at_command_parameter("PASS", password);
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ��ָ��MAC��ַ��������������
//  @param      *mac_and_password      ��Ҫ���ӵ��豸mac��ַ������
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       bluetooth_ch9141_connect("58:B7:33:E4:C2:84,000000");
//              58:B7:33:E4:C2:84Ϊmac��ַ ,Ϊ�ָ��� 000000Ϊ�ӻ���������
//              ===================�ر�ע��==================
//              ���ʹ���ֻ��鿴������mac��ַ����ʹ�ñ��������ӵ�ʱ���뽫mac����һ��
//              �����ֻ��鿴����mac��ַΪ61:62:63:64:65:66����ʹ�ñ��������ӵ�ʱ��Ӧ��д
//              bluetooth_ch9141_connect("66:65:64:63:62:61,000000");
//-------------------------------------------------------------------------------------------------------------------
void bluetooth_ch9141_connect(const int8 *mac_and_password)
{
    bluetooth_ch9141_send_at_command_parameter("CONN", mac_and_password);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ��Ĭ�����Ӳ������ã����úú�ÿ�ο����������Զ���������豸��
//  @param      *mac_and_password      ��Ҫ���ӵ��豸mac��ַ������
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       bluetooth_ch9141_default_connect("58:B7:33:E4:C2:84,000000");
//              58:B7:33:E4:C2:84Ϊmac��ַ ,Ϊ�ָ��� 000000Ϊ�ӻ���������
//              ===================�ر�ע��==================
//              ���ʹ���ֻ��鿴CH9141��mac��ַ����CH9141����Ϊ�ӻ���ʹ���ֻ�����
//              ��ʹ�ñ��������ӵ�ʱ���뽫mac����һ��
//              �����ֻ��鿴����mac��ַΪ61:62:63:64:65:67����ʹ�ñ��������ӵ�ʱ��Ӧ��д
//              bluetooth_ch9141_default_connect("67:65:64:63:62:61,000000");
//-------------------------------------------------------------------------------------------------------------------
void bluetooth_ch9141_default_connect(const int8 *mac_and_password)
{
    bluetooth_ch9141_send_at_command_parameter("CONADD", mac_and_password);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ���ȡrssi(�ź�ǿ��)
//  @param      void      
//  @return     int8    �����ź�ǿ��0��-127
//  @since      v1.0
//  Sample usage:	
//  @note       ���ô˺�����Ҫ�ȵ���bluetooth_ch9141_enter_at_mode��������CMD���� ����ATģʽ
//              ��Ҫ�ر�ע��bluetooth_ch9141_enter_at_mode�����ڲ���500ms����ʱ
//-------------------------------------------------------------------------------------------------------------------
int16 bluetooth_ch9141_get_rssi(void)
{
    uint8 i;
    size_t length;
    int16 rssi;
    bluetooth_ch9141_send_at_command_parameter("RSSI", "ON,0");
    length = strlen((int8 *)at_mode_data);
    length -= 12;//����RSSI �ж���λ
    
    rssi = 0;
    for(i=0; i<length; i++)
    {
        rssi = rssi*10 + (at_mode_data[0] - '0');
    }
    
    return -rssi;
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
uint32 bluetooth_ch9141_send_buff(uint8 *buff, uint32 len)
{
    while(len)
    {
        //���ؼ�� RTSΪ�߱�ʾ����ģ���ڲ����������޷�������������
        
        //RTSΪ�ߴ���ʽһ�������⵽RTSΪ�����������ݲ��ټ������ͣ�������ֵȴ�
        if(gpio_get(BLUETOOTH_CH9141_RTS_PIN))  
        {
            break;
        }
        
        //RTSΪ�ߴ���ʽ���������⵽RTSΪ����ȴ�RTSΪ��֮�������������
        //while(gpio_get(BLUETOOTH_CH9141_RTS_PIN));  //���RTSΪ�͵�ƽ���������������
        
        //��������
        uart_putchar(BLUETOOTH_CH9141_UART, *buff);

        buff++;
        len--; 
    }

    return len;
}





//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ���ʼ��
//  @param      mode    ����ģʽ MASTER_MODE(����)����SLAVE_MODE(�ӻ�)
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       
//				���ʹ���ֻ��鿴������mac��ַ����ʹ�ñ��������ӵ�ʱ���뽫mac����һ��
//				�����ֻ��鿴����mac��ַΪ61:62:63:64:65:66����ʹ�ñ��������ӵ�ʱ��Ӧ��д
//				bluetooth_ch9141_connect("66:65:64:63:62:61,000000");
//				58:B7:33:E4:C2:84Ϊmac��ַ ,Ϊ�ָ��� 000000Ϊ�ӻ���������
//				�ӻ�MAC��ַ������
//				int8 salve_mac_password[] = "80:90:2C:E4:C2:84,000000";
//-------------------------------------------------------------------------------------------------------------------
void bluetooth_ch9141_init(CH9141_MODE_enum mode, int8 *salve_mac_password)
{
    int8 *str = salve_mac_password;

    if(str != NULL)
    {
       while(*str)
       {
           if(*str++ == 0x20)
           {
               //�������пո�
               ZF_ASSERT(0);
               return;
           }
       }
    }
    boolean irq_status;
    //������ʹ�õĲ�����Ϊ115200��Ϊ����ת����ģ���Ĭ�ϲ����ʣ�����������������ʹ����λ���޸�ģ�����
    //��ʼ����������
    wireless_type = WIRELESS_CH9141;

    gpio_init(BLUETOOTH_CH9141_RTS_PIN, GPI, 0, PULLDOWN);
    //��ʼ������
    uart_init (BLUETOOTH_CH9141_UART, BLUETOOTH_CH9141_UART_BAUD, BLUETOOTH_CH9141_UART_TX, BLUETOOTH_CH9141_UART_RX);	//��ʼ������    
    irq_status = areInterruptsEnabled();
    enableInterrupts();
    
    //������Ϊ������ӻ�ģʽ������������Ҫ���ӳɹ��ͱ�����һ��Ϊ��������һ��Ϊ�ӻ������Ե��ó�ʼ����ʱ����Ҫ�������д�����������ܳɹ�������
    //������Ϊ������ӻ�ģʽ������������Ҫ���ӳɹ��ͱ�����һ��Ϊ��������һ��Ϊ�ӻ������Ե��ó�ʼ����ʱ����Ҫ�������д�����������ܳɹ�������
    //�ϵ�˳����ôӻ����ϵ磬Ȼ���������ϵ�

    if(MASTER_MODE == mode)
    {
        //1.����������Ϊ����ģʽ��Ȼ������ָ��mac��ַ�Ĵӻ��豸
        bluetooth_ch9141_enter_at_mode();  //����ATģʽ
        bluetooth_ch9141_set_mode(mode);   //��������ģʽ
        bluetooth_ch9141_get_mac_address();//��ȡ����MAC��ַ
		bluetooth_ch9141_reset();                  //������ɺ���Ҫ��λ�����òŻ���Ч
		bluetooth_ch9141_enter_at_mode();          //����ATģʽ

		//������ģʽ֮����Ҫ��λȻ���ٴν���ATģʽ���ܼ���������������������ģʽ���ò��ɹ�
        bluetooth_ch9141_set_tx_power(TX_POWER_4DB);//�����������͹���
        //===================�ر�ע��==================
        //���ʹ���ֻ��鿴������mac��ַ����ʹ�ñ��������ӵ�ʱ���뽫mac����һ��
        //�����ֻ��鿴����mac��ַΪ61:62:63:64:65:66����ʹ�ñ��������ӵ�ʱ��Ӧ��д
        //bluetooth_ch9141_connect("66:65:64:63:62:61,000000");
        //58:B7:33:E4:C2:84Ϊmac��ַ ,Ϊ�ָ��� 000000Ϊ�ӻ���������
        //�ӻ�MAC��ַ������
        //int8 salve_mac_password[] = "80:90:2C:E4:C2:84,000000";
        
        bluetooth_ch9141_default_connect(salve_mac_password);  //����Ĭ�����Ӳ�������ʹ�´β�����Ҳ���Զ����Ӵӻ�
        bluetooth_ch9141_connect(salve_mac_password);          //�����������õĴӻ���ַ
        
        //�ȴ����ӳɹ�
        while(MASTER_CONNECTED != bluetooth_ch9141_get_status(mode));
        bluetooth_ch9141_exit_at_mode();                       //�˳�ATģʽ
    }
    else if(SLAVE_MODE == mode)
    {
        //2.��������Ϊ�ӻ����ȴ�����
        bluetooth_ch9141_enter_at_mode();  //����ATģʽ
        bluetooth_ch9141_set_mode(mode);   //��������ģʽ
        bluetooth_ch9141_get_mac_address();//��ȡ����MAC��ַ
		bluetooth_ch9141_reset();                  //������ɺ���Ҫ��λ�����òŻ���Ч
		bluetooth_ch9141_enter_at_mode();          //����ATģʽ

		//������ģʽ֮����Ҫ��λȻ���ٴν���ATģʽ���ܼ���������������������ģʽ���ò��ɹ�
        bluetooth_ch9141_set_tx_power(TX_POWER_4DB);//�����������͹���
        bluetooth_ch9141_set_name("ble");
        bluetooth_ch9141_set_password(1, "000000");  //000000Ϊ������������Լ��޸�
        bluetooth_ch9141_reset();                  //������ɺ���Ҫ��λ�����òŻ���Ч
        bluetooth_ch9141_enter_at_mode();          //����ATģʽ
        //�ȴ����ӳɹ�
        while(SLAVE_CONNECTED != bluetooth_ch9141_get_status(mode));
        bluetooth_ch9141_exit_at_mode();           //�˳�ATģʽ
    }
    
    //������ȡ�����ź�ǿ�ȿ��԰��������ʾ������
    //bluetooth_ch9141_enter_at_mode();
    //int16 rssi = bluetooth_ch9141_get_rssi();
    
    disableInterrupts();
    restoreInterrupts(irq_status);
}


