#include "show.h"
#include "math.h"
#include "control.h"	

void oled_show(void)
{	
	  OLED_ShowString(0,0,"V_Wheel:",12);  //第一行：惯性轮旋转速度显示
	  if(Encoder_x>=0){OLED_ShowString(80,0,"+",12);OLED_ShowNum(87,0,Encoder_x,3,12);}
    else          {OLED_ShowString(80,0,"-",12);OLED_ShowNum(87,0,-Encoder_x,3,12);}
   
    OLED_ShowString(0,1,"Vol_Bat:",12);  //第二行：电池电量显示
		OLED_ShowNum(60,1,Voltage,4,12);OLED_ShowString(100,1,"mV",12);

		OLED_ShowString(0,2,"Gyr_Rol:",12);  //第三行：ROLL轴角速度显示
    if(Gyro_Balance_x>=0){OLED_ShowString(61,2,"+",12);OLED_ShowNum(68,2,Gyro_Balance_x,4,12);}
		else                  {OLED_ShowString(61,2,"-",12);OLED_ShowNum(68,2,-Gyro_Balance_x,4,12);}

		OLED_ShowString(0,3,"Rol:",12);      //第四行：ROLL轴角度显示
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
				
		OLED_ShowString(0,4,"State:",12);    //第五行：当前运行状态显示
		OLED_ShowNum(55,4,t,3,12);
			
		OLED_ShowString(0,5,"Cen_G:",12);    //第六行：动态机械中值显示
    OLED_ShowNum(50,5,Center_Gravity,2,12);
		OLED_ShowString(65,5,".",12);
		OLED_ShowNum(74,5,(int)(Center_Gravity*10)%10,1,12);
		OLED_ShowString(82,5,"deg",12);
			
		OLED_ShowString(0,6,"Cen_SET:",12);  //第七行：设定开机机械中值显示
		OLED_ShowNum(77,6,Center_Gra_Sart,2,12);
		OLED_ShowString(89,6,".",12);
		OLED_ShowNum(94,6,(int)(Center_Gra_Sart*10)%10,1,12);
		OLED_ShowString(102,6,"deg",12);
			
			
		OLED_ShowString(0,7,"Ene_Sto:",12);   //第八行：蓄能系数显示
		OLED_ShowNum(70,7,Energy_Storage,3,12);
		OLED_ShowString(89,7,".",12);
		OLED_ShowNum(94,7,(int)(Energy_Storage*100)/10,1,12);
		OLED_ShowNum(102,7,(int)(Energy_Storage*100)%10,1,12);
		OLED_ShowString(110,7,"deg",12);
	}
void fill_picture(unsigned char fill_Data)    //OLED填充函数
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Byte(0xb0+m,0);	//从第0页到第7页
		OLED_WR_Byte(0x00,0);		//low column start address
		OLED_WR_Byte(0x10,0);		//high column start address
		for(n=0;n<128;n++)OLED_WR_Byte(fill_Data,1);	
	}
}



