#ifndef __MOTOR_H
#define __MOTOR_H
#include <sys.h>	 

#define DIR   PAout(6)     //�����ֵ�������������
#define EN   PAout(4)      //�����ֵ��ʹ������
#define Brake   PAout(5)      //�����ֵ��ʹ������

#define PWM   TIM3->CCR2   //PA7

void MiniBalance_PWM_Init(u16 arr,u16 psc);
void MiniBalance_Motor_Init(void);
#endif
