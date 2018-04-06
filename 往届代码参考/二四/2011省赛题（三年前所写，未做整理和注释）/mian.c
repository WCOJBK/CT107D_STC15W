#include <config.h>
#include <init.h>
#include <display.h>
#include <key.h>
#include <onewire.h>
#include <at24c02.h> 

uchar inr = 0;	  //��������
uchar key_flag = 0;			//������־
uchar key_temp = 0;			//��ǰ���°����洢

uint temp_inr = 0;		   //�¶ȶ�ȡ������ÿ250ms��һ��
uchar temp_flag = 0;	   //�¶ȱ�־
uchar tempValue = 0;	   //��ǰ��ȡ�¶�

uchar temp_up = 30;		   //����
uchar temp_down = 20;	   //����

void Timer0Init(void)		//1����@12.000MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x20;		//���ö�ʱ��ֵ
	TH0 = 0xD1;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
}

void Timer1Init(void)		//100΢��@12.000MHz
{
	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0x50;		//���ö�ʱ��ֵ
	TH1 = 0xFB;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
}


void main(){
	gpio();					//��ʼ�����
	Timer0Init();			//��ʼ��������0 ���ڼ���
	Timer1Init();
	EA = 1;				  
	PT1 = 1;			  //��������1�����ȶȵ�Ϊ���
	PT0 = 0;			  //��������0�����ȶȵ�Ϊ0�������
	ET0 = 1;
	temp_up = readEeprom(0x00);					 //��ȡ������
	temp_down = readEeprom(0x01);
	if(temp_down >= temp_up){
		temp_down = temp_up;
	}							 //����ȡ���ݻ��ҵ�ʱ�����һ������ʹ���������
	while(1){
		if(key_flag){
			key_flag = 0;
		   	key_temp = scanKey();	  //ɨ������������ö�������Ϊ״̬�����޷�ͬʱʶ�����Σ��ް���ʱ�򷵻�0xff
			if(key_temp != 0xff){
				switch(key_temp){
					case 1:
						if(temp_down > 0){
							temp_down--;
						}
						break;
					case 2:
						if(temp_up > temp_down){
							temp_up--;
						}
						break;
					case 3:
						if(temp_down < temp_up){
							temp_down++;
						}
						break;
					case 4:
						if(temp_up < 99){
							temp_up++;
						}
						break;
				}
				writeEeprom(0x00, temp_up);				//��ʾ������
				writeEeprom(0x01, temp_down);				
			}
		}
		if(temp_flag){
			temp_flag = 0;
			tempValue = (uchar)(readTheTemp() /100);
			settemp(tempValue);
			if(tempValue > temp_up){
				ET1 = 1;				  //��PWM

				ET0 = 0;				  //��ʱ�ر���ʾ��ʱ������ֹ���ŵ�P0��
				P0 = 0x00; 
				setY5();
				ET0 = 1;
			}else if(tempValue < temp_down){
				ET1 = 0;
				P34 = 1;

				ET0 = 0;
				P0 = 0x10; 
				setY5();
				ET0 = 1;
			}else{
				ET1 = 0;
				P34 = 1;

				ET0 = 0;
				P0 = 0x00; 
				setY5();
				ET0 = 1;				
			}
		}

		setdown(temp_down);			//��ʾ������
		setup(temp_up);
	}
}

void time0_isr() interrupt 1{
	display();	
	if(++inr == 10){
		inr = 0;
		key_flag = 1;
	}
	if(++temp_inr == 250){
		temp_inr = 0;
		temp_flag = 1;
	}
}

void time1_isr() interrupt 3{
	static uchar cnt;
	if(++cnt == 3){
		P34 = 0;
	}else if(cnt >= 10){
		cnt = 0;
		P34 = 1;
	}		
}