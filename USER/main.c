
#include "init.h"
#include <math.h>
/*
 * ��������main
 * ����  ��������
 * ����  ����
 * ���  ����
 * ����  ����
 */
int main(void)
{	
	Stm32_Clock_Init(6);//ϵͳʱ������Ϊ�ⲿ����9��Ƶ
	delay_init(72);//ϵͳSysTick��ʼ�� �о���̫׼������delayms()������Ϊ1864
	
  GPIO_Configuration();//������led��
	USART1_Config();	
	
	I2C_config(); 
  InitSensor();/*��������ʼ��*/

	SPI1_Init();
	PWM_Init(7199,0);//10kHZ
	TIM3_Init(29,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms  30Ϊ3ms

	/***********************************************************************/
	for(;;)
	{		
		u8 data1[20] = {0x00,0x00,0x00,0x38,0x44,0x44,0x38,0x00,0x00,0x7e,0x18,0x24,0x42,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
			
    int k=0,j=0;	
		
		//��������
		for(j=0;j<1;j++)
		{
			Led_RollShowStart(start);
		}
		
		
	//��ɨ��
		for(k=0;k<3;k++)
		{
			Led_StaticShow1(data1);
		}
		
		
		//������ʾ1
		for(j=0;j<3;j++)
		{
			Led_RollShow(emoje);
		}

		/*
    for(;j<200;j++)
		{
			Led_StaticShow(data);
		}
*/
		
	/*
		//��̬��ʾ����1
		for(j=0;j<200;j++)
		{
			Led_StaticShowHello(hello);
		}
		*/
			//Angle�ںϽǶ�
			//Gyro_y;//�������ٶ�
			//Angle_gy;//���ֽǶ�
			//0������û�����
			//showwave(Angle,Gyro_y,Angle_gy,0);


	}//while����		

}

