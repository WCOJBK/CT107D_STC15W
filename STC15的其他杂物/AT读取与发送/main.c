#include <STC15F2K60S2.H>
bit Send_ed = 1;
bit New_rec = 1;
char xdata Rec_data[25] = {0};
char Rec_data_down = 0;

void UartInit(void)		//9600bps@11.0592MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR &= 0xBF;		//��ʱ��1ʱ��ΪFosc/12,��12T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//�趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
	TL1 = 0xE8;		//�趨��ʱ��ֵ
	TH1 = 0xFF;		//�趨��ʱ��ֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1
}
char SendData(char dat){
	if(Send_ed == 1){
	   	Send_ed = 0;
		SBUF = dat;	
		return 1;	
	}
	return 0;
}

void SendString(char *s){
	s++;s++;
	while(*s){
		if(SendData(*s) == 1){
			s++;
		}
	}
}

void main(){
	UartInit();
	ES = 1;
	EA = 1;
	while(1){
	 	if(New_rec == 1){
			New_rec = 0;
			SendString(Rec_data);
		}
	}
}

void ser() interrupt 4{
	if(TI){
		TI = 0;
		Send_ed = 1;
	}
	if(RI){
		RI = 0;
		Rec_data[Rec_data_down] = SBUF;
		if(Rec_data_down == 0 && Rec_data[Rec_data_down] == 'A'){
			Rec_data_down = 1;
		}else if(Rec_data_down == 1 && Rec_data[Rec_data_down] == 'T'){
			Rec_data_down = 2;
		}else if(Rec_data_down >= 2 && Rec_data[Rec_data_down] != 'Q'){
			Rec_data_down++;
		}else if(Rec_data[Rec_data_down] == 'Q'){
			Rec_data[Rec_data_down] = '\0';
			Rec_data_down = 0;
			New_rec = 1;
		}else{
			Rec_data_down = 0;	
		}		
	}
}