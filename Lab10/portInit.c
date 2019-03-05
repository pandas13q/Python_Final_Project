//Initializes ports

#include "tm4c123gh6pm.h"

void portDInit(){
	SYSCTL_RCGC2_R |= 0x08;
	volatile int foo = 123;
	GPIO_PORTD_AFSEL_R &= ~0x07;
	GPIO_PORTD_AMSEL_R &= ~0x07;
	GPIO_PORTD_PCTL_R = 0;
	GPIO_PORTD_DEN_R |= 0x07;
	GPIO_PORTD_DR8R_R = 0xFF;
	GPIO_PORTD_DIR_R &= ~0x07;
	GPIO_PORTD_PDR_R |= 0x07;
}

void portBInit(){
	SYSCTL_RCGC2_R |= 0x02;
	volatile int foo = 123;
	GPIO_PORTB_AFSEL_R &= ~0x3F;
	GPIO_PORTB_AMSEL_R &= ~0x3F;
	GPIO_PORTB_PCTL_R = 0;
	GPIO_PORTB_DEN_R |= 0x3F;
	GPIO_PORTB_DIR_R |= 0x3F;
}
