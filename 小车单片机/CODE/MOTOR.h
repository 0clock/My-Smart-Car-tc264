#ifndef __MOTOR_h__
#define __MOTOR_h__

#define SPEED_MEASURING_PERIOD_ms 10
#define SPEED_MAX 7.0
#define SPEED_MIN -7.0
#define MOTOR_DUTY_MAX 5000

#define OPEN_LOOP 1
#define PID_CLOSED_LOOP 2
#define FUZZY_PID_CLOSED_LOOP 3

extern float speed_Measured;
extern float speed_Target;
extern float speed_Output;

void UART_Speed(void);
void Set_Speed_Target(uint8 val);

void My_Init_Motor(void);
void My_Init_SpeedSensor(void);
void Get_Speed_perSPEED_MEASURING_PERIOD_ms(void);
//void Get_Speed(void);
void Cal_Speed_Output(uint8 mode);
void Set_Speed(void);


#endif
