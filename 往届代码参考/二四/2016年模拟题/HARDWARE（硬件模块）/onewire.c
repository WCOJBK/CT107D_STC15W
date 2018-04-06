/*
  ����˵��: ��������������
  �������: Keil uVision 4.10 
  Ӳ������: CT107��Ƭ���ۺ�ʵѵƽ̨
  ��    ��: 2011-8-9
*/

#include "onewire.h"

//��������ʱ����
void Delay_OneWire(unsigned int t)
{
	uchar i;
	while(t--){
 		 for(i=0;i<12;i++);
	}
}

//DS18B20оƬ��ʼ��
bit Init_DS18B20(void)
{
	bit initflag = 0;
	DQ = 1;
	Delay_OneWire(12);
	DQ = 0;
	Delay_OneWire(80); //480us
	DQ = 1;		//	
	Delay_OneWire(10); //60us
	initflag = DQ;    //����Ƿ��ʼ�����	
	Delay_OneWire(5);
  
	return initflag;
}

//ͨ����������DS18B20дһ���ֽ�//8λ


void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ = 0;
		DQ = dat & 0x01;//0����1
		Delay_OneWire(5);//30us
		DQ = 1;
		dat >>= 1;
	}
	Delay_OneWire(5);
}

//��DS18B20��ȡһ���ֽ�
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(5);
		//30us
	}
	return dat;
}

int getTemp(void){
	uchar a, b;
	uint temp;//��ʱ����
	float f_temp;//��ʱ����//���Լ���С������
	
	//��ʼ��
	Init_DS18B20();//������ʼ��
	Write_DS18B20(0xcc);	//ѡ���¶�
	Write_DS18B20(0x44);	//ת���¶�

	
	//��ȡ�¶�
	Init_DS18B20();
	Write_DS18B20(0xcc);//ѡȡ�¶�оƬ
	Write_DS18B20(0xbe);//��ȡ����

	a = Read_DS18B20(); //�ش�һ��16λ���¶����ݡ�
	b = Read_DS18B20();	//A�ǵ�λ��B�Ǹ�λ
	
	//�����¶ȸ���������0000 1111 0000 1011
	//                 b = 0000 1111, a = 0000 1011
	//1.b ��char,��ֻ��8λ�����š������ȷ���temp�
	//	temp = b => temp = 0000 0000 0000 1111
	//2.��temp����������ƶ�����λ����Ϊb�������ڸ�λ
	//  temp <<= 8 => temp = 0000 1111 0000 0000
	//3.��temp ��A����һ�𣬱��ԭ��������
	//  temp |= a =>  temp = 0000 1111 0000 1011
	
	//���ݴ���
		//����Ҫ��A,B����һ��
	temp = b;
	temp <<= 8;
	temp |= a;
		//ת��������ʵ�ʵ��¶�
	
	f_temp = temp;
	f_temp = f_temp *0.0625;		//�õ�ʵ�ʵ��¶�
				// temp >> 4
				
				
		//��������		f_temp = 24.5625
	temp = f_temp + 0.5;
	
	
	return temp; //25
  
	
	
	
}

//��char дchar
//����ʽ������Ҫдһ�������һ������  
//Write_DS18B20(0x01);	//����
//Read_DS18B20(data); //����


//�Ĵ����ķ�ʽ��
//Write_DS18B20(0x01);	//��ȡ����д��
//Write_DS18B20(addr); //�Ĵ����ĵ�ַ
//Read_DS18B20(data); //�Ĵ�������
