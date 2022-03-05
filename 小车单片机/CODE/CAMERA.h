#ifndef __CAMERA_h__
#define __CAMERA_h__

#define Y_WIDTH_CAMERA 40
#define X_WIDTH_CAMERA 187

#define width_Inverse_Perspective_Max 256
#define height_Inverse_Perspective_Max 256

#define CLASS_NUM 12

#define KMEANS_K 3
#define GOD_LIGHT 205

#define SEARCH_LINES_STRAIGHT ((int)(20/(10.0f*ratioOfPixelToHG))) //��ʾֱ��ɨ�跶ΧΪ��ǰ2.0������ͷ�߶�
#define STRAIGHT_CONDITION 5

extern uint8 mt9v03x_image[MT9V03X_H][MT9V03X_W];
extern uint8 mt9v03x_image_cutted[Y_WIDTH_CAMERA][X_WIDTH_CAMERA];
extern uint8 thresholding_Value;
extern uint8 mt9v03x_image_cutted_thresholding_inversePerspective[height_Inverse_Perspective_Max][width_Inverse_Perspective_Max];
extern float cameraAlphaUpOrDown;//����У��
extern float cameraThetaDown;//��ҪУ��
extern float ratioOfMaxDisToHG;//��Ӱ����ʾ����
extern float ratioOfPixelToHG;//��Ӱ��ֱ���
extern uint8 classification_Result;

extern float Col_Center[height_Inverse_Perspective_Max];//���������ϵ�˳��洢�������ߵ��кŽ�������Ϸ���ȫ��Ϊ-2
extern int search_Lines;

void My_Init_Camera(void);

void UART_Image(void);
void UART_Thresholding_Value(void);
void UART_Inverse_Perspective(void);
void UART_Classification(void);
void Set_Thresholding_Value(uint8 val);
void Set_CameraAlphaUpOrDown(uint8 val);
void Set_CameraThetaDown(uint8 val);
void Set_RatioOfMaxDisToHG(uint8 val);
void Set_RatioOfPixelToHG(uint8 val);
uint8 Classification(void);
int Check_Straight(void);
void DrawCenterLine(void);
void DrawCenterLinewithConfig(float filter);






void Get_Cutted_Image(void);
void Get_Thresholding_Image(void);
void Get_Inverse_Perspective_Image(void);





#endif
