#include "ds1302.h"
#include "common.h"
/********************************************************************/ 


enum
{
	DS1302_SEC_W = 0X80,
	DS1302_SEC_R,
	DS1302_MINUTE_W,
	DS1302_MINUTE_R,
	DS1302_HOUR_W,
	DS1302_HOUR_R,
	DS1302_DATE_W,
	DS1302_DATE_R,
	DS1302_MONTH_W,
	DS1302_MONTH_R,
	DS1302_DAY_W,
	DS1302_DAY_R,
	DS1302_YEAR_W,
	DS1302_YEAR_R,
	DS1302_WP_W,
	DS1302_WP_R
};


#define ds1302_write_enable() {Ds1302_Single_Byte_Write(ds1302_control_addr,0x00);}
#define ds1302_write_disable() {Ds1302_Single_Byte_Write(ds1302_control_addr,0x80);}

uchar bcd2dec(uchar bcd)
{
	uchar tmp;
	tmp = (bcd >> 4) * 10 + (bcd & (0x0f));
	return tmp;
}


/*���ֽ�д��һ�ֽ�����*/
void Write_Ds1302_Byte(unsigned char dat) 
{
	unsigned char i;
	SCK = 0;
	for (i=0;i<8;i++) 
	{ 
		if (dat & 0x01) 	// �ȼ���if((addr & 0x01) ==1) 
		{
			SDA_SET;		//#define SDA_SET SDA=1 /*��ƽ�ø�*/
		}
		else 
		{
			SDA_CLR;		//#define SDA_CLR SDA=0 /*��ƽ�õ�*/
		}		 
		SCK_SET;
		SCK_CLR;		
		dat = dat >> 1; 
	} 
}
/********************************************************************/ 
/*���ֽڶ���һ�ֽ�����*/
unsigned char Read_Ds1302_Byte(void) 
{
	unsigned char i, dat=0;	
	for (i=0;i<8;i++)
	{	
		dat = dat >> 1;
		if (SDA_R) 	  //�ȼ���if(SDA_R==1)    #define SDA_R SDA /*��ƽ��ȡ*/	
		{
			dat |= 0x80;
		}
		else 
		{
			dat &= 0x7F;
		}
		SCK_SET;
		SCK_CLR;
	}
	return dat;
}

/********************************************************************/ 
/*��DS1302 ���ֽ�д��һ�ֽ�����*/
void Ds1302_Single_Byte_Write(unsigned char addr, unsigned char dat)
{ 

	RST_CLR;			/*RST���õͣ�ʵ��DS1302�ĳ�ʼ��*/
	SCK_CLR;			/*SCK���õͣ�ʵ��DS1302�ĳ�ʼ��*/

	RST_SET;			/*����DS1302����,RST=1��ƽ�ø� */
	addr = addr & 0xFE;	 
	Write_Ds1302_Byte(addr); /*д��Ŀ���ַ��addr,��֤��д����,д֮ǰ�����λ����*/	
	Write_Ds1302_Byte(dat);	 /*д�����ݣ�dat*/
	RST_CLR;				 /*ֹͣDS1302����*/
}

/********************************************************************/ 
/*��DS1302���ֽڶ���һ�ֽ�����*/
unsigned char Ds1302_Single_Byte_Read(unsigned char addr) 
{ 
	unsigned char temp;
	RST_CLR;			/*RST���õͣ�ʵ��DS1302�ĳ�ʼ��*/
	SCK_CLR;			/*SCK���õͣ�ʵ��DS1302�ĳ�ʼ��*/

	RST_SET;	/*����DS1302����,RST=1��ƽ�ø� */	
	addr = addr | 0x01;	 
	Write_Ds1302_Byte(addr); /*д��Ŀ���ַ��addr,��֤�Ƕ�����,д֮ǰ�����λ�ø�*/
	temp=Read_Ds1302_Byte(); /*��DS1302�ж���һ���ֽڵ�����*/		
	
	RST=0;
	_nop_();
 	RST=0;
	SCK=0;
	_nop_();
	SCK=1;
	_nop_();
	SD=0;
	_nop_();
	SD=1;
	_nop_();
//	RST_CLR;	/*ֹͣDS1302����*/
	return temp;
}

void ds1302_reset()
{
//	EA = 0;
	ds1302_write_enable();
	Ds1302_Single_Byte_Write(ds1302_min_addr, 0x30);
	Ds1302_Single_Byte_Write(ds1302_hr_addr, 0x08);
	Ds1302_Single_Byte_Write(ds1302_sec_addr, 0x00);
	ds1302_write_disable();
//	EA = 1;
}

void ds1302_gettime(uchar *sec, uchar *min, uchar *hr)
{
	uchar tmp;
	EA = 0;
	tmp = Ds1302_Single_Byte_Read(ds1302_sec_addr+1);
	tmp &= 0x7f;
	*sec = bcd2dec(tmp);
	
	tmp = Ds1302_Single_Byte_Read(ds1302_min_addr+1);
	*min = bcd2dec(tmp);
	
	tmp = Ds1302_Single_Byte_Read(ds1302_hr_addr+1);
	tmp &= 0x7f;
	*hr = bcd2dec(tmp);
	
	EA = 1;
}