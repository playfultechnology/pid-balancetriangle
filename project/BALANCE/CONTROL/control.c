#include "control.h"	
#include "filter.h"
#include "show.h"
#include "usart.h"
#include "stmflash.h"
#include "led.h"

float Energy_Storage=0.45;          //ɲ��ǰ���ܵĹ������ٶ�ϵ������ֵԽ��ɲ��ǰ������ת��Խ�ߣ���������Խ��
float Center_Gravity;               //��е��ֵ
float Center_Gra_Sart=88.9;         //�����趨��е��ֵ
int Encoder_x=0;              //����������
int Moto_x;                   //���PWM����	
int Voltage;                  //��ص�ѹ����
float Angle_Balance_x;        //����Ƕ�
float Gyro_Balance_x=0;       //����Ǽ��ٶ�


static float Bias,Pwm,Last_bias;      //ƫ����Ʊ�������һ��ƫ��

static float Pwm_L,Integral_bias_L,Last_Bias_L,Encoder_L;

int Balance_Pwm_x=0,velocity_Pwm_x=0; //������PWM����
char t=0;                             //״̬����
int s=0;                              //�ϵ��ȶ�����                    
int TIM1_UP_IRQHandler(void)          //���еĿ��ƴ��붼�������� TIM1���Ƶ�10ms��ʱ�ж�  
{    
	if(TIM1->SR&0X0001)
	{   
		  u8 key_value;
		  TIM1->SR&=~(1<<0);                     //===�����ʱ��1�жϱ�־λ
      Get_Angle();                           //��ȡ�Ƕ�
		  Voltage=Get_battery_volt()*1000;       //��ȡ��ѹ
      Encoder_x=-Read_Encoder(2);            //���±�����λ����Ϣ
//      if(s<100)s++;                        //�ϵ粻�ȶ�״̬
//      else                                 //�ϵ��ȶ�״̬
			{				
				if(Angle_Balance_x>120)Pwm_L=Integral_bias_L=Last_Bias_L=Encoder_L=0,left_up();      //����������
				else if(Angle_Balance_x<60)Pwm_L=Integral_bias_L=Last_Bias_L=Encoder_L=0,Right_up(); //����������
				else nomal();                          //ά��ֱ��״̬
				Set_Pwm(Moto_x);                       //===��ֵ��PWM�Ĵ���*/	
			}
			key_value=key_read();
			if(key_value==3)                         //��������ͬʱ�����£������������״̬
			{
				Set_Pwm(0);                            //�رյ��
				while(key_value==3)key_value=key_read();
				fill_picture(0x00);	            //OLED����
				OLED_ShowString(5,10,"SET_UP",12);
			  OLED_ShowString(10,4,"Entering...",12);					
				delay_ms(1000);
				fill_picture(0x00);	            //OLED����
				while(1)
				{
					Get_Angle();                           //��ȡ�Ƕ�
		      Voltage=Get_battery_volt();            //��ȡ��ѹ
          Encoder_x=-Read_Encoder(2);             //���±�����λ����Ϣ
					key_value=key_read();
					if(Angle_Balance_x<70||Angle_Balance_x>110)
					{
						if(key_value==1)
						{
							if(Angle_Balance_x<70)Center_Gra_Sart=Center_Gra_Sart-0.1;
							else if(Angle_Balance_x>110)Center_Gra_Sart=Center_Gra_Sart+0.1;
							oled_show(); 
							delay_ms(200);				
						}
						else if(key_value==2)
						{
							if(Angle_Balance_x<70)Energy_Storage=Energy_Storage-0.01;
							else if(Angle_Balance_x>110)Energy_Storage=Energy_Storage+0.01;
							if(Energy_Storage>0.6)Energy_Storage=0.6;
							oled_show(); 
							delay_ms(200);
						}
				  }
					else if(key_value==3)
					{
						fill_picture(0x00);	            //OLED����
						OLED_ShowString(5,3,"Parameters",12);
						OLED_ShowString(10,4,"Saving...",12);
            datatemp[0]=((int)(Center_Gra_Sart*10))/100;
					  datatemp[1]=(((int)(Center_Gra_Sart*10))%100)/10;
					  datatemp[2]=((int)(Center_Gra_Sart*10))%10;
						datatemp[3]=((int)(Energy_Storage*100))/100;
						datatemp[4]=(((int)(Energy_Storage*100))%100)/10;
						datatemp[5]=((int)(Energy_Storage*100))%10;
					  STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)datatemp,7);						
						delay_ms(1000);delay_ms(1000);
						fill_picture(0x00);	            //OLED����
						break;
					}
					oled_show(); 
					delay_ms(5);
				}
			}
	
	
	}       	
	 return 0;	  
} 
void left_up()
{
  if(t==0)t=1;	
	if(t==1)                                                     //��һ�׶Σ���ȡ�ȶ��Ƕ�
	{
		static float angle_last;
		float angle_bias;      //�Ƕȱ仯��
		angle_bias*=0.8;
		angle_bias+=(angle_last-Angle_Balance_x)*0.2;//һ���˲�
		if(angle_bias<0.01||angle_bias>-0.01)t=2;    //���Ƕȱ仯����С�ˣ�˵���Ƕ����ȶ�
		angle_last=Angle_Balance_x;
	}
	else if(t==2)                                                //�ڶ��׶Σ����ݵ��µĽǶ�ȷ����������ת���ٶ�
	{
		float speed_target,speed_bias;
		speed_target=Angle_Balance_x-90.0;
		Moto_x=Incremental_PI (-Encoder_x,speed_target*Energy_Storage);
		speed_bias=speed_target*Energy_Storage-Encoder_x; 
		if(speed_bias<3&&speed_bias>-3)t=3,Bias=Pwm=Last_bias=0;    //���ٶȱ仯����С�ˣ�˵���ٶ����ȶ�
	}
	else if(t==3)                                                //�����׶Σ�ɲ������
	{		
		int i=0; 	
    while(Angle_Balance_x>105)      //����Ƕ�ƽ��
		{
		 Encoder_x=-Read_Encoder(2);             //���±�����λ����Ϣ
		 Moto_x=Encoder_x*240*8;             //����ʩ������
     Set_Pwm(Moto_x); 		
		 Get_Angle();
     delay_ms(5);			
		 i++;
		 if(i>400)break;   //��ʱδ����������ʧ��
	  }
		t=0;
  }
}
void Right_up()
{
	if(t==0)t=11;	
	if(t==11)                                                     //��һ�׶Σ���ȡ�ȶ��Ƕ�
	{
		static float angle_last;
		float angle_bias;      //�Ƕȱ仯��
		angle_bias*=0.8;
		angle_bias+=(angle_last-Angle_Balance_x)*0.2;//һ���˲�
		if(angle_bias<0.01||angle_bias>-0.01)t=12;    //���Ƕȱ仯����С�ˣ�˵���Ƕ����ȶ�
		angle_last=Angle_Balance_x;
	}
	else if(t==12)                                                //�ڶ��׶Σ����ݵ��µĽǶ�ȷ����������ת���ٶ�
	{
		float speed_target,speed_bias;
		speed_target=Angle_Balance_x-90.0;
		Moto_x=Incremental_PI (-Encoder_x,speed_target*Energy_Storage);
		speed_bias=speed_target*Energy_Storage-Encoder_x; 
		if(speed_bias<3&&speed_bias>-3)t=13,Bias=Pwm=Last_bias=0;    //���ٶȱ仯����С�ˣ�˵���ٶ����ȶ�
	}
	else if(t==13)                                                //�����׶Σ�ɲ������
	{		
		int i=0; 	
    while(Angle_Balance_x<75)      //����Ƕ�ƽ��
		{
		 Encoder_x=-Read_Encoder(2);             //���±�����λ����Ϣ
		 Moto_x=Encoder_x*240*8;             //����ʩ������
     Set_Pwm(Moto_x); 			
		 Get_Angle();
     delay_ms(5);			
		 i++;
		 if(i>400)break;   //��ʱδ����������ʧ��
	  }
		t=0;
  }
}
void nomal()
{
      if(Encoder_x>10)Center_Gravity=Center_Gravity-0.001;
      else if(Encoder_x<-10)Center_Gravity=Center_Gravity+0.001;	
      if(Encoder_x>20)Center_Gravity=Center_Gravity-0.002;
      else if(Encoder_x<-20)Center_Gravity=Center_Gravity+0.002;
      if(Encoder_x>30)Center_Gravity=Center_Gravity-0.005;
      else if(Encoder_x<-30)Center_Gravity=Center_Gravity+0.005;
		
     	Balance_Pwm_x=balance_x(Angle_Balance_x,Gyro_Balance_x);   //===�Ƕ�PD����	
			velocity_Pwm_x=velocity_x(Encoder_x);       //===�ٶ�PD����
      Moto_x=Balance_Pwm_x+velocity_Pwm_x;        //===����������PWM			                    
}
int balance_x(float Angle,float gyro)//���PD���� ��ڲ������Ƕ� ����  ֵ����ǿ���PWM
{  
	 float Balance_KP=450,Balance_KI=0,Balance_KD=4;
   float Bias; //���ƫ��
	 static float D_Bias,Integral_bias; //PID��ر���
	 int balance;   //PWM����ֵ 
	 Bias=Angle-Center_Gravity;   //���ƽ��ĽǶ���ֵ �ͻ�е���
	 Integral_bias+=Bias;	
	 if(Integral_bias>30000)Integral_bias=30000;
	 if(Integral_bias<-30000)Integral_bias=-30000;
   D_Bias=gyro;	            //���ƫ���΢�� ����΢�ֿ���
	 balance=Balance_KP*Bias+Balance_KI*Integral_bias+D_Bias*Balance_KD;   //===������ǿ��Ƶĵ��PWM  PD����
	 return balance;
}
int velocity_x(int encoder) //λ��ʽPID������ ��ڲ���������������λ����Ϣ��Ŀ��λ��  ����  ֵ�����PWM
{ 	
	 float Position_KP=-600,Position_KI=-0.5,Position_KD=0;
	 Encoder_L *= 0.65;		                                                //===һ�׵�ͨ�˲���       
	 Encoder_L += encoder*0.35;	                                    //===һ�׵�ͨ�˲���    
	 Integral_bias_L+=Encoder_L;	                                 //���ƫ��Ļ���
	 if(Integral_bias_L>10000)Integral_bias_L=10000;
	 if(Integral_bias_L<-10000)Integral_bias_L=-10000;
	 
	 Pwm_L=Position_KP*Encoder_L+Position_KI*Integral_bias_L+Position_KD*(Encoder_L-Last_Bias_L);       //λ��ʽPID������
	 Last_Bias_L=Encoder_L;                                       //������һ��ƫ�� 
	 return Pwm_L;                                           //�������
}
void Set_Pwm(int motox)
{
	  int Amplitude_x=7199;    //===PWM������7200 ������6900
	  if(motox<-Amplitude_x) motox=-Amplitude_x;	
		if(motox>Amplitude_x)  motox=Amplitude_x;	
	
    if(motox<0)			DIR=0;
		else 	          DIR=1;
		PWM=7199-myabs(motox);
}
int Incremental_PI (int Encoder,float Target)
{ 	
   float Kp=2,Ki=0.5;	
	 Bias=Encoder-Target;                //����ƫ��
	 Pwm+=Kp*(Bias-Last_bias)+Ki*Bias;   //����ʽPI������
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
void Get_Angle(void)
{ 
  u8 bufa[6],bufg[6];	
	float Accel_Y,Accel_X,Accel_Z,Gyro_Y,Gyro_X,Gyro_Z;
	MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,bufa);
	MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,bufg);
	
 	sensor.acc.origin.x = ((((int16_t)bufa[0]) << 8) | bufa[1]);
 	sensor.acc.origin.y = ((((int16_t)bufa[2]) << 8) | bufa[3]);
 	sensor.acc.origin.z = ((((int16_t)bufa[4]) << 8) | bufa[5]);

 	sensor.gyro.origin.x = ((((int16_t)bufg[0]) << 8)| bufg[1]);
 	sensor.gyro.origin.y = ((((int16_t)bufg[2]) << 8)| bufg[3]);
 	sensor.gyro.origin.z = ((((int16_t)bufg[4]) << 8)| bufg[5]);	
		  
	Gyro_Y=sensor.gyro.origin.y;//��ȡX��������
	Gyro_Z=sensor.gyro.origin.z;    //��ȡZ��������
	Accel_X=sensor.acc.origin.x; //��ȡY����ٶȼ�
	Accel_Z=sensor.acc.origin.z; //��ȡZ����ٶȼ�
	if(Gyro_Y>32768)  Gyro_Y-=65536;                       //��������ת��  Ҳ��ͨ��shortǿ������ת��
	if(Gyro_Z>32768)  Gyro_Z-=65536;                       //��������ת��
	if(Accel_X>32768) Accel_X-=65536;                      //��������ת��
	if(Accel_Z>32768) Accel_Z-=65536;                      //��������ת��	
	//Gyro_Balance_y=-Gyro_Y;           //����ƽ����ٶ�		
	Accel_Y=atan2(Accel_X,Accel_Z)*180/PI; //�������	
  Gyro_Y=Gyro_Y/16.4;               //����������ת��
	//Yijielvbo_Y(Accel_Y,-Gyro_Y);

			
	Gyro_X=sensor.gyro.origin.x;//��ȡX��������
	Accel_Y=sensor.acc.origin.y; //��ȡY����ٶȼ�
	if(Gyro_X>32768)  Gyro_X-=65536;                       //��������ת��  Ҳ��ͨ��shortǿ������ת��
	if(Accel_Y>32768) Accel_Y-=65536;                      //��������ת��
	Gyro_Balance_x=-Gyro_X;           //����ƽ����ٶ�
	Accel_X= (atan2(Accel_Z , Accel_Y)) * 180 / PI; //�������	
  Gyro_X=Gyro_X/16.4;               //����������ת��			
	Yijielvbo_X(Accel_X,-Gyro_X);
}

