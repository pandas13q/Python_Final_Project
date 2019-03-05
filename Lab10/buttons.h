// Functions for buttons.c
// See the relevant functions in buttons.c for more detail
#ifndef _buttonsH_
#define _buttonsH_

#include "Globals.h"

void buttonInit(void); // Called at startup; initializes button array

void buttonTest(entity entities[], int currentLevel); // Updates the status of all buttons and associated objects
	
#endif
