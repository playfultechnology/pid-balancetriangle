#ifndef __FILTER_H
#define __FILTER_H
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
extern float angle, angle_dot; 	
void Kalman_Filter(float Accel,float Gyro);		
void Yijielvbo_X(float angle_m, float gyro_m);
void Yijielvbo_Y(float angle_m, float gyro_m);
#endif