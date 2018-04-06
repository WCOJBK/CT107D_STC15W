//�ļ�����main.c

//ÿ10msɨ��һ�ΰ������Ѱ�������ֵ�����������

//ϵͳͷ�ļ�
#include "stc_config.h"

//����ͷ�ļ�
#include "display.h"
#include "key.h"
#include "onewire.h"

//���ܺ���ͷ�ļ�
#include "word_gui.h"
#include "set_gui.h"



//ϵͳ����
uchar sys_stat = WORD_GUI;
uchar sys_temp_upper = 18;
uchar sys_temp_lower = 20;
uchar sys_led_cache = 0xff;


//��ʱ������
uint Key_count = 0;	//���̼�������
bit Key_Flag = 0;		//��־λ

uint Temp_count = 0;	//�¶ȼ�������
bit Temp_Flag = 0;		//��־λ

uint led_count = 0; 	//led�����ʱ����������
uchar led_usedFlag = 0; //led�����ʱ��ʹ�ñ�־


void Timer0Init(void)	
{
	AUXR &= 0x7F;	
	TMOD &= 0xF0;	
	TL0 = 0xCD;	
	TH0 = 0xF8;	
	TF0 = 0;		
	TR0 = 1;	
	ET0 = 1;
	EA = 1;
}



void main(){
	
	//��ʼ��
	uchar Key_temp = 0xff;
	int Temp_temp = 0;
	
	
	P0 = 0x00;
	setOpen;
	P0 = 0xff;
	setLed;

	
	
	Timer0Init();
	while(1){
		switch(sys_stat){
			case WORD_GUI:
				word_gui_funtion();
			
				break;
			case SET_GUI:
				set_gui_funtion();
			
				break;
		}
	}
}



void timer0() interrupt 1{		//1ms
	Key_count++;
	if(Key_count == 10){
		Key_count = 0;
		Key_Flag = 1;
	}
	
	Temp_count++;
	if(Temp_count ==250){
		Temp_count = 0;
		Temp_Flag = 1;
	}
	
	if(led_count != 0){
		led_count--;
	}
	
	
	display();
}
