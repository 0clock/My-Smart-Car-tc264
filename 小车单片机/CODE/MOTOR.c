#include "headfile.h"
#include "MOTOR.h"
#include "IfxGpt12_reg.h"
#include "IfxGpt12.h"
#include "fuzzy_PID.h"//ģ��PID�㷨




//��Ҫ����ͨ�����ȥ�������ô������ı���
float speed_Measured = 0;//�����ٶȣ�m/s��


//��Ҫ����ͨ�Ŵ������ı�������������ִ�б������µĺ�����
float speed_Target = 0;//Ŀ���ٶȣ�m/s�������º���Set_Speed_Target(uint8 val)
float PID_KP=289;
float PID_KI=94;
float PID_KD=65;

//��������
float speed_Output = 0;//����ٶȣ�m/s��


void My_Init_Motor(void)
{
    gtm_pwm_init(ATOM0_CH6_P23_1, 12500, 5000);//����P23.1���PWM����Ƶ��12.5kHz��ռ�ձ�5000/10000������ֱ���������
    gtm_pwm_init(ATOM1_CH5_P32_4, 12500, 5000);//����P32.4���PWM����Ƶ��12.5kHz��ռ�ձ�5000/10000������ֱ���������
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
    uart_putchar(DEBUG_UART,0x01);//��������ͷ
    uart_putchar(DEBUG_UART, (uint8)round((speed_Measured - SPEED_MIN)/(SPEED_MAX-SPEED_MIN)*255));
    uart_putchar(DEBUG_UART, (uint8)round((speed_Target - SPEED_MIN)/(SPEED_MAX-SPEED_MIN)*255));
    uart_putchar(DEBUG_UART,0x00);
    uart_putchar(DEBUG_UART,0xff);
    uart_putchar(DEBUG_UART,0x04);
    uart_putchar(DEBUG_UART,0x02);//��������β
}

void Set_Speed_Target(uint8 val)
{
    speed_Target = ((float)val) / 256 * (SPEED_MAX-SPEED_MIN)+SPEED_MIN;//��Ϊ�ٶȷ�Χ�ǶԳƵģ���255���Գƣ����Խ���ʱ��256
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
    // (����������/��������)/1024 = ת�٣�ת/s����(����������/��������)/1024*0.033*2*3.1415926 = �ٶȣ�m/s��
}


/*ͨ��T5��Captureģʽ������
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


//��speed_Target��speed_Measured�õ�speed_Output
void Cal_Speed_Output(uint8 mode)
{
    //���ٱ�����������SPEED_MAX��SPEED_MIN��
    float true_Speed_Target;
    if (speed_Target > SPEED_MIN && speed_Target < SPEED_MAX)
    {
        true_Speed_Target = speed_Target;
    }
    else
    {
        true_Speed_Target = speed_Target>0?SPEED_MAX:SPEED_MIN;
    }


    if (mode == OPEN_LOOP)
    {
        speed_Output = true_Speed_Target;
    }
    else if (mode == PID_CLOSED_LOOP)
    {
        static int flag = 3;
        flag--;
        static float speed_Error[3];
        speed_Error[2] = speed_Error[1];
        speed_Error[1] = speed_Error[0];
        speed_Error[0] = true_Speed_Target - speed_Measured;
        if (flag == 0)
        {
            flag++;
            float delta_Speed = PID_KP*(speed_Error[0]-speed_Error[1]) + PID_KI*speed_Error[0] + PID_KD*(speed_Error[0]-2*speed_Error[1]+speed_Error[2]);
            speed_Output = speed_Output + delta_Speed;
        }
    }
    else if (mode == FUZZY_PID_CLOSED_LOOP)
    {
        speed_Output = fuzzy_pid_control(speed_Measured, true_Speed_Target, pid_vector[0]);
    }

}



void Set_Speed(void)
{
    uint32 duty;
    //duty = 34.243speed_Output^2 + 321.79speed_Output + 1339.2
    //R-Square = 0.9944
    //duty = 34.243*speed_Output*speed_Output + 321.79*(speed_Output>0?speed_Output:(-speed_Output)) + 1339.2;//��ת��������
    duty = (speed_Output>0?speed_Output:(-speed_Output))*500 +1500;//���濪������
    if (duty>MOTOR_DUTY_MAX)//�������
    {
        duty = MOTOR_DUTY_MAX;
    }
    if (speed_Output == 0)
    {
        duty = 0;
    }
    pwm_duty(ATOM0_CH6_P23_1, (uint32)(5000-(speed_Output>0?1:(-1))*duty));
}


