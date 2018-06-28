#include "stdint.h"
#include"tm4c123gh6pm.h"
void SystemInit() {}
void portf_init()
{
	int volatile a;
	SYSCTL_RCGCGPIO_R |=0x20;
	//while((SYSCTL_PRGPIO_R & 0x20) == 0) {}
	a=20;
	GPIO_PORTF_LOCK_R = 0x4C4F434B;
	GPIO_PORTF_CR_R = 0x1F;
	GPIO_PORTF_AMSEL_R = 0x00;
	GPIO_PORTF_PCTL_R = 0x00000000;
	GPIO_PORTF_DIR_R = 0x0E;
	GPIO_PORTF_AFSEL_R = 0x00000000;
	GPIO_PORTF_PUR_R = 0x11;
	GPIO_PORTF_DEN_R = 0x1F;
	//for the interrupt of SW1 in PF0
	GPIO_PORTF_IS_R &=~(1<<0);
	GPIO_PORTF_IBE_R &=~(1<<0);
	GPIO_PORTF_IEV_R |=(1<<0);
	GPIO_PORTF_ICR_R |=(1<<0); 
	GPIO_PORTF_IM_R |=(1<<0);
	NVIC_EN0_R |=(1<<30);
	NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF00FFFF)|0x00A00000;

}

void portA_init()
{
	SYSCTL_RCGCGPIO_R |=1<<0;		
	SYSCTL_RCGCUART_R |=1<<0;  	
	GPIO_PORTA_AFSEL_R=0x03;
	GPIO_PORTA_PCTL_R |=(1<<0) | (1<<4);
	GPIO_PORTA_DEN_R |=(1<<0) | (1<<1);
	UART0_CTL_R=~1;		
	UART0_IBRD_R=104;
	UART0_FBRD_R=11;
	UART0_LCRH_R =(1<<5) | (1<<6);
	UART0_CTL_R =(1<<0) | (1<<8) | (1<<9);
	UART0_CC_R=0x00;
}

char readChar(void)
{
	while((UART0_FR_R & 0x0010) !=0){}
	return (char)(UART0_DR_R & 0xFF);
}

void printChar(char c)
{
	while((UART0_FR_R & 0x0020)!=0){}
	UART0_DR_R = c;
}

void printString(char* string)
{
	while(*string)
	{
		printChar(*(string++));
	}
}


void GPIOF_Handler(void)
{
	GPIO_PORTF_ICR_R |=(1<<0);
	GPIO_PORTF_DATA_R ^= 0x02;
	if(GPIO_PORTF_DATA_R&0xE)
	{
		printString("led is on \n\r");
	}
	else
	{
		printString("led is off \n\r");
	}
}


int main()
{
	char c;
	portf_init();
	portA_init();
	GPIO_PORTF_DATA_R = 0x02;
	__enable_irq();
	while(1)
	{
		printString("enter \"r\" or \"b\" or \"g\" \n\r");
		c=readChar();
		printChar(c);
		printString("\n\r");
		switch(c)
		{
			case 'r':
				GPIO_PORTF_DATA_R = 0x2;
				break;
			case 'b':
				GPIO_PORTF_DATA_R = 0x4;
				break;
			case 'g':
				GPIO_PORTF_DATA_R = 0x8;
				break;
			default:
				GPIO_PORTF_DATA_R = 0x0;
				break;
		}
	}
	return 0;
}
