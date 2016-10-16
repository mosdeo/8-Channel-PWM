#include <REG_MG87FL2051-6051.H>
#include "BIT.h"
#include "SinLookUpTable.h"

void delay1ms(unsigned int x)
{unsigned int i,j;
	
	for(i=0;i<x;++i)
		for(j=0;j<328;++j); //error at j<1967 sometimes,reason unknow
}
void DemoProgram(void);
void PWM_Initial(void);


int main()
{
	PWM_Initial();
	
	P37=0;
	P1=0xFF;
	
	while(1)
	{
		
		DemoProgram();
		
	}
}

void DemoProgram(void)
{	int i,j=0;
	char T=19;
	
	//��sin[n]�άd��k�q0�]��511 (�s��@�ɪ����k:sin(t)�q0~2pi)
			
			for(j=0;j<8;j++)//�����q�D
			{
				CR=0;
				CMOD=0x00+(j<<4);	//�]�w�q�D	
				CR=1;
				
				for(i=0;i<512;++i)
				{
					CCAP0H=~Sin[i];
					CCAP0L=~(255-Sin[i]);
				
					delay1ms(5);
				}
			
			}
		
}

void PWM_Initial(void)
{unsigned char i=0;
	//*************for PWM***********//
	PCON|=0x20; //set PWMEN=1,��PWM-Timer�@PWM
	CMOD|=0x0E; //���W128
	
	//�]�w�q�D��lPWM��
	CCAP0H = 0;
	CCAP0L = 255;
	
	CR=1;//�Ұ�PWM
	//*************for PWM***********//
}