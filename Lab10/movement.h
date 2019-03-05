// Functions used in movement.c
// See movement.c for more detailed breakdowns
#ifndef _MovementH_
#define _MovementH_

#include "Globals.h"

int* posUpdate(entity entities[], int ent, int deaths, int currentLevel);

int groundCheck(entity entities[], int ent, int gravity, int currentLevel, int useCurrentPos);

int playerMove(void);

int wallCheck(entity entities[], int ent, int currentLevel, int useCurrentPos);

int magCheck(entity entities[], int ent, int currentLevel);

void entDisplay(entity entities[], int ent, int gravity, int magStatus);

#endif
