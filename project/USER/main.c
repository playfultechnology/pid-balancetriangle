//����:Lee Young
//2021��2��19�գ���һ�θ��£��������Զ������Ĺ��ܡ�
//2021��4�h15�գ��ڶ��θ��£�������ͨ������������е��ֵ�͹����������ٶȵĹ��ܡ�


#include "sys.h"
#include "show.h"
#include "control.h"
#include "stmflash.h"
#include "key.h"

u16 datatemp[7];

int main(void)
{ 
	Stm32_Clock_Init(9);            //ϵͳʱ������
	delay_init(72);                 //��ʱ��ʼ��
	uart_init(72,115200);             //��ʼ������1
	uart2_init(72,19200);
	JTAG_Set(JTAG_SWD_DISABLE);     //�ر�JTAG�ӿ�
	JTAG_Set(SWD_ENABLE);           //��SWD�ӿ� �������������SWD�ӿڵ���
	
	KEY_Init();
	MiniBalance_PWM_Init(7199,0);   //��ʼ��PWM 10KHZ������������� 
	OLED_Init();                    //OLED��ʼ��       
  fill_picture(0x00);	            //OLED����
  Baterry_Adc_Init();             //�������Adc��ʼ��
	OLED_ShowString(25,3,"MPU6050",12);
	OLED_ShowString(27,4,"init...",12);
	delay_ms(500);
	
	while(MPU_Init());              //MPU6050��ʼ��
	
	LED_Init();                     //LED��ʼ��
	fill_picture(0x00);
  OLED_ShowString(25,3,"MPU6050",12);
	OLED_ShowString(5,4,"init success!!!",12);
	delay_ms(500);
	fill_picture(0x00);
	
	Encoder_Init_TIM2();            //��ʼ����������TIM4�ı������ӿ�ģʽ��
	STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)datatemp,7);   //��ȡflash�е���ֵ����
	if(datatemp[6]==86)             //����Ϊ��һ�ο���
	{
   Center_Gra_Sart=datatemp[0]*10+datatemp[1]+((float)datatemp[2])/10;
	 Energy_Storage=(s16)datatemp[3]+((float)((s16)datatemp[4]))/10+((float)((s16)datatemp[5]))/100;
	}
	else                            //��һ�ο��� 
	{ 
		datatemp[6]=86;  
    datatemp[0]=((int)(Center_Gra_Sart*10))/100;
		datatemp[1]=(((int)(Center_Gra_Sart*10))%100)/10;
		datatemp[2]=((int)(Center_Gra_Sart*10))%10;
		
		datatemp[3]=((int)(Energy_Storage*100))/100;
		datatemp[4]=(((int)(Energy_Storage*100))%100)/10;
		datatemp[5]=((int)(Energy_Storage*100))%10;
		STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)datatemp,7);
	}
  Center_Gravity=Center_Gra_Sart;
	Angle_Balance_x=Center_Gra_Sart;   //����Ƕ�	
	Timer1_Init(99,7199);           //��ʱ�жϳ�ʼ�� 
	while(1)oled_show();                           //ˢ����ʾ
}
