#ifndef _ONEWIRE_H_
#define	_ONEWIRE_H_
#include<stc15.h>
#define OW_SKIP_ROM 0xcc
#define DS18B20_CONVERT 0x44
#define DS18B20_READ 0xbe
#define uchar unsigned char
#define uint unsigned int
//IC���Ŷ���
sbit DQ = P1^4;

/*
  ����˵��: ��������������
  �������: Keil uVision 4.10 
  Ӳ������: CT107��Ƭ���ۺ�ʵѵƽ̨
  ��    ��: 2011-8-9
*/



//��������ʱ����
void Delay_OneWire(unsigned int t);
//DS18B20оƬ��ʼ��
bit Init_DS18B20(void);
//ͨ����������DS18B20дһ���ֽ�
void Write_DS18B20(unsigned char dat);
//��DS18B20��ȡһ���ֽ�
unsigned char Read_DS18B20(void);
//��ȡ�¶�

uint readTheTemp(void);
#endif