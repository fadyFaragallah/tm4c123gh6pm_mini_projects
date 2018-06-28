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
void GPIOF_Handler(void)
{
	GPIO_PORTF_ICR_R |=(1<<0);
	GPIO_PORTF_DATA_R ^= 0x02;
}
int main()
{
	//int x;
	portf_init();
	GPIO_PORTF_DATA_R = 0x02;
	__enable_irq();
	while(1)
	{
		
	}
	return 0;
}
