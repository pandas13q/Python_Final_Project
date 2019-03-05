// dac.c
// This software configures DAC output
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: 8/25/2014 
// Last Modified: 3/23/2015 
// Section Wednesday 10-11     TA: Ce
// Lab number: 6
// Hardware connections
// PE0-3: DAC output
// PA2-4: Piano input
// PF2: Heartbeat
// PA6: Systick interrupt output

#include <stdint.h>
#include "tm4c123gh6pm.h"
// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data

// **************DAC_Init*********************
// Initialize 4-bit DAC, called once  --- Do not call for Lab 10! Taken care of in Port A initialization!
// Input: none
// Output: none
void DAC_Init(void){
	SYSCTL_RCGC2_R |= 0x10;
	volatile int foo = 1234;
	GPIO_PORTE_AFSEL_R = 0;
	GPIO_PORTE_AMSEL_R = 0;
	GPIO_PORTE_PCTL_R = 0;
	GPIO_PORTE_DEN_R = 0xFF;
	GPIO_PORTE_DR8R_R = 0xFF;
	GPIO_PORTE_DIR_R = 0x0F;
}

// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(uint32_t data){
	GPIO_PORTE_DATA_R = data;
}
