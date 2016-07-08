#include "pwm.h"
/*��Ӧ�ܽ�
	PC6			PC7			PC8			PA8	
	T8C1		T8C2		T8C3		T1C1
	
	PA7			PB0			PB1			PB13
	T8C1N		T8C2N		T8C3N		T1C1N
*/
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//��ʼ��TIMx��ʱ�����
	TIM_OCInitTypeDef  TIM_OCInitStructure;//��ʼ������TIMx
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;//��������
	
/*************1������io����*************************/
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);//ʹ��GPIO����ʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//ʹ�ܸ��ù���   
	
	//���ø�����Ϊ�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //�˿�Ƶ�ʿ���Ϊ2,10,50 
	GPIO_Init(GPIOA, &GPIO_InitStructure);//����A�˿�
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_13;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//����B�˿�
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	GPIO_Init(GPIOC, &GPIO_InitStructure);//����C�˿�
	
/*****************2��ͨ������*****************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1|RCC_APB2Periph_TIM8, ENABLE);//ʹ������ʱ��TIMx,TIM8
	
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
	TIM_ARRPreloadConfig(TIM1, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM8, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //TIM�����ȵ���ģʽ1��С���趨ֵ��Ϊ�ߣ�2��֮
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //ѡ�񻥲�����Ƚ�״̬
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High; //TIM�����������Ϊ��
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset; //ѡ�����״̬
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset; //���û������������Ƚ�״̬
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //TIMxͨ��1����
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);  
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);  
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);  
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //Ԥװ��ʹ�ܣ�Ϊ���ÿ��Ƹ���׼�����Ĳ�������ı䣬�ڱ��β���ִ����֮��
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  //����Ҳ����
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  //
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);  //
 

	//��������
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;//����������ģʽ�·ǹ���״̬ѡ��         
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;//���ÿ���״̬�µķǹ���״̬        
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;//��������ƽ�������������κ�λ            
	TIM_BDTRInitStructure.TIM_DeadTime  = 100;//����ʱ��,0-0xff,����������򿪺͹ر�֮�����ʱ                        
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;//ɲ������ʹ��                              
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;   
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//�Զ����ʹ�� 
	TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure); //
	TIM_BDTRConfig(TIM8,&TIM_BDTRInitStructure); //
	
	
	
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIMx
	TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE �����ʹ��	
	TIM_Cmd(TIM8, ENABLE);  //ʹ��TIM8
	TIM_CtrlPWMOutputs(TIM8,ENABLE);	//MOE �����ʹ��	
	
	pwm_set_off(TIM8, TIM_Channel_1);
	pwm_set_off(TIM8, TIM_Channel_2);
	pwm_set_off(TIM8, TIM_Channel_3);
	pwm_set_off(TIM1, TIM_Channel_1);
}

//********************************************************************************  
/* set phase to "positive without PWM" */
void pwm_set_high(TIM_TypeDef* TIMx, uint16_t TIM_Channel)
{
	TIM_SelectOCxM(TIMx, TIM_Channel, TIM_ForcedAction_Active);
	TIM_CCxCmd(TIMx, TIM_Channel, TIM_CCx_Enable);
	TIM_CCxNCmd(TIMx, TIM_Channel, TIM_CCxN_Disable);
}
//********************************************************************************  
/* set phase to "negative without PWM */
void pwm_set_low(TIM_TypeDef* TIMx, uint16_t TIM_Channel)
{
    TIM_SelectOCxM(TIMx, TIM_Channel, TIM_ForcedAction_InActive);
    TIM_CCxCmd(TIMx, TIM_Channel, TIM_CCx_Disable);
    TIM_CCxNCmd(TIMx, TIM_Channel, TIM_CCxN_Enable);
}
//�������ʹ��
void pwm_set_on(TIM_TypeDef* TIMx, uint16_t TIM_Channel)
{
		TIM_SelectOCxM(TIMx, TIM_Channel, TIM_OCMode_PWM1);
		TIM_CCxCmd(TIMx, TIM_Channel, TIM_CCx_Enable);
		TIM_CCxNCmd(TIMx, TIM_Channel, TIM_CCxN_Enable);
}

//����ȽϹض�
void pwm_set_off(TIM_TypeDef* TIMx, uint16_t TIM_Channel)
{
    TIM_CCxCmd(TIMx, TIM_Channel, TIM_CCx_Disable);
    TIM_CCxNCmd(TIMx, TIM_Channel, TIM_CCxN_Disable);
}





