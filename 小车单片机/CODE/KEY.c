#include "headfile.h"
#include "KEY.h"
#include "OLED.h"//��ʾ�����


void My_Init_Key(void)
{
    gpio_init(KEY0_GPIO, GPI, 0, NO_PULL);//KEY0����
    gpio_init(KEY1_GPIO, GPI, 0, NO_PULL);//KEY1����
    gpio_init(KEY2_GPIO, GPI, 0, NO_PULL);//KEY2����
    pit_interrupt_ms(CCU6_0, PIT_CH0, 10);//Check_Key_per10ms
}

//������ʼ״̬Ϊ0
//���״̬0��״̬1��ĳ�����͵�ƽ��������һ
//���״̬0��״̬1��ĳ����һ���ߵ�ƽ����ص�״̬0����������
//�������״̬0��ĳ���������ﵽFIRST_COUNTER_MAX�������״̬1����ִ����Ӧ��������������
//�������״̬1��ĳ���������ﵽSECOND_COUNTER_MAX��������״̬1����ִ����Ӧ��������������
//�ú���Ҫ��10msִ��һ��
void Check_Key_per10ms(void)
{
    static uint8 counter[3], status[3], trigger[3];
    //���Key0
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
    //���Key1
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
    //���Key2
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
    //ִ�ж���
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
    static uint8 cnt;
    oled_fill(0x00);
    OLED_PRINTF(0, 0, "Key0's pressed for %d times!", ++cnt);

}

void Key1_Action(void)
{
    static uint8 cnt;
    oled_fill(0x00);
    OLED_PRINTF(0, 2, "Key1's pressed for %d times!", ++cnt);
}

void Key2_Action(void)
{
    static uint8 cnt;
    oled_fill(0x00);
    OLED_PRINTF(0, 4, "Key2's pressed for %d times!", ++cnt);
}
