#ifndef __UART_h__
#define __UART_h__

#define RECEIVE_LENGTH ((4+1+4 + 4+1+4 + 4+4+4 + 4+1+4 + 4+1+4 + 4+6+4)*2)
#define CACHE_LENGTH 0

enum UARTstate
{
    Read,
    Send
};

extern uint8 data_Buffer[RECEIVE_LENGTH + CACHE_LENGTH];
extern uint8 *dat;
extern uint8 UART_Flag_RX;
extern uint8 UART_Flag_TX;
extern uint8 UART_EN;
extern uint8 data_Buffer_Shadow[RECEIVE_LENGTH];
extern uint8 UART_Flag_NO_IMAGE;

void My_Init_UART(void);
void UART(enum UARTstate state);


#endif
