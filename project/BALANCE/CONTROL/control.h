#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
#define PI 3.14159265
int TIM1_UP_IRQHandler(void);
int balance_x(float angle,float gyro);
int velocity_x(int Encoder);
int balance_y(float Angle,float Gyro);//直立PD控制 入口参数：角度、角速度
int velocity_y(int encoder);
void Set_Pwm(int motox);
void Xianfu_Pwm(void);
int myabs(int a);
void Get_Angle(void);
void left_up(void);
void Right_up(void);
void nomal(void);
int Incremental_PI (int Encoder,float Target);

extern float Energy_Storage;  
extern float Center_Gravity;          //动态机械中值
extern float Center_Gra_Sart;         //开机设定机械中值


#endif
