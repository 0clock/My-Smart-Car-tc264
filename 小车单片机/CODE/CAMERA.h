#ifndef __CAMERA_h__
#define __CAMERA_h__

#define width_Inverse_Perspective_Max 256
#define height_Inverse_Perspective_Max 256

extern uint8 mt9v03x_image_cutted[Y_WIDTH][X_WIDTH];
extern uint8 thresholding_Value;
extern uint8 mt9v03x_image_cutted_compressed[Y_WIDTH][X_WIDTH];
extern uint32 compressed_Size;
extern uint8 mt9v03x_image_cutted_thresholding_inversePerspective[height_Inverse_Perspective_Max][width_Inverse_Perspective_Max];
extern float cameraAlphaUpOrDown;//����У��
extern float cameraThetaDown;//��ҪУ��
extern float ratioOfMaxDisToHG;//��Ӱ����ʾ����
extern float ratioOfPixelToHG;//��Ӱ��ֱ���

void My_Init_Camera(void);

void UART_Image(void);
void UART_Thresholding_Value(void);
void UART_Inverse_Perspective(void);
void Set_Thresholding_Value(uint8 val);
void Set_CameraAlphaUpOrDown(uint8 val);
void Set_CameraThetaDown(uint8 val);
void Set_RatioOfMaxDisToHG(uint8 val);
void Set_RatioOfPixelToHG(uint8 val);




void Get_Cutted_Image(void);
void Get_Thresholding_Image(void);
void Get_Inverse_Perspective_Image(void);





#endif
