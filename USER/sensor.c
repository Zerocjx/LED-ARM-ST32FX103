#include  "init.h"

//******�ǶȲ���************
float Gyro_y;        //Y�������������ݴ�
float Angle_gy;      //�ɽ��ٶȼ������б�Ƕ�
float Accel_x;	     //X����ٶ�ֵ�ݴ�
float Angle_ax;      //�ɼ��ٶȼ������б�Ƕ�
float Angle;         //���ղ����Ƕ�


//----------------------------�Ƕȿ���PD------------------------------//
float     Angle_Control_P = 40;                        
float     Angle_Control_D = 0.3;  

float     Angle_Control_Out_Max = 5000.0;
 
float     g_Angle_Control_Out=0;//�Ƕȿ������

//----------------------------�������---------------------------------//
float     g_Left_Motor_Dead_Value = 680;//
float     g_Right_Motor_Dead_Value = 850;


/*************�������˲�*********************************/

void Kalman_Filter(float Accel,float Gyro)		
{
		
	static const float Q_angle=0.001;  
	static const float Q_gyro=0.003;
	static const float R_angle=0.5;
	static const float dt=0.009;	                  //dtΪkalman�˲�������ʱ��;
	static const char  C_0 = 1;
	static float Q_bias, Angle_err;
	static float PCt_0, PCt_1, E;
	static float K_0, K_1, t_0, t_1;
	static float Pdot[4] ={0,0,0,0};
	static float PP[2][2] = { { 1, 0 },{ 0, 1 } };
	
	Angle+=(Gyro - Q_bias) * dt; //�������

	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-����������Э�����΢��

	Pdot[1]= -PP[1][1];
	Pdot[2]= -PP[1][1];
	Pdot[3]=Q_gyro;
	
	PP[0][0] += Pdot[0] * dt;   // Pk-����������Э����΢�ֵĻ���
	PP[0][1] += Pdot[1] * dt;   // =����������Э����
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;
		
	Angle_err = Accel - Angle;	//zk-�������
	
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //����������Э����
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
		
	Angle	+= K_0 * Angle_err;	 //�������
	Q_bias	+= K_1 * Angle_err;	 //�������
	Gyro_y   = Gyro - Q_bias;	 //���ֵ(�������)��΢��=���ٶ�
}
void Angle_Calculate(void)
{
/****************************���ٶ�****************************************/

	Angle_ax = (Accel_x - 400 ) /16384;   //ȥ�����ƫ��,����õ��Ƕȣ����ȣ�
	Angle_ax = Angle_ax*1.2*180/3.14;     //����ת��Ϊ��,

/****************************���ٶ�****************************************/

	Gyro_y = -(Gyro_y - 8)/16.4;         //ȥ�����ƫ�ƣ�������ٶ�ֵ 
	Angle_gy = Angle_gy + Gyro_y*0.009;  //���ٶȻ��ֵõ���б�Ƕ�.	

/***************************�������˲�+�Ƕ��ں�*************************************/
	Kalman_Filter(Angle_ax,Gyro_y);       //+�������˲��������

/*******************************�����˲�******************************************/
/*����ԭ����ȡ��ǰ��Ǻͼ��ٶȻ�
	����ǲ�ֵ���зŴ�Ȼ��������
	�ǽ��ٶȵ��Ӻ��ٻ��֣��Ӷ�ʹ��
	�������Ϊ���ٶȻ�õĽǶ�0.5
	Ϊ�Ŵ������ɵ��ڲ�����;
	0.01Ϊϵͳ����10ms	
*/	
	Angle = Angle + (((Angle_ax-Angle)*0.3 + Gyro_y)*0.009);
															  		
}


void Angle_Control(void)
{
    float value;

    value = Angle * Angle_Control_P
					 + Gyro_y* Angle_Control_D;

    if(value > Angle_Control_Out_Max)
    {
        value = Angle_Control_Out_Max;
    }
    else if(value < -Angle_Control_Out_Max)
    {
        value = -Angle_Control_Out_Max;
    }
       g_Angle_Control_Out = value;
		
}

void Set_Motor_Pwm(void)
{
    float Left_Motor_Voltage;
    float Right_Motor_Voltage;
		

    Left_Motor_Voltage =g_Angle_Control_Out;// g_Angle_Control_Out- g_Speed_Control_Out + g_Direction_Control_Out;
    Right_Motor_Voltage =g_Angle_Control_Out;//g_Angle_Control_Out- g_Speed_Control_Out - g_Direction_Control_Out;
  
		if(Angle>30||Angle<-30)//����ֹͣ
		{
				pwm_set_off(TIM1, TIM_Channel_1);
				pwm_set_off(TIM8, TIM_Channel_3);
				pwm_set_off(TIM8, TIM_Channel_1);
				pwm_set_off(TIM8, TIM_Channel_2);
			  TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);//ʹ�ܻ���ʧ��ָ����TIM�ж�
			return;
		}
		//��������,
    if(Left_Motor_Voltage > 0)
    {
        Left_Motor_Voltage += g_Left_Motor_Dead_Value;
    }
    else if(Left_Motor_Voltage < 0)
    {
        Left_Motor_Voltage -= g_Left_Motor_Dead_Value;
    }
    if(Right_Motor_Voltage > 0)
    {
        Right_Motor_Voltage += g_Right_Motor_Dead_Value;
    }
    else if(Right_Motor_Voltage < 0)
    {
        Right_Motor_Voltage -= g_Right_Motor_Dead_Value;
    }

    //================���ʹ���===================//
    if(Left_Motor_Voltage > Angle_Control_Out_Max)
    {
        Left_Motor_Voltage = Angle_Control_Out_Max;
    }
    else if(Left_Motor_Voltage < -Angle_Control_Out_Max)
    {
        Left_Motor_Voltage = -Angle_Control_Out_Max;
    }
    if(Right_Motor_Voltage > Angle_Control_Out_Max)
    {
        Right_Motor_Voltage = Angle_Control_Out_Max;
    }
    else if(Right_Motor_Voltage < -Angle_Control_Out_Max)
    {
        Right_Motor_Voltage = -Angle_Control_Out_Max;
    }
    //==================������=================//
		
    if(Left_Motor_Voltage > 0)
    {    	
				pwm_set_high(TIM8, TIM_Channel_1);
				pwm_set_on(TIM8, TIM_Channel_2);
				TIM_SetCompare2(TIM8, 7200-(u16)Left_Motor_Voltage);
    }
    else
    {    
        Left_Motor_Voltage = -Left_Motor_Voltage; 
				pwm_set_high(TIM8, TIM_Channel_2);
        pwm_set_on(TIM8, TIM_Channel_1);				
				TIM_SetCompare1(TIM8, 7200-(u16)Left_Motor_Voltage);
    }
    if(Right_Motor_Voltage > 0)
    {   
		
				pwm_set_high(TIM8, TIM_Channel_3);
				pwm_set_on(TIM1, TIM_Channel_1);
				TIM_SetCompare1(TIM1, 7200-(u16)Right_Motor_Voltage);
    }
    else
    {     
				Right_Motor_Voltage = -Right_Motor_Voltage;  	      
				pwm_set_high(TIM1, TIM_Channel_1);
				pwm_set_on(TIM8, TIM_Channel_3);
				TIM_SetCompare3(TIM8, 7200-(u16)Right_Motor_Voltage);
    }
}
