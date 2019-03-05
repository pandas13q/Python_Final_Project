// Sound.h
#ifndef _SoundH_
#define _SoundH_

#include <stdint.h>

#define numInstr 5

#define sine 1
#define piano 2

extern uint8_t instruments[numInstr][32];

void setSysTick(int period);

void soundTick(void);

void sound1(void);

void sound2(void);

void sound3(void);

int noteConvert(int note);

#endif
