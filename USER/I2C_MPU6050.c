/******************** (C) COPYRIGHT 2012  **************************

 * Ӳ�����ӣ�-----------------
 *          |                 |
 *          |  PB6-I2C1_SCL		|
 *          |  PB7-I2C1_SDA   |
 *          |                 |
 *           -----------------
 * ��汾  ��ST3.5.0
 * ����    �� Orange 
**********************************************************************************/
#include "stm32f10x.h"
#include "I2C_MPU6050.h"
#include "delay.h"

/*
 * ��������I2C_GPIO_Config
 * ����  ��I2C1 I/O����
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
static void I2C_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
		/* ʹ���� I2C1 �йص�ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB,ENABLE); 
	  
	 /* PB6-I2C1_SCL��PB7-I2C1_SDA*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; //��©���
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
}


/*
 * ��������I2C_Mode_Config
 * ����  ��I2C ����ģʽ����
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
static void I2C_Mode_Config(void)
{
	I2C_InitTypeDef I2C_InitStructure; 
	 
	  /* I2C ���� */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C ; 
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; 
	I2C_InitStructure.I2C_OwnAddress1 = 0x01; //�����ַ����Ҫ����豸��ַ��ͬ
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; 
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; 
	I2C_InitStructure.I2C_ClockSpeed = 400000;//���Ϊ400khz  
	I2C_Init(I2C1, &I2C_InitStructure);		/* I2C1 ��ʼ�� */ 
	
	I2C_Cmd(I2C1,ENABLE); /* ʹ�� I2C1 */
	I2C_AcknowledgeConfig(I2C1, ENABLE);/*����Ӧ��ģʽ*/   
}
/***************************************************************************************/
/*
 * ��������I2C_MMA_Init
 * ����  ��I2C ����(MMA7455)��ʼ��
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void I2C_config(void)
{	   
 	I2C_GPIO_Config();
	I2C_Mode_Config();
}  

/*
 * ��������I2C_ByteWrite
 * ����  ��дһ���ֽڵ�I2C�豸�Ĵ�����
 * ����  ��REG_Address �������ݵ�IIC�豸�Ĵ����ĵ�ַ 
 *         REG_data ��д�������
 * ���  ����
 * ����  ����
 * ����  ���ڲ�����
 */	
void I2C_ByteWrite(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data)
{
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));//��������Ƿ�æ ���ǿ� SCL ��SDA�Ƿ�Ϊ �� 
	I2C_AcknowledgeConfig(I2C1,ENABLE);//�������Ӧ��
	I2C_GenerateSTART(I2C1,ENABLE);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(I2C1,SlaveAddress,I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2C1,REG_Address);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_SendData(I2C1,REG_data);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_GenerateSTOP(I2C1,ENABLE);

}


/*
 * ��������I2C_ByteRead
 * ����  ����IIC�豸�Ĵ����ж�ȡһ���ֽ�
 * ����  ��REG_Address ��ȡ���ݵļĴ����ĵ�ַ 
 * ���  ����
 * ����  ����
 * ����  ���ڲ����� 
*/
uint8_t I2C_ByteRead(uint8_t SlaveAddress, uint8_t REG_Address)
{
uint8_t REG_data;

while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));//��������Ƿ�æ ���ǿ� SCL ��SDA�Ƿ�Ϊ �� 

I2C_GenerateSTART(I2C1,ENABLE);//������ʼ�ź�
while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));//����Ϊ��ģʽ���

I2C_Send7bitAddress(I2C1,SlaveAddress,I2C_Direction_Transmitter);//�����豸��ַ+д�ź�

while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//

I2C_Cmd(I2C1,ENABLE);

I2C_SendData(I2C1,REG_Address);//���ʹ洢��Ԫ��ַ����0��ʼ

while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));

I2C_GenerateSTART(I2C1,ENABLE);//��ʼ�ź�
while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));

I2C_Send7bitAddress(I2C1,SlaveAddress,I2C_Direction_Receiver);//�����豸��ַ+���ź�
while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));//��ַ�������

I2C_AcknowledgeConfig(I2C1,DISABLE);//������Ҫ��ʱ���ر�Ӧ��
I2C_GenerateSTOP(I2C1,ENABLE);//����ֹͣ�ź�
while(!(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)));//�ȴ��������

REG_data=I2C_ReceiveData(I2C1);//�����Ĵ�������

return REG_data;

}

/*
 * ��������void InitMPU6050(void)
 * ����  ����ʼ��Mpu6050
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void InitSensor(void)
{
	/***********************MPU9250_SET********************************************/
	delayms(10);
	I2C_ByteWrite(MPU6050_ADDRESS,PWR_MGMT_1,0x00);//�������״̬
	delayms(10);
	I2C_ByteWrite(MPU6050_ADDRESS,SMPLRT_DIV,0x07);//������1000/(1+7)=125HZ
	delayms(10);
	I2C_ByteWrite(MPU6050_ADDRESS,CONFIG,0x06);//��ͨ�˲��� 0x06 5hz
	delayms(10);
	I2C_ByteWrite(MPU6050_ADDRESS,GYRO_CONFIG,0x18);//�����ǲ�����Χ 0X18 ����2000��
	delayms(10);
	I2C_ByteWrite(MPU6050_ADDRESS,ACCEL_CONFIG,0x00);//���ٶ���������2g  
	delayms(10);
	I2C_ByteWrite(MPU6050_ADDRESS,0x37,0x02);// Set by pass mode for the magnetometers
	delayms(10);
	
	/***********************MAG_SET********************************************/
	I2C_ByteWrite(MAG_ADDRESS,0x0B,0x01); // Reset Device
	delayms(10);
	I2C_ByteWrite(MAG_ADDRESS,0x0A,0x10);	// Power-down mode
	delayms(10);
	I2C_ByteWrite(MAG_ADDRESS,0x0A,0x11); //Request first magnetometer single measurement
	delayms(10);
	
	/***************************BMP280_SET*************************************/
	I2C_ByteWrite(BMP280_ADDRESS,0xF4,0x25);//����forced mode��ÿ�β����궼Ҫ�ص�sleep mode
	delayms(10);
	
}


/*
 * ��������GetData
 * ����  �����16λ����
 * ����  ��REG_Address �Ĵ�����ַ
 * ���  �����ؼĴ�������
 * ����  ���ⲿ����
 */
//�з�����
int16_t GetData_GYRO(unsigned char REG_Address)
{
	uint8_t H,L;
	H=I2C_ByteRead(MPU6050_ADDRESS,REG_Address);
	L=I2C_ByteRead(MPU6050_ADDRESS,REG_Address+1);
	return (H<<8)+L;   //�ϳ�����

}
//�з����� ���Ƿ������Ϊ������ȡģʽ
int16_t GetData_MAG(unsigned char REG_Address)
{
	uint8_t H,L;
	uint8_t ST1;
	I2C_ByteWrite(MAG_ADDRESS,0x0A,0x11);//�����ֲ�֪���β���ģʲ������ص�powerdownģʽ����Ҫ����һ��
	delayms(1);
	
  do// Read register Status 1 and wait for the DRDY: Data Ready
  {
    ST1=I2C_ByteRead(MAG_ADDRESS,0x02);
  }
  while(!(ST1&0x01));
	 
	L=I2C_ByteRead(MAG_ADDRESS,REG_Address);
	H=I2C_ByteRead(MAG_ADDRESS,REG_Address+1);

	return (H<<8)+L;   //�ϳ�����

}

//�Ƿ��Ǵ�����������ȷ��
int16_t GetData_BMP280(unsigned char REG_Address)
{
	uint8_t H,L;
	I2C_ByteWrite(BMP280_ADDRESS,0xF4,0x25);//����forced mode��ÿ�β����궼Ҫ�ص�sleep mode
	delayms(5);
	H=I2C_ByteRead(BMP280_ADDRESS,REG_Address);
	L=I2C_ByteRead(BMP280_ADDRESS,REG_Address+1);
	return (H<<8)+L;   //�ϳ�����

}


/******************* (C) COPYRIGHT 2012  *****END OF FILE************/



