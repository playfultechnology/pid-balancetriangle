//作者:Lee Young
//2021年2月19日，第一次更新：增加了自动起立的功能。
//2021年4玥15日，第二次更新：增加了通过按键调整机械中值和惯量轮蓄能速度的功能。

// INCLUDES
#include "sys.h"
#include "show.h"
#include "control.h"
#include "stmflash.h"
#include "key.h"

// GLOBALS
u16 datatemp[7];

int main(void) {
	// Initialise hardware
	Stm32_Clock_Init(9);
	delay_init(72);
	uart_init(72, 115200);
	uart2_init(72, 19200);
	JTAG_Set(JTAG_SWD_DISABLE);
	JTAG_Set(SWD_ENABLE);
	KEY_Init();
	// Initialise 10kHz PWM for driving the motor 
	MiniBalance_PWM_Init(7199,0);   
	OLED_Init();      
  fill_picture(0x00);
  Baterry_Adc_Init();
	OLED_ShowString(25,3,"MPU6050",12);
	OLED_ShowString(27,4,"init...",12);
	delay_ms(500);
	
	while(MPU_Init());
	
	LED_Init();
	fill_picture(0x00);
  OLED_ShowString(25,3,"MPU6050",12);
	OLED_ShowString(5,4,"init success!!!",12);
	delay_ms(500);
	fill_picture(0x00);
	
	Encoder_Init_TIM2();
	// Read median data from flash memory
	STMFLASH_Read(FLASH_SAVE_ADDR, (u16*)datatemp, 7);
	
	// If there is valid data in EEPROM, load it
	if(datatemp[6] == 86) {
   Center_Gra_Sart = datatemp[0]*10 + datatemp[1] + ((float)datatemp[2])/10;
	 Energy_Storage = (s16)datatemp[3] + ((float)((s16)datatemp[4]))/10 + ((float)((s16)datatemp[5]))/100;
	}
	// If no valid data from previous run found
	else { 
		datatemp[6]=86;  
    datatemp[0]=((int)(Center_Gra_Sart*10))/100;
		datatemp[1]=(((int)(Center_Gra_Sart*10))%100)/10;
		datatemp[2]=((int)(Center_Gra_Sart*10))%10;
		
		datatemp[3]=((int)(Energy_Storage*100))/100;
		datatemp[4]=(((int)(Energy_Storage*100))%100)/10;
		datatemp[5]=((int)(Energy_Storage*100))%10;
		STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)datatemp,7);
	}
  Center_Gravity = Center_Gra_Sart;
	// Lateral angle
	Angle_Balance_x = Center_Gra_Sart;
	// Initialise timer interrupt
	Timer1_Init(99, 7199);
	while(1) { 
		oled_show();
	}
}
