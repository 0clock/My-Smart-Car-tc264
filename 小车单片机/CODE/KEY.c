#include "headfile.h"
#include "KEY.h"
#include "OLED.h"//显示屏相关
#include "UART.h"


void My_Init_Key(void)
{
    gpio_init(KEY0_GPIO, GPI, 0, NO_PULL);//KEY0输入
    gpio_init(KEY1_GPIO, GPI, 0, NO_PULL);//KEY1输入
    gpio_init(KEY2_GPIO, GPI, 0, NO_PULL);//KEY2输入
    pit_interrupt_ms(CCU6_0, PIT_CH0, 10);//Check_Key_per10ms
}

//按键初始状态为0
//如果状态0或状态1的某按键低电平，计数加一
//如果状态0或状态1的某按键一旦高电平，则回到状态0，计数清零
//如果处于状态0的某按键计数达到FIRST_COUNTER_MAX，则进入状态1，并执行相应动作，计数清零
//如果处于状态1的某按键计数达到SECOND_COUNTER_MAX，则留在状态1，并执行相应动作，计数清零
//该函数要求10ms执行一次
void Check_Key_per10ms(void)
{
    static uint8 counter[3], status[3], trigger[3];
    //检查Key0
    if (gpio_get(KEY0_GPIO)==0)
    {
        counter[Key0]++;
        if (counter[Key0]>=(status[Key0]==0?STATUS0_COUNTER_MAX:STATUS1_COUNTER_MAX))
        {
            status[Key0] = 1;
            trigger[Key0] = TRUE;
            counter[Key0] = 0;
        }
    }
    else
    {
        status[Key0] = 0;
        counter[Key0] = 0;
    }
    //检查Key1
    if (gpio_get(KEY1_GPIO)==0)
    {
        counter[Key1]++;
        if (counter[Key1]>=(status[Key1]==0?STATUS0_COUNTER_MAX:STATUS1_COUNTER_MAX))
        {
            status[Key1] = 1;
            trigger[Key1] = TRUE;
            counter[Key1] = 0;
        }
    }
    else
    {
        status[Key1] = 0;
        counter[Key1] = 0;
    }
    //检查Key2
    if (gpio_get(KEY2_GPIO)==0)
    {
        counter[Key2]++;
        if (counter[Key2]>=(status[Key2]==0?STATUS0_COUNTER_MAX:STATUS1_COUNTER_MAX))
        {
            status[Key2] = 1;
            trigger[Key2] = TRUE;
            counter[Key2] = 0;
        }
    }
    else
    {
        status[Key2] = 0;
        counter[Key2] = 0;
    }
    //执行动作
    if (trigger[Key0])
    {
        Key0_Action();
        trigger[Key0] = FALSE;
    }
    if (trigger[Key1])
    {
        Key1_Action();
        trigger[Key1] = FALSE;
    }
    if (trigger[Key2])
    {
        Key2_Action();
        trigger[Key2] = FALSE;
    }
}

void Key0_Action(void)
{
    if (OLED_EN)
    {
        OLED_Page++;
        if (OLED_Page >= PAGE_NUM)
        {
            OLED_Page = 0;
        }
    }
    OLED_Page_Active_Flag = TRUE;
}

void Key1_Action(void)
{
    OLED_Page_Active_Flag = TRUE;
}

void Key2_Action(void)
{
    switch (OLED_Page)
    {
        case UART_Setting_Page:
            UART_EN = UART_EN?FALSE:TRUE;
            break;
        case OLED_Setting_Page:
            OLED_EN = OLED_EN?FALSE:TRUE;
            break;
        default:
            break;
    }
    OLED_Page_Active_Flag = TRUE;
}
