#ifndef __OLED_h__
#define __OLED_h__

#define OLED_PRINTF(x,y,...) {char string[40];sprintf(string, __VA_ARGS__);oled_p6x8str(x,y,string);}
//可以像正常的printf一样在OLED上输出格式化字符串
//例如：OLED_PRINTF(0,0,"The speed is %d km/s", speed);

enum OLEDPage
{
    Camera_Page,
    UART_Debug_Page,
    Speed_Page
};

extern enum OLEDPage OLED_Page;

void My_Init_OLED(void);
void Update_OLED_per10ms(void);



#endif
