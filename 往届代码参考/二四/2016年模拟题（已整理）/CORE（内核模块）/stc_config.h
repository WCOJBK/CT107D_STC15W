//�ļ�����stc_config.h

#ifndef _STC_CONFIG_H_
#define _STC_CONFIG_H_

#include <STC15F2K60S2.H>
#include <intrins.h>

#define uchar unsigned char			//0-255
#define uint unsigned int				//0-65535
#define u32 unsigned long				//0- 11111111 11111111 11111111 11111111


#define WORD_GUI 0
#define SET_GUI 1


#define LED_STAT_COLSE 0
#define LED_STAT_0_8 1
#define LED_STAT_0_4 2
#define LED_STAT_0_2 3


extern uchar sys_stat;
extern uchar sys_temp_upper;
extern uchar sys_temp_lower;
extern uchar sys_led_cache;


//�����ʱ����
extern uint led_count; 	//led�����ʱ����������
extern uchar led_usedFlag; //led�����ʱ��ʹ�ñ�־



//38�������궨��
#define setLed P2=((P2&0x1f)|0x80);_nop_();_nop_();P2=(P2&0x1f);
#define setOpen P2=((P2&0x1f)|0xA0);_nop_();_nop_();P2=(P2&0x1f);




#endif
