#include "motor.h"
#include "sys.h"
void MiniBalance_Motor_Init(void)  //电机方向控制引脚
{
//	RCC->APB2ENR|=1<<2;       //PORTA时钟使能 
//	
//	GPIOA->CRL&=0XF000FFFF;   //PORTA4,5,6推挽输出 
//	GPIOA->CRL|=0X02220000;   //PORTA4,5,6推挽输出 
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能B端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度2MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  //初始化GPIO
	
	EN=1;
	Brake=0;
}
void MiniBalance_PWM_Init(u16 arr,u16 psc)  //电机PWM引脚初始化
{		 					 
  MiniBalance_Motor_Init(); //初始化电机控制所需IO
	RCC->APB1ENR|=1<<1;       //TIM3时钟使能    
	RCC->APB2ENR|=1<<3;       //PORTB时钟使能 
  RCC->APB2ENR|=1<<2;       //PORTA时钟使能 
	
	GPIOA->CRL&=0X0FFFFFFF;   //PORTA  7复用输出
	GPIOA->CRL|=0XB0000000;   //PORTA  7复用输出
	
	TIM3->ARR=arr;//设定计数器自动重装值 
	TIM3->PSC=psc;//预分频器不分频

  TIM3->CCMR1|=7<<12; //CH2 PWM1模式
  TIM3->CCMR1|=1<<11; //CH2预装载使能
  TIM3->CCER|=1<<4;   //CH2输出使能
	
	TIM3->BDTR |= 1<<15;
	TIM3->CR1=0x80;  //ARPE使能 
	TIM3->CR1|=0x01;   //使能定时器3 
	PWM=7199;
} 

