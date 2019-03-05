//Controls movement of two types of aliens

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "globals.h"
#include "conversions.h"
#include "ADC.h"
#include "levels.h"
#include "Tileset.h"
#include "ST7735.h"
#include "movement.h"

int dir[maxEntities] = {1,1,1,1,1,1,1};

short AIMoveX(entity entities[], int ent, int currentLevel){
	if (entities[ent].ai == 3) {
		if ((entities[ent].xVel < 0) & (entities[ent].xVel > -1200)){
			entities[ent].xVel = 1200;
			dir[ent] = 1;
		}
		if ((entities[ent].xVel > 0) & (entities[ent].xVel < 1200)){
			entities[ent].xVel = -1200;
			dir[ent] = -1;
		}
		if (entities[ent].xVel == 0){
			if (dir[ent] == 1){
				entities[ent].xVel = -1200;
				dir[ent] = -1;
			}
			else{
				entities[ent].xVel = 1200;
				dir[ent] = 1;
			}
		}
	}
	if (entities[ent].ai == 101) {
		if ((dir[ent] == 3) | (dir[ent] == 7) | (dir[ent] == 2) | (dir[ent] == 6)){
			return 0;
		}
		if (entities[ent].xVel == 0){
			if ((dir[ent] == 1) | (dir[ent] == 5)){
				dir[ent]++;
				entities[ent].xVel = 0;
			}
			if (dir[ent] == 4){
				dir[ent] = 5;
				entities[ent].xVel = -2000;
			}
			if (dir[ent] == 8){
				dir[ent] = 1;
				entities[ent].xVel = 2000;
			}
		}
	}
	return entities[ent].xVel;
}

short AIMoveY(entity entities[], int ent, int currentLevel){
	if ((dir[ent] == 1) | (dir[ent] == 5) | (dir[ent] == 8) | (dir[ent] == 4)){
		return 0;
	}
	if (entities[ent].yVel == 0){
		if ((dir[ent] == 3) | (dir[ent] == 7)){
			dir[ent]++;
			entities[ent].yVel = 0;
		}
		if (dir[ent] == 2){
			dir[ent] = 3;
			entities[ent].yVel = -2000;
		}
		if (dir[ent] == 6){
			dir[ent] = 7;
			entities[ent].yVel = 2000;
		}
	}
	return entities[ent].yVel;
}
//AI for a flying alien not affected by gravity
//AI number 101 (must be over 100 to ignore gravity)
