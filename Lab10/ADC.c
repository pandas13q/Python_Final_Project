// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0
// Last Modified: 3/6/2015 
// Student names: Summer Gregurek and Zachary Susskind
// Last modification date: 4/13/16

#include <stdint.h>
#include "tm4c123gh6pm.h"

// ADC initialization function 
// Input: none
// Output: none
void ADC_Init(void){ 
	SYSCTL_RCGC2_R |= 0x10;
	volatile int foo = 123;
	GPIO_PORTE_DIR_R &= ~0x04;
	GPIO_PORTE_AFSEL_R |= 0x04;
	GPIO_PORTE_DEN_R &= ~0x04;
	GPIO_PORTE_AMSEL_R |= 0x04;
	SYSCTL_RCGCADC_R |= 0x01;
	int delay = 100;
	while(delay > 0){
		delay--;
	}
	ADC0_PC_R = 0x01;
	delay = 100;
	while(delay > 0){
		delay--;
	}
	volatile int biz = 789;
	ADC0_SSPRI_R = 0x0123;
	ADC0_ACTSS_R &= ~0x08;
	ADC0_EMUX_R &= ~(0xF000);
	ADC0_SSMUX3_R = (ADC0_SSMUX3_R& 0xFFFFFFF0) + 1;
	ADC0_SSCTL3_R = 0x06;
	ADC0_IM_R &= ~0x08;
	ADC0_ACTSS_R |= 0x08;
}

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint32_t ADC_In(void){  
	ADC0_PSSI_R = 0x08;
	while((ADC0_RIS_R & 0x08) == 0){};
	int result = (ADC0_SSFIFO3_R & 0xFFF);
	ADC0_ISC_R = 0x08;
  return result;
}


