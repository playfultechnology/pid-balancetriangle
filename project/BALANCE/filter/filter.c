#include "filter.h"
#include "sys.h" 
float K1 =0.02; 
float angle, angle_dot; 	

//һ�׻����˲�  ��ڲ��������ٶȡ����ٶ�
void Yijielvbo_X(float angle_m, float gyro_m)
{
   //Angle_Balance_x = K1 * angle_m+ (1-K1) * (Angle_Balance_x + gyro_m * 0.005);
	Angle_Balance_x = 0.1 * angle_m+ (1-0.1) * (Angle_Balance_x + gyro_m * 0.003);
}


