#include <REG_MG87FL2051-6051.H>
#include "SinLookUpTable.h"

#define XTAL 44236800
#define TIMER0_INTERVAL 168 //0.05ms
#define TH0_NUM (65535-TIMER0_INTERVAL)>>8
#define TL0_NUM (65535-TIMER0_INTERVAL)&0x00ff

void DemoProgram(void);
void delay1ms(unsigned int x)
{unsigned int i,j;
	
	for(i=0;i<x;++i)
		for(j=0;j<328;++j);
}

unsigned char PWM[10]={255,255,255,255,255,255,255,255,255,255};
unsigned char PWM_DutyCycle=0;
unsigned char RxBuf;

int main()
{
	EA=1; //啟用中斷
	ET0=1;
	
	TMOD|=0x01;// 使用Timer0 mode1
	TH0=TH0_NUM;
	TL0=TL0_NUM;
	TR0=1;//啟動Timer0
	
	//*************for UART***********//
	TMOD|=0x20;// 使用Timer1 mode2
	SCON=0x70; //
	TH1=TL1=250; 
	TR1=1; /*Timer1 啟動!*/
	TI=1; //使傳送可以傳送
	//*************for UART***********//
	
	while(1)
	{
		
		if(RI)
		{
			RI=0;
			RxBuf = SBUF;
			
			if(RxBuf>96 && RxBuf<123)RxBuf-=32; //小寫變大寫
			
			switch(RxBuf)
			{
				case'A':PWM[0]=~PWM_DutyCycle,PWM_DutyCycle=0;break;
				case'B':PWM[1]=~PWM_DutyCycle,PWM_DutyCycle=0;break;
				case'C':PWM[2]=~PWM_DutyCycle,PWM_DutyCycle=0;break;
				case'D':PWM[3]=~PWM_DutyCycle,PWM_DutyCycle=0;break;
				case'E':PWM[4]=~PWM_DutyCycle,PWM_DutyCycle=0;break;
				case'F':PWM[5]=~PWM_DutyCycle,PWM_DutyCycle=0;break;
				case'G':PWM[6]=~PWM_DutyCycle,PWM_DutyCycle=0;break;
				case'H':PWM[7]=~PWM_DutyCycle,PWM_DutyCycle=0;break;
				case'I':PWM[8]=~PWM_DutyCycle,PWM_DutyCycle=0;break;
				case'J':PWM[9]=~PWM_DutyCycle,PWM_DutyCycle=0;break;
				case'X':DemoProgram();break;
					
				default:
					if(RxBuf>47 && RxBuf<58)//0~9
					{
						RxBuf-=48; //ASCII to Dec
						PWM_DutyCycle = PWM_DutyCycle*10;
						PWM_DutyCycle = PWM_DutyCycle + RxBuf;
					}
					break;
			}
		}
		
		
	}
}


void Timer0_ISR(void) interrupt 1
{
	//unsigned char i;
	static unsigned char SawtoothWave=0;
	P37=1;
	
	TR0=0;
	TH0=TH0_NUM;
	TL0=TL0_NUM;
	TR0=1;
	
	++SawtoothWave;
	
	//迴圈很慢
// 	for(i=0;i<8;i++)
// 	{
// 		if(PWM[i]>=SawtoothWave)P1|=1<<i;else P1&=~(1<<i);
// 	}
	
	
	//手動inline比較快
	if(PWM[0]>=SawtoothWave)P1|=0x01;else P1&=~(0x01);
	if(PWM[1]>=SawtoothWave)P1|=0x02;else P1&=~(0x02);
	if(PWM[2]>=SawtoothWave)P1|=0x04;else P1&=~(0x04);
	if(PWM[3]>=SawtoothWave)P1|=0x08;else P1&=~(0x08);
	if(PWM[4]>=SawtoothWave)P1|=0x10;else P1&=~(0x10);
	if(PWM[5]>=SawtoothWave)P1|=0x20;else P1&=~(0x20);
	if(PWM[6]>=SawtoothWave)P1|=0x40;else P1&=~(0x40);
	if(PWM[7]>=SawtoothWave)P1|=0x80;else P1&=~(0x80);
	if(PWM[8]>=SawtoothWave)P42=1;	 else P42=0;
	if(PWM[9]>=SawtoothWave)P43=1;	 else P43=0;
	
	
	P37=0;
}

void DemoProgram(void)
{unsigned int i,j;
	
	for(i=0;i<512;i++)
	{
		for(j=0;j<8;j+=2)
			PWM[j]=~Sin[i];
		
		for(j=1;j<8;j+=2)
			PWM[j]=Sin[i];
		
		delay1ms(6);
	}
	
	
	for(i=0;i<256;i++)
	{
		for(j=1;j<8;j+=2)
			PWM[j]=Sin[i];
		
		delay1ms(6);
	}
	
}