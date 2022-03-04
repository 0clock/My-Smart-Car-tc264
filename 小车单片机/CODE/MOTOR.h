#ifndef __MOTOR_h__
#define __MOTOR_h__

#define SPEED_MEASURING_PERIOD_ms 10
#define SPEED_MAX 7.0
#define SPEED_MIN -7.0
#define MOTOR_DUTY_MAX 5000

enum PID_Mode
{
    OPEN_LOOP,
    PID_CLOSED_LOOP,
    FUZZY_PID_CLOSED_LOOP
};

extern float speed_Measured;
extern float speed_Target;
extern float speed_Output;
extern enum PID_Mode PID_mode;


void UART_Speed(void);
void Set_Speed_Target(uint8 val);
void UART_FuzzyPID(void);
void UART_PID(void);
void Set_PID(uint8 v1, uint8 v2, uint8 v3, uint8 v4, uint8 v5, uint8 v6);


void My_Init_Motor(void);
void My_Init_SpeedSensor(void);
void Get_Speed_perSPEED_MEASURING_PERIOD_ms(void);
//void Get_Speed(void);
void Cal_Speed_Output(void);
void Set_Speed(void);
void Cal_Speed_Target(void);



#endif
