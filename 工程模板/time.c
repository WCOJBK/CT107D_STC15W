#include "config.h"
#include "ds1302.h"
#include "time.h"


uchar data time_data[]={16,0,3,6,23,44,58};
uchar data addr[]={0x8c,0x8a,0x88,0x86,0x84,0x82,0x80};
/********************************************************************/ 
/*BCDת��Ϊʮ����*/
uchar BCD2Num(uchar BCD){
	uchar i, j, dec;
	i = BCD % 16;
	j = (BCD >> 4) * 10;
	dec = i + j;
	return dec;
}

/********************************************************************/ 
/*��ʼ����д����������ʱ����*/
void set_rtc(void){	
	uchar i,j;
	for(i = 0; i < 7; i++){
		j=time_data[i]/10;
		time_data[i]=time_data[i]%10;
		time_data[i]=time_data[i]+j*16;
	}
	Ds1302_Single_Byte_Write(0x8e,0x00);   //ȥ��д����
    for(i = 0; i < 7; i++){
   		Ds1302_Single_Byte_Write(addr[i],time_data[i]);
   	}
   	Ds1302_Single_Byte_Write(0x8e,0x80);   //��д����
}

/**************��������������ʱ��������****************************/
void read_rtc(void){
	uchar i;
	for(i=0;i<7;i++){
    	time_data[i]=BCD2Num(Ds1302_Single_Byte_Read(addr[i]+1));
   	}
}
