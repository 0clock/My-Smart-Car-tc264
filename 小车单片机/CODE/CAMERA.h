#ifndef __CAMERA_h__
#define __CAMERA_h__

#define Y_WIDTH_CAMERA 40
#define X_WIDTH_CAMERA 188

#define width_Inverse_Perspective_Max 256
#define height_Inverse_Perspective_Max 256

#define CLASS_NUM 12

#define KMEANS_K 3

extern uint8 mt9v03x_image[MT9V03X_H][MT9V03X_W];
extern uint8 mt9v03x_image_cutted[Y_WIDTH_CAMERA][X_WIDTH_CAMERA];
extern uint8 thresholding_Value;
extern uint8 mt9v03x_image_cutted_thresholding_inversePerspective[height_Inverse_Perspective_Max][width_Inverse_Perspective_Max];
extern float cameraAlphaUpOrDown;//无需校正
extern float cameraThetaDown;//需要校正
extern float ratioOfMaxDisToHG;//仅影响显示距离
extern float ratioOfPixelToHG;//仅影响分辨率
extern uint8 classification_Result;

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
void Classification(void);





void Get_Cutted_Image(void);
void Get_Thresholding_Image(void);
void Get_Inverse_Perspective_Image(void);





#endif
