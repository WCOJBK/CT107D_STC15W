#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#define setY3 P2 = ((P2&0x1f) | 0x60); _nop_(); _nop_(); P2=P2&0x1f	//8255��CE
#define setY4 P2 = ((P2&0x1f) | 0x80); _nop_(); _nop_(); P2=P2&0x1f	//LED
#define setY5 P2 = ((P2&0x1f) | 0xA0); _nop_(); _nop_(); P2=P2&0x1f	//UNL2003
#define setY6 P2 = ((P2&0x1f) | 0xC0); _nop_(); _nop_(); P2=P2&0x1f	//�����λѡ
#define setY7 P2 = ((P2&0x1f) | 0xE0); _nop_(); _nop_(); P2=P2&0x1f	//����ܶ�ѡ


extern uchar dspbuf[];

void display(void);
#endif