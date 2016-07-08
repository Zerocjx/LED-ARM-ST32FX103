#include "init.h"
//crc16���麯��
unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++){      
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++) {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}
/*
	��ϴ���ʾ���������Э�飬float�ֳ�Ϊ16λ��ע���еĹ���Ϊ32λ
	1�����������·��Ϣ��ÿ·���ݳ�����16Ϊ��
	2��ÿ����������ֽ�Ϊ10�����������Ϊ���顣

ֻ��Ҫ����ʾ�����ݸ�ֵ��Outdata[],�������
*/
float OutData[4]={0};
void OutPut_Data(void)
{
  int temp[4] = {0};
  unsigned int temp1[4] = {0};
  unsigned char databuf[10] = {0};
  unsigned char i;
  unsigned short CRC16 = 0;
  for(i=0;i<4;i++)
   {
    
    temp[i]  = (int16_t)OutData[i];
    temp1[i] = (uint16_t)temp[i];
    
   }
   
  for(i=0;i<4;i++) 
  {
    databuf[i*2]   = (int8_t)(temp1[i]%256);
    databuf[i*2+1] = (int8_t)(temp1[i]/256);
  }
  
  CRC16 = CRC_CHECK(databuf,8);
  databuf[8] = CRC16%256;
  databuf[9] = CRC16/256;
  
  for(i=0;i<10;i++){
		USART_SendData(USART1, databuf[i]);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) ;
	}
}
/*
ʹ�ô˺���������α��벨����Ϊ9600
������������ѭ���
*/
void showwave(float a,float b,float c,float d)
{
	OutData[0] = a;
	OutData[1] = b;
	OutData[2] = c;
	OutData[3] = d;
	OutPut_Data();
}
