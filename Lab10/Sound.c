// Sound.c
// "It's not a bug, it's a feature."
// -Abraham Lincoln, 1923

#include <stdint.h>
#include <math.h>
#include "sound.h"
#include "tm4c123gh6pm.h"

const uint8_t sound1notes = 14;
const uint8_t sound2notes = 7;
const uint8_t sound3notes = 0;
uint8_t sound1curr = 0;
uint8_t sound2curr = 0;
uint8_t sound3curr = 0;
uint8_t soundfxcurr = 0;
uint8_t sound1pt = 0;
uint8_t sound2pt = 0;
uint8_t sound3pt = 0;
uint8_t soundfxpt = 0;
int16_t sound1count = 0;
int16_t sound2count = 0;
int16_t sound3count = 0;
int8_t soundfxcount = 0;
uint16_t sound1arrpt = 0;
uint16_t sound2arrpt = 0;
uint16_t sound3arrpt = 0;

void sound1Set(void);
void sound2Set(void);

uint8_t instruments[numInstr][32] = {
// Silence
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
// Pure sine wave
	{8,9,11,12,13,14,14,15,15,15,14,14,13,12,11,9,8,7,5,4,3,2,2,1,1,1,2,2,3,4,5,7},
// Sawtooth wave
	{7,7,8,8,9,9,10,10,11,11,12,12,13,13,14,14,15,15,0,0,1,1,2,2,3,3,4,4,5,5,6,6},
// Piano (Might sound awful? - Confirmed, sounds awful)
	{1,4,6,5,4,5,5,2,3,6,9,11,10,9,9,9,10,10,11,13,15,13,10,9,10,10,8,7,8,7,4,0},
// Noise
	{23, 28, 29, 11, 27, 29, 23, 18, 10, 10, 7, 13, 19, 5, 29, 30, 15, 14, 4, 25, 7, 27, 3, 7, 17, 0, 11, 25, 13, 4, 0, 14}
};

const int8_t sound1Notes[] = {-24,0,-24,0,-23,0,-23,0,-22,0,-22,0,-21,0};

const uint8_t sound1Inst[] = {1,0,1,0,1,0,1,0,1,0,1,0,1,0};

const uint16_t sound1Times[] = {19,1,19,11,9,1,19,1,19,1,19,11,29,1};

const int8_t sound2Notes[] = {0,-5,0,-9,0,-12,0};
	
const uint8_t sound2Inst[] = {0,1,0,1,0,1,0};
	
const uint16_t sound2Times[] = {320,22,8,9,21,19,1};
	
//const int8_t sound3Notes[] = {};
	
//const uint8_t sound3Inst[] = {};
	
//const uint16_t sound3Times[] = {};

// Convert a numeric note value (with A = 440 HZ, middle C = 261.62 Hz) into a timer TAILR_R value
// Formula: y = (2 ^ ((x + 105) / 12)) * 1.67
//For middle C, x = -9. For A440, x = 0
int noteConvert(int note){
	long double noteConv = (note + 105) / 12;
	noteConv = powl(2.00000, noteConv);
	long double freq = (noteConv * 1.67);
	freq *= (12.5 * 32);
	freq = (1000000000.000 / freq);
	return freq; //1 billion ns/s, 12.5 ns/cycle, 32 DAQ outputs per complete wave
}

void setSysTick(int period){
	NVIC_ST_CTRL_R = 0;
  NVIC_ST_RELOAD_R = period;
  NVIC_ST_CURRENT_R = 0;
	NVIC_ST_CTRL_R = 7;
}

void soundTick(){
	sound1count--;
	sound2count--;
	sound3count--;
	if (sound1count <= 0){
		sound1Set();
	}
	if (sound2count <= 0){
		sound2Set();
	}
}

void sound1(){
	GPIO_PORTB_DATA_R -= sound1curr;
	sound1curr = instruments[sound1Inst[sound1arrpt - 1]][sound1pt];
	sound1pt++;
	if (sound1pt == 32) {
		sound1pt = 0;
	}
	GPIO_PORTB_DATA_R += sound1curr;
}

void sound2(){
	GPIO_PORTB_DATA_R -= sound2curr;
	sound2curr = instruments[sound2Inst[sound2arrpt - 1]][sound2pt];
	sound2pt++;
	if (sound2pt == 32) {
		sound2pt = 0;
	}
	GPIO_PORTB_DATA_R += sound2curr;
}

void sound3(){
	GPIO_PORTB_DATA_R -= sound3curr;
	sound3curr = instruments[4][sound3pt];
	sound3pt++;
	if (sound3pt == 32) {
		sound3pt = 0;
	}
}

/*void sound3(){
	GPIO_PORTB_DATA_R -= sound3curr;
	sound3curr = instruments[sound3Inst[sound3arrpt - 1]][sound3pt];
	sound3pt++;
	if (sound3pt == 33) {
		sound3pt = 0;
	}
	GPIO_PORTB_DATA_R += sound3curr;
}*/


void sound1Set(){
	if (sound1arrpt == sound1notes) {
		sound1arrpt = 0;
	}
	GPIO_PORTB_DATA_R = sound2curr + sound3curr + soundfxcurr;
	sound1curr = 0;
	sound1count = sound1Times[sound1arrpt];
	sound1pt = 0;
	TIMER1_CTL_R = 0x00000000;
	TIMER1_TAILR_R = noteConvert(sound1Notes[sound1arrpt]);
  TIMER1_CTL_R = 0x00000001;
	sound1arrpt++;
}

void sound2Set(){
	if (sound2arrpt == sound2notes) {
		sound2arrpt = 0;
	}
	GPIO_PORTB_DATA_R = sound1curr + sound3curr + soundfxcurr;
	sound2curr = 0;
	sound2count = sound2Times[sound2arrpt];
	sound2pt = 0;
	TIMER2_CTL_R = 0x00000000;
	TIMER2_TAILR_R = noteConvert(sound2Notes[sound2arrpt]);
  TIMER2_CTL_R = 0x00000001;
	sound2arrpt++;
}
