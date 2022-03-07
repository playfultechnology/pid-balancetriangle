#ifndef __MOTOR_H
#define __MOTOR_H
#include <sys.h>	 

#define DIR   PAout(6)     //惯量轮电机方向控制引脚
#define EN   PAout(4)      //惯量轮电机使能引脚
#define Brake   PAout(5)      //惯量轮电机使能引脚

#define PWM   TIM3->CCR2   //PA7

void MiniBalance_PWM_Init(u16 arr,u16 psc);
void MiniBalance_Motor_Init(void);
#endif
