#include "headfile.h"
#include "STEERING.h"
#include "CAMERA.h"

extern float Col_Center[height_Inverse_Perspective_Max];//���������ϵ�˳��洢�������ߵ��кŽ�������Ϸ���ȫ��Ϊ-2
extern int search_Lines;//ɨ�跶Χ

float steering_Error = 0;//��ǰͼ���µ�ʵ�����������������ߵ����

struct
{
    float last_error;//�ϴ�
    float current_error; //��ǰ
    float KP;
    float KI;
    float KD;
}Steering_PID={0.0f,0.0f,30.0/200.0f,0.0f,0.0f};


//��Ҫ����ͨ�Ŵ������ı�������������ִ�б������µĺ�����
float steering_Target = 0;//Ŀ��Ƕȣ��㣩�����º���Set_Steering_Target(uint8 val)����Χ-30��30��




void My_Init_Steering(void)
{
    gtm_pwm_init(ATOM2_CH0_P33_10, 100, 1500);//����P33.10���PWM����Ƶ��100Hz��ռ�ձ�1500/10000�����ڶ��
}

void UART_Steering(void)
{
    uart_putchar(DEBUG_UART,0x00);
    uart_putchar(DEBUG_UART,0xff);
    uart_putchar(DEBUG_UART,0x05);
    uart_putchar(DEBUG_UART,0x01);//��������ͷ
    uart_putchar(DEBUG_UART, (uint8)round((steering_Target - STEERING_MIN)/(STEERING_MAX-STEERING_MIN)*255));
    uart_putchar(DEBUG_UART,0x00);
    uart_putchar(DEBUG_UART,0xff);
    uart_putchar(DEBUG_UART,0x05);
    uart_putchar(DEBUG_UART,0x02);//��������β
}

void Set_Steering_Target(uint8 val)
{
    steering_Target = ((float)val) / 256 * (STEERING_MAX-STEERING_MIN)+STEERING_MIN;
}

void Set_Steering(void)
{
    uint32 duty;
    //duty = 11.759*steering_Target + 4.4933
    //R-Square = 0.9975
    duty = 11.759*(steering_Target>0?steering_Target:(-steering_Target)) + 4.4933;
    if (duty>STEERING_DUTY_MAX)//�������
    {
        duty = STEERING_DUTY_MAX;
    }
    if (steering_Target == 0)
    {
        duty = 0;
    }
    pwm_duty(ATOM2_CH0_P33_10, (uint32)(1500+(steering_Target<0?1:-1)*duty));
}

void Cal_Steering_Error(float Cal_Steering_Range_of_Img)
{
    //����Col_Center��ɨ�跶Χsearch_Lines������ȫ�ֱ�����

    float steering_Error_tmp = 0;
    float Col_Center_Init = 0;

    for (int i=0;i<search_Lines;i++)
    {
        if(Col_Center[i] != -2)
        {
            Col_Center_Init = Col_Center[i];
            break;
        }
    }

    for (int i=0;i<(search_Lines*Cal_Steering_Range_of_Img);i++)
    {
        if(Col_Center[i] != -2)
        {
            steering_Error_tmp = steering_Error_tmp + Col_Center[i] - Col_Center_Init;
        }
    }

        steering_Error = steering_Error_tmp /(10.0f * ratioOfPixelToHG);

}

//https://blog.csdn.net/qq_49487109/article/details/117963017 �ɲο�
void Cal_Steering_Target(void)
{
    //����ȫ�ֱ����������壩����λ��ʽPDԭ����ת��Ŀ��Steering_Target(��Χ-30~30��������ת��������ת)
    Steering_PID.last_error = Steering_PID.current_error;
    Steering_PID.current_error = steering_Error;

    steering_Target = (Steering_PID.KP * Steering_PID.current_error) +( Steering_PID.current_error - Steering_PID.last_error );

    if(steering_Target>30) steering_Target = 30;
    if(steering_Target<-30) steering_Target = - 30;
}
