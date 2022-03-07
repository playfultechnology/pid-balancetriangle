#include "led.h"
void LED_Init(void)
{
RCC->APB2ENR|=1<<3;      //使能 PORTB 时钟  
GPIOB->CRL&=0XFFFFFF0F;
GPIOB->CRL|=0X00000020;  //PB1 推挽输出
LED=1;	
//GPIOB->ODR|=1<<4;        //PB1 输出高
}
void Led_Flash(u16 time)
{
	  static int temp;
	  if(0==time) LED=1;
	  else		if(++temp==time)	{LED=~LED;temp=0;}
}
