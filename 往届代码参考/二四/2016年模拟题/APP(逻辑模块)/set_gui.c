/*2016��ģ����

1.�¶ȼ�⣬�¶ȼ����� DS18B20 �¶ȴ����������û��趨���¶�
�����ޱȽϣ�ȷ����ǰ�¶����������䡣
�¶����� 0�� ��ǰ�¶�<TMIN 
�¶����� 1�� TMIN�ܵ�ǰ�¶ȡ�TMAX 
�¶����� 2�� ��ǰ�¶�>TMAX 
���趨������¶����䣺0�桫99�� 

2.ͨ��3*4��������趨ϵͳ�Ĺ�����������������
0    1    2
3    4    5
6    7    8
9   ���� ���

3.�����á��������º󣬽��빤�������趨���档�趨 TMAXΪ 35 ���϶ȣ�
TMINΪ 25 ���϶�Ϊ��˵�������趨���̣����¡����á�
������Ȼ�����ΰ������ְ�����3�� ��5�� ��2�� ��5�����ٴΰ��¡���
�á���������ɲ����趨�����˳������趨���档����������У����¡��������
�����������ǰ�������ݣ����趨�������������� TMAX<TMIN��L2 ���������������趨�����������L2 Ϩ��

4.
ִ�л�����ָʾ�� L1 �ͼ̵�����ɣ����ڱ����������ⲿ�ߵ���ִ�л����� 
3.1 ʵʱ�¶ȴ����¶����� 0���̵����رգ�ָʾ�� L1 �� 0.8 ��Ϊ�����˸�� 
3.2 ʵʱ�¶ȴ����¶����� 1���̵����رգ�ָʾ�� L1 �� 0.4 ��Ϊ�����˸�� 
3.3 ʵʱ�¶ȴ����¶����� 2���̵����򿪣�ָʾ�� L1 �� 0.2 ��Ϊ�����˸�� 

5.ϵͳĬ�ϵ��¶����ޣ�TMAX��Ϊ 30�棬�¶�����(TMIN)Ϊ 20�棬����ͨ���������
�޸ġ�

*/
#include "stc_config.h"
#include "set_gui.h"

#include "display.h"
#include "key.h"


//�������ռ�
uchar buffer_blank[4] = {0};
uchar buffer_upper = 0;

//ע�����
//		����
void addBufferData(uchar input){
	if(buffer_upper != 4){
		buffer_blank[buffer_upper] = input;
		buffer_upper++;
	}
}

//ע�����NULL
void delBufferDate(){
	uchar i;
	for(i = 0; i < 4; i++){
		buffer_blank[i] = 0;
	}
	buffer_upper = 0;
}

//ת������
//���裺����ת���������жϣ����ݴ���
//ע����������ޣ����ݴ�����û�з���Ҫ��
uchar transBufferDate(){
	uchar output = 0;
	uchar temp_upper = 0;
	uchar temp_lower = 0;
	
	//����ת��
	temp_upper = buffer_blank[0] *10 +  buffer_blank[1];
	temp_lower = buffer_blank[2] *10 +  buffer_blank[3];
	
	//���û�дﵽ4��λ����output = 1��
	if(buffer_upper != 4){
		output = 1;
	}
	//���޲���������,��output = 2��
	else if(temp_upper <= temp_lower){
		output = 2;
	}
	//��������������򴫸�������
	else{
		output = 0;
		sys_temp_upper = temp_upper;
		sys_temp_lower = temp_lower;
	}
	
	return output;
}




void set_gui_funtion(void){
	uchar temp_key = 0xff;
	uchar temp_buffer = 0;
	uchar i;
	while(1){
	//�¼�
	//1.��ʾ

		dspbuf[0] = 11;
		dspbuf[1] = buffer_blank[0];
		dspbuf[2] = buffer_blank[1];	
		dspbuf[5] = 11;
		dspbuf[6] = buffer_blank[2];
		dspbuf[7] = buffer_blank[3];		

		//��ת�ж�+�¼�����
		temp_key = scanKey();
		if(temp_key != 0xff){
			//����������
			//��������
			if(temp_key!= 16 && temp_key != 17){
				addBufferData(temp_key);
			}
			//���
			if(temp_key == 17){
				delBufferDate();
			}
			
			
			//��ת�жϡ�����
			if(temp_key == 16){	//���������ð���
				
				//���û�����⣬�ٱ�����ת
				temp_buffer = transBufferDate();
				if(temp_buffer == 0){
					ET0 = 0;
					
					sys_led_cache |= 0x02;
					P0 = sys_led_cache;
					
					setLed;
					ET0 = 1;					
					for(i = 0; i < 8; i++){
						dspbuf[i] = 10;
					}
					sys_stat = WORD_GUI;
					break;
				}
				//����
				else if(temp_buffer == 2){
					ET0 = 0;
					
					sys_led_cache &= ~(1<<1);
					P0 = sys_led_cache;
					
					
					
					setLed;
					ET0 = 1;
				}

			}
		}		
	}
}

