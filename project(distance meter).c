/*#include"stdint.h"
#include"D:/CSE_3rd_year/uC/tm4c123gh6pm.h"
void SystemInit(){}
	
void init()
{
	SYSCTL_RCGCGPIO_R |=0x20;
	while((SYSCTL_RCGCGPIO_R&0x20)==0x00){};
	GPIO_PORTF_LOCK_R=0x4c4f4e4b;
	GPIO_PORTF_CR_R=0x1f;
	GPIO_PORTF_AFSEL_R=0;
	GPIO_PORTF_AMSEL_R=0;
	GPIO_PORTF_DIR_R=0x0E;
	GPIO_PORTF_DEN_R=0X1F;
	GPIO_PORTF_PUR_R=0x11;
	
}

	
int main()
{

}*/

#include <stdlib.h>
//#include "tmc.h"
#include"D:/CSE_3rd_year/uC/tmc.h"


#define ULTRA_SONIC_PORT GPIO_PORTF_DATA_R
#define TRIGGER_PIN 2
#define ECHO_PIN 3

#define UNITS_SEG GPIO_PORTA_DATA_R
#define TENS_SEG GPIO_PORTB_DATA_R
#define HUNDREDS_SEG GPIO_PORTD_DATA_R

typedef unsigned char uint8_t;

void PortF_Init(void);
void clock_init(int clk);

void PortA_Init(void);
void PortB_Init(void);
void PortD_Init(void);
void delay(int millis);
void systick_init(void);
void delayTenMicro2();
void delayTenMs();
void delay(int millis);
void show(int n);
void PortE_Init(void);
uint8_t SevenSegment(uint8_t count,uint8_t dp, uint8_t dec_hex);
void SystemInit(){}

int main(void){

  PortF_Init();
  PortA_Init();
  PortB_Init();
  PortD_Init();
    systick_init();
    //initialize portat el 7 segments


while(1){int flag=  0;
            ULTRA_SONIC_PORT |= 1<<TRIGGER_PIN;
            delayTenMicro2();
           ULTRA_SONIC_PORT &= ~(1<<TRIGGER_PIN);
            int end = 0;
	
	
	            show(300);


            while(!(ULTRA_SONIC_PORT & 1<<ECHO_PIN));

            systick_init();
            while(ULTRA_SONIC_PORT & (1<<ECHO_PIN))
            {
                if((NVIC_ST_CTRL_R & 1<<16))
                {
                    flag = 1;
                    break;
                }

            }

            end =  NVIC_ST_CURRENT_R;
            int timeTaken = 0x00FFFFFF - end;

if(flag==1)
    show(300);
else (show(timeTaken*80/58));
flag = 0;



  }

}


void systick_init(void)
{
    NVIC_ST_CTRL_R = 0;

    NVIC_ST_RELOAD_R = 0x00FFFFFF;

    NVIC_ST_CURRENT_R = 0;

    NVIC_ST_CTRL_R = 0x5;
}
void PortF_Init(void){ volatile unsigned long delay;

  SYSCTL_RCGC2_R |= 0x00000020;     // 1) activate clock for Port F

  delay = SYSCTL_RCGC2_R;           // allow time for clock to start

  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F

  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0

  // only PF0 needs to be unlocked, other bits can't be locked

  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PF

  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0

  GPIO_PORTF_DIR_R = 0x1F;          // 5) PF4,PF0 in, PF3-1 out

  GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0

  //GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4

  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital I/O on PF4-0

}



void PortA_Init(void){ volatile unsigned long delay;

  SYSCTL_RCGC2_R |= 0x00000001;     // 1) activate clock for Port F

  delay = SYSCTL_RCGC2_R;           // allow time for clock to start

  GPIO_PORTA_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F

  GPIO_PORTA_CR_R = 0x7F;           // allow changes to PF4-0

  // only PF0 needs to be unlocked, other bits can't be locked

  GPIO_PORTA_AMSEL_R = 0x00;        // 3) disable analog on PF

  GPIO_PORTA_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0

  GPIO_PORTA_DIR_R = 0x7F;          // 5) PF4,PF0 in, PF3-1 out

  GPIO_PORTA_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0

  GPIO_PORTA_DEN_R = 0x7F;          // 7) enable digital I/O on PF4-0

}

void PortB_Init(void){ volatile unsigned long delay;

  SYSCTL_RCGC2_R |= 0x00000002;     // 1) activate clock for Port F

  delay = SYSCTL_RCGC2_R;           // allow time for clock to start

  GPIO_PORTB_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F

  GPIO_PORTB_CR_R = 0x7F;           // allow changes to PF4-0

  // only PF0 needs to be unlocked, other bits can't be locked

  GPIO_PORTB_AMSEL_R = 0x00;        // 3) disable analog on PF

  GPIO_PORTB_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0

  GPIO_PORTB_DIR_R = 0x7F;          // 5) PF4,PF0 in, PF3-1 out

  GPIO_PORTB_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0

  GPIO_PORTB_DEN_R = 0x7F;          // 7) enable digital I/O on PF4-0

}



void show(int n)
{
    int units;
	int tens;
	int hundreds;
    units = n%10;
    tens = ((n%100) - units)/10;
    hundreds = ((n%1000) - (tens + units))/100;
    UNITS_SEG = SevenSegment(units,0,10);
  //  GPIO_PORTF_DATA_R = (GPIO_PORTF_DATA_R & 3) | (SevenSegment(units,0,10)<<2);
    TENS_SEG = SevenSegment(tens,0,10);
    HUNDREDS_SEG = SevenSegment(hundreds,0,10);
}






void PortD_Init(void){ volatile unsigned long delay;

  SYSCTL_RCGC2_R |= 0x00000008;     // 1) activate clock for Port F

  delay = SYSCTL_RCGC2_R;           // allow time for clock to start

  GPIO_PORTD_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F

  GPIO_PORTD_CR_R = 0x7F;           // allow changes to PF4-0

  // only PF0 needs to be unlocked, other bits can't be locked

  GPIO_PORTD_AMSEL_R = 0x00;        // 3) disable analog on PF

  GPIO_PORTD_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0

  GPIO_PORTD_DIR_R = 0x7F;          // 5) PF4,PF0 in, PF3-1 out

  GPIO_PORTD_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0

  GPIO_PORTD_DEN_R = 0x7F;          // 7) enable digital I/O on PF4-0

}





void delayTenMicro2()
{
    NVIC_ST_CTRL_R = 0;
    NVIC_ST_RELOAD_R = 950;
    NVIC_ST_CURRENT_R = 0;
    NVIC_ST_CTRL_R = 0x5;

    while(!(NVIC_ST_CTRL_R & 1<<16));

}



void delayTenMs()
{
    NVIC_ST_CTRL_R = 0;
    NVIC_ST_RELOAD_R = 800000;
    NVIC_ST_CURRENT_R = 0;
    NVIC_ST_CTRL_R = 0x5;

    while(!(NVIC_ST_CTRL_R & 1<<16));
}

void delay(int millis)
{
    millis/=10;
    int i=0;
    for(i=0; i<millis; i++)
    delayTenMs();
}

uint8_t SevenSegment(uint8_t count,uint8_t dp, uint8_t dec_hex)
{
    uint8_t PORT_7_SEGMENT;
/* This function shows value of count on display the decimal point is displayed if dp=1
Note:
count must be less than 10 for decimal, or less than 16 for Hex. */
   if(count <dec_hex)
   {
      switch (count)
      {
         case 0:
         PORT_7_SEGMENT=0x80;
         break;

         case 1:
         PORT_7_SEGMENT=0x79;
         break;

         case 2:
         PORT_7_SEGMENT=0x24;
         break;

         case 3:
         PORT_7_SEGMENT=0x30;
         break;

         case 4:
         PORT_7_SEGMENT=0x19;
         break;

         case 5:
         PORT_7_SEGMENT=0x12;
         break;

         case 6:
         PORT_7_SEGMENT=0x02;
         break;

         case 7:
         PORT_7_SEGMENT=0x78;
         break;

         case 8:
         PORT_7_SEGMENT=0x1;
         break;

         case 9:
         PORT_7_SEGMENT=0x10;
         break;
        
      }
			
		}
     

   return PORT_7_SEGMENT;
}

