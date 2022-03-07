#include "show.h"
#include "math.h"
#include "control.h"	

void oled_show(void)
{	
	  OLED_ShowString(0,0,"V_Wheel:",12);  //��һ�У���������ת�ٶ���ʾ
	  if(Encoder_x>=0){OLED_ShowString(80,0,"+",12);OLED_ShowNum(87,0,Encoder_x,3,12);}
    else          {OLED_ShowString(80,0,"-",12);OLED_ShowNum(87,0,-Encoder_x,3,12);}
   
    OLED_ShowString(0,1,"Vol_Bat:",12);  //�ڶ��У���ص�����ʾ
		OLED_ShowNum(60,1,Voltage,4,12);OLED_ShowString(100,1,"mV",12);

		OLED_ShowString(0,2,"Gyr_Rol:",12);  //�����У�ROLL����ٶ���ʾ
    if(Gyro_Balance_x>=0){OLED_ShowString(61,2,"+",12);OLED_ShowNum(68,2,Gyro_Balance_x,4,12);}
		else                  {OLED_ShowString(61,2,"-",12);OLED_ShowNum(68,2,-Gyro_Balance_x,4,12);}

		OLED_ShowString(0,3,"Rol:",12);      //�����У�ROLL��Ƕ���ʾ
    if(Angle_Balance_x>=0)
      {
			  OLED_ShowString(30,3,"+",12);
			  OLED_ShowNum(37,3,Angle_Balance_x,3,12);
				OLED_ShowString(55,3,".",12);
				OLED_ShowNum(60,3,(int)(Angle_Balance_x*10)%10,1,12);
				OLED_ShowString(68,3,"deg",12);
			}
		else                  
      {
			  OLED_ShowString(30,3,"-",12);
			  OLED_ShowNum(37,3,-Angle_Balance_x,3,12);
				OLED_ShowString(55,3,".",12);
				OLED_ShowNum(60,3,(int)(-Angle_Balance_x*10)%10,1,12);
				OLED_ShowString(68,3,"deg",12);
			}
				
		OLED_ShowString(0,4,"State:",12);    //�����У���ǰ����״̬��ʾ
		OLED_ShowNum(55,4,t,3,12);
			
		OLED_ShowString(0,5,"Cen_G:",12);    //�����У���̬��е��ֵ��ʾ
    OLED_ShowNum(50,5,Center_Gravity,2,12);
		OLED_ShowString(65,5,".",12);
		OLED_ShowNum(74,5,(int)(Center_Gravity*10)%10,1,12);
		OLED_ShowString(82,5,"deg",12);
			
		OLED_ShowString(0,6,"Cen_SET:",12);  //�����У��趨������е��ֵ��ʾ
		OLED_ShowNum(77,6,Center_Gra_Sart,2,12);
		OLED_ShowString(89,6,".",12);
		OLED_ShowNum(94,6,(int)(Center_Gra_Sart*10)%10,1,12);
		OLED_ShowString(102,6,"deg",12);
			
			
		OLED_ShowString(0,7,"Ene_Sto:",12);   //�ڰ��У�����ϵ����ʾ
		OLED_ShowNum(70,7,Energy_Storage,3,12);
		OLED_ShowString(89,7,".",12);
		OLED_ShowNum(94,7,(int)(Energy_Storage*100)/10,1,12);
		OLED_ShowNum(102,7,(int)(Energy_Storage*100)%10,1,12);
		OLED_ShowString(110,7,"deg",12);
	}
void fill_picture(unsigned char fill_Data)    //OLED��亯��
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Byte(0xb0+m,0);	//�ӵ�0ҳ����7ҳ
		OLED_WR_Byte(0x00,0);		//low column start address
		OLED_WR_Byte(0x10,0);		//high column start address
		for(n=0;n<128;n++)OLED_WR_Byte(fill_Data,1);	
	}
}



