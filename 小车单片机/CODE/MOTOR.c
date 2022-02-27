#include "headfile.h"
#include "MOTOR.h"
#include "IfxGpt12_reg.h"
#include "IfxGpt12.h"
#include "fuzzy_PID.h"//模糊PID算法




//需要串口通信输出去，但不用传过来的变量
float speed_Measured = 0;//测量速度（m/s）


//需要串口通信传过来的变量（必须配以执行变量更新的函数）
float speed_Target = 0;//目标速度（m/s），更新函数Set_Speed_Target(uint8 val)


//其他变量
float speed_Output = 0;//输出速度（m/s）
float PID_KP=7.0f/0.5f;//7.0f/3.0f;
float PID_KI=0.04f;//0.01f;
float PID_KD=0.0f;

enum PID_Mode PID_mode = PID_CLOSED_LOOP;//PID模式选择

void My_Init_Motor(void)
{
    gtm_pwm_init(ATOM0_CH6_P23_1, 12500, 5000);//设置P23.1输出PWM波，频率12.5kHz，占空比5000/10000；用于直流电机正端
    gtm_pwm_init(ATOM1_CH5_P32_4, 12500, 5000);//设置P32.4输出PWM波，频率12.5kHz，占空比5000/10000；用于直流电机负端
}

void My_Init_SpeedSensor(void)
{
    gpt12_init(GPT12_T2, GPT12_T2INB_P33_7, GPT12_T2EUDB_P33_6);
    pit_interrupt_ms(CCU6_1, PIT_CH0, SPEED_MEASURING_PERIOD_ms);
}

void UART_Speed(void)
{
    uart_putchar(DEBUG_UART,0x00);
    uart_putchar(DEBUG_UART,0xff);
    uart_putchar(DEBUG_UART,0x04);
    uart_putchar(DEBUG_UART,0x01);//发送数据头
    uart_putchar(DEBUG_UART, (uint8)round((speed_Measured - SPEED_MIN)/(SPEED_MAX-SPEED_MIN)*255));
    uart_putchar(DEBUG_UART, (uint8)round((speed_Output - SPEED_MIN)/(SPEED_MAX-SPEED_MIN)*255));
    uart_putchar(DEBUG_UART, (uint8)round((speed_Target - SPEED_MIN)/(SPEED_MAX-SPEED_MIN)*255));
    uart_putchar(DEBUG_UART, (uint8)round((pid_vector[0]->last_error - SPEED_MIN)/(SPEED_MAX-SPEED_MIN)*255));
    uart_putchar(DEBUG_UART, (uint8)round((pid_vector[0]->current_error - SPEED_MIN)/(SPEED_MAX-SPEED_MIN)*255));
    uart_putchar(DEBUG_UART,0x00);
    uart_putchar(DEBUG_UART,0xff);
    uart_putchar(DEBUG_UART,0x04);
    uart_putchar(DEBUG_UART,0x02);//发送数据尾
}

void Set_Speed_Target(uint8 val)
{
    speed_Target = ((float)val) / 256 * (SPEED_MAX-SPEED_MIN)+SPEED_MIN;//因为速度范围是对称的，而255不对称，所以接收时用256
}

void UART_FuzzyPID(void)
{
    uart_putchar(DEBUG_UART,0x00);
    uart_putchar(DEBUG_UART,0xff);
    uart_putchar(DEBUG_UART,0x06);
    uart_putchar(DEBUG_UART,0x01);//发送数据头
    int16 kp = (int16)round(10000*(pid_vector[0]->kp+pid_vector[0]->delta_kp));
    int16 ki = (int16)round(10000*(pid_vector[0]->ki+pid_vector[0]->delta_ki));
    int16 kd = (int16)round(10000*(pid_vector[0]->kd+pid_vector[0]->delta_kd));
    uart_putchar(DEBUG_UART, kp>>8);//先传高8位，再传低8位
    uart_putchar(DEBUG_UART, kp&0x00FF);//先传高8位，再传低8位
    uart_putchar(DEBUG_UART, ki>>8);//先传高8位，再传低8位
    uart_putchar(DEBUG_UART, ki&0x00FF);//先传高8位，再传低8位
    uart_putchar(DEBUG_UART, kd>>8);//先传高8位，再传低8位
    uart_putchar(DEBUG_UART, kd&0x00FF);//先传高8位，再传低8位
    uart_putchar(DEBUG_UART,0x00);
    uart_putchar(DEBUG_UART,0xff);
    uart_putchar(DEBUG_UART,0x06);
    uart_putchar(DEBUG_UART,0x02);//发送数据尾
}

void UART_PID(void)
{
    uart_putchar(DEBUG_UART,0x00);
    uart_putchar(DEBUG_UART,0xff);
    uart_putchar(DEBUG_UART,0x07);
    uart_putchar(DEBUG_UART,0x01);//发送数据头
    int16 kp = (int16)round(1000*PID_KP);
    int16 ki = (int16)round(1000*PID_KI);
    int16 kd = (int16)round(1000*PID_KD);
    uart_putchar(DEBUG_UART, kp>>8);//先传高8位，再传低8位
    uart_putchar(DEBUG_UART, kp&0x00FF);//先传高8位，再传低8位
    uart_putchar(DEBUG_UART, ki>>8);//先传高8位，再传低8位
    uart_putchar(DEBUG_UART, ki&0x00FF);//先传高8位，再传低8位
    uart_putchar(DEBUG_UART, kd>>8);//先传高8位，再传低8位
    uart_putchar(DEBUG_UART, kd&0x00FF);//先传高8位，再传低8位
    uart_putchar(DEBUG_UART,0x00);
    uart_putchar(DEBUG_UART,0xff);
    uart_putchar(DEBUG_UART,0x07);
    uart_putchar(DEBUG_UART,0x02);//发送数据尾
}

void Set_PID(uint8 v1, uint8 v2, uint8 v3, uint8 v4, uint8 v5, uint8 v6)
{
    PID_KP = (v1*256.0f+v2)/1000.0f;
    PID_KI = (v3*256.0f+v4)/1000.0f;
    PID_KD = (v5*256.0f+v6)/1000.0f;
}

void Get_Speed_perSPEED_MEASURING_PERIOD_ms(void)
{
    uint16 count = gpt12_get(GPT12_T2);
    gpt12_clear(GPT12_T2);
    if (count <= 32767)
    {
        speed_Measured = ((float)count)*0.033*2*3.1415926/(1.0*SPEED_MEASURING_PERIOD_ms/1000.0)/1024.0;
    }
    else
    {
        speed_Measured = ((float)count-65536)*0.033*2*3.1415926/(1.0*SPEED_MEASURING_PERIOD_ms/1000.0)/1024.0;
    }
    // (编码器计数/计数周期)/1024 = 转速（转/s），(编码器计数/计数周期)/1024*0.033*2*3.1415926 = 速度（m/s）
}


/*通过T5的Capture模式来测速
void My_Init_SpeedSensor(void)
{
    IfxGpt12_enableModule(&MODULE_GPT120);
    IfxGpt12_setGpt2BlockPrescaler(&MODULE_GPT120, IfxGpt12_Gpt2BlockPrescaler_4);
    IfxPort_setPinModeInput(IfxGpt120_CAPINA_P13_2_IN.pin.port, IfxGpt120_CAPINA_P13_2_IN.pin.pinIndex, IfxPort_InputMode_pullUp);
    IfxPort_setPinPadDriver(IfxGpt120_CAPINA_P13_2_IN.pin.port, IfxGpt120_CAPINA_P13_2_IN.pin.pinIndex, IfxPort_PadDriver_cmosAutomotiveSpeed1);
    IfxGpt12_initTxEudInPinWithPadLevel(&IfxGpt120_T5EUDB_P10_1_IN, IfxPort_InputMode_pullUp, IfxPort_PadDriver_cmosAutomotiveSpeed1);
    IfxGpt12_T5_setCaptureTrigger(&MODULE_GPT120, IfxGpt12_CaptureTrigger_capin);
    IfxGpt12_T5_setCaptureTriggerMode(&MODULE_GPT120, IfxGpt12_CaptureTriggerMode_risingEdge);
    IfxGpt12_T5_enableClearTimer(&MODULE_GPT120, 1);
    IfxGpt12_T5_setCaptureTriggerEnable(&MODULE_GPT120, 1);
    IfxGpt12_T5_setDirectionSource(&MODULE_GPT120, IfxGpt12_TimerDirectionSource_external);
    IfxGpt12_T5_setMode(&MODULE_GPT120, IfxGpt12_Mode_timer);
    IfxGpt12_T5_run(&MODULE_GPT120, IfxGpt12_TimerRun_start);
}

void Get_Speed(void)
{
    speed_Measured = (float)(uint16)MODULE_GPT120.CAPREL.U;
}
*/


//由speed_Target，speed_Measured得到speed_Output
void Cal_Speed_Output(void)
{

    if (PID_mode == OPEN_LOOP)
    {
        speed_Output = speed_Target;
    }
    else if (PID_mode == PID_CLOSED_LOOP)
    {
        static int flag = 3;
        flag--;
        static float speed_Error[3];
        speed_Error[2] = speed_Error[1];
        speed_Error[1] = speed_Error[0];
        speed_Error[0] = speed_Target - speed_Measured;
//        if (speed_Error[0]<0.1 && speed_Error[0]>-0.1)
//        {
//            speed_Error[0] = 0;
//        }//带死区的增量式pid
        if (flag == 0)
        {
            flag++;
            float delta_Speed = PID_KP*(speed_Error[0]-speed_Error[1]) + PID_KI*speed_Error[0] + PID_KD*(speed_Error[0]-2*speed_Error[1]+speed_Error[2]);
            speed_Output = speed_Output + delta_Speed;
        }
    }
    else if (PID_mode == FUZZY_PID_CLOSED_LOOP)
    {
        speed_Output = fuzzy_pid_control(speed_Measured, speed_Target, pid_vector[0]);
    }

}



void Set_Speed(void)
{
    uint32 duty;
    duty = (speed_Output>0?speed_Output:(-speed_Output))*500 +1500;//地面开环用它
    if (duty>MOTOR_DUTY_MAX)//保护电机
    {
        duty = MOTOR_DUTY_MAX;
    }
    if (speed_Output == 0)
    {
        duty = 0;
    }
    pwm_duty(ATOM0_CH6_P23_1, (uint32)(5000-(speed_Output>0?1:(-1))*duty));
}


