#include "stc_config.h"
#include "word_gui.h"

#include "onewire.h"
#include "display.h"
#include "key.h"




void reSetLed(){
	ET0 = 0;
	
	sys_led_cache = (~(sys_led_cache&0x01)) | (sys_led_cache&0xfe);
	P0 = sys_led_cache;
	setLed;
	
	ET0 = 1;
}


void word_gui_funtion(void){
	int temp_temp = 0;
	int temp_range = 0;
	int temp_key = 0;
	char i;
	
	led_count = 0;
	
	//��while��ʵ���¼�
	//��������ת������ʱ���޸�now_sys_stat����break����
	//��ʵ����״̬������ת
	
	while(1){
		//��ν�¼�����ʵ���ǹ���
		
		//1.�¶ȵĶ�ȡ
		temp_temp =  getTemp();
		//2.������ж�(���ޣ�����)
		if(temp_temp > sys_temp_upper){
			temp_range = 2;
		}else if(		 temp_temp <= sys_temp_upper 
							&& temp_temp >= sys_temp_lower){
			temp_range = 1;
		}else if(temp_temp < sys_temp_lower){
			temp_range = 0;
		}
		//3.������������жϣ�ִ��LED��̵����Ĳ���
		if(temp_range == 0 || temp_range == 1){
			ET0 = 0;
			//�̵���
			P0 = 0x00;
			setOpen;	
			if(temp_range == 0){
				//��ʼ��
				if(led_usedFlag == 0){
					led_usedFlag = 1;
					led_count = 800;
				}
				
				//�������Ƿ�0 
				//(ͬʱҲҪ������Ƿ��б���ʼ����Ҳ�����б�ʹ��)
				if(led_count == 0 && led_usedFlag == 1){
					led_usedFlag = 0;
					
					//LED��ת
					reSetLed();
				}
				
			}else if(temp_range == 1){
				
				//��ʼ��
				if(led_usedFlag == 0){
					led_usedFlag = 1;
					led_count = 400;
				}
				
				//�������Ƿ�0 
				//(ͬʱҲҪ������Ƿ��б���ʼ����Ҳ�����б�ʹ��)
				if(led_count == 0 && led_usedFlag == 1){
					led_usedFlag = 0;
					
					//LED��ת
					reSetLed();
				}
			}
			
			
			
			ET0 = 1;
		}else if(temp_range == 2){
			ET0 = 0;
			P0 = 0x10;
			setOpen;
			//��ʼ��
			if(led_usedFlag == 0){
				led_usedFlag = 1;
				led_count = 200;
			}
			
			//�������Ƿ�0 
			//(ͬʱҲҪ������Ƿ��б���ʼ����Ҳ�����б�ʹ��)
			if(led_count == 0 && led_usedFlag == 1){
				led_usedFlag = 0;
				
				//LED��ת
				reSetLed();
			}		
			
			ET0 = 1;
		}
		//4.��ʾ����
		dspbuf[7] = temp_temp%10;
		dspbuf[6] = temp_temp/10%10;
		dspbuf[5] = 10;
		dspbuf[4] = 10;
		dspbuf[3] = 10;
		dspbuf[2] = 11;
		dspbuf[1] = temp_range;
		dspbuf[0] = 11;
		
		
		//��ת�ж�
		temp_key = scanKey();
		if(temp_key != 0xff){
			if(temp_key == 16){	//���������ð���
				led_count = 0;
				led_usedFlag = 0;
				for(i = 0; i < 8; i++){
					dspbuf[i] = 10;
				}
				sys_stat = SET_GUI;
				break;
			}
		}
		
		break;
	}
}