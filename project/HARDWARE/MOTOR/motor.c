#include "motor.h"
#include "sys.h"
void MiniBalance_Motor_Init(void)  //��������������
{
//	RCC->APB2ENR|=1<<2;       //PORTAʱ��ʹ�� 
//	
//	GPIOA->CRL&=0XF000FFFF;   //PORTA4,5,6������� 
//	GPIOA->CRL|=0X02220000;   //PORTA4,5,6������� 
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��B�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�2MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  //��ʼ��GPIO
	
	EN=1;
	Brake=0;
}
void MiniBalance_PWM_Init(u16 arr,u16 psc)  //���PWM���ų�ʼ��
{		 					 
  MiniBalance_Motor_Init(); //��ʼ�������������IO
	RCC->APB1ENR|=1<<1;       //TIM3ʱ��ʹ��    
	RCC->APB2ENR|=1<<3;       //PORTBʱ��ʹ�� 
  RCC->APB2ENR|=1<<2;       //PORTAʱ��ʹ�� 
	
	GPIOA->CRL&=0X0FFFFFFF;   //PORTA  7�������
	GPIOA->CRL|=0XB0000000;   //PORTA  7�������
	
	TIM3->ARR=arr;//�趨�������Զ���װֵ 
	TIM3->PSC=psc;//Ԥ��Ƶ������Ƶ

  TIM3->CCMR1|=7<<12; //CH2 PWM1ģʽ
  TIM3->CCMR1|=1<<11; //CH2Ԥװ��ʹ��
  TIM3->CCER|=1<<4;   //CH2���ʹ��
	
	TIM3->BDTR |= 1<<15;
	TIM3->CR1=0x80;  //ARPEʹ�� 
	TIM3->CR1|=0x01;   //ʹ�ܶ�ʱ��3 
	PWM=7199;
} 

