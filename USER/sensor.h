#ifndef _SENSOR_H
#define _SENSOR_H

extern float Gyro_y;        //Y�������������ݴ�
extern float Angle_gy;      //�ɽ��ٶȼ������б�Ƕ�
extern float Accel_x;	     //X����ٶ�ֵ�ݴ�
extern float Angle_ax;      //�ɼ��ٶȼ������б�Ƕ�
extern float Angle;         //���ղ����Ƕ�


void Angle_Calculate(void);
void Angle_Control(void);
void Set_Motor_Pwm(void);//�������


#endif
