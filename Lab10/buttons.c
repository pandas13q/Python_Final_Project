// Defines position and behavior of buttons
#include <stdint.h>
#include "Globals.h"
#include "Conversions.h"
#include "Tileset.h"
#include "ST7735.h"
#include "levels.h"

button buttons[levels][maxButtons];

void buttonInit(){	
	buttons[1][0].xTile = 1;
	buttons[1][0].yTile = 9;
	buttons[1][0].xTrigger = 13;
	buttons[1][0].yTrigger = 14;
	buttons[1][0].rotation = 0;
	buttons[1][0].inverted = 1;
	
	buttons[2][0].xTile = 11;
	buttons[2][0].yTile = 4;
	buttons[2][0].xTrigger = 13;
	buttons[2][0].yTrigger = 13;
	buttons[2][0].rotation = 180;
	buttons[2][0].inverted = 1;
	
	buttons[3][0].xTile = 6;
	buttons[3][0].yTile = 1;
	buttons[3][0].xTrigger = 6;
	buttons[3][0].yTrigger = 13;
	buttons[3][0].rotation = 180;
	buttons[3][0].inverted = 1;
	
	buttons[3][1].xTile = 17;
	buttons[3][1].yTile = 6;
	buttons[3][1].xTrigger = 15;
	buttons[3][1].yTrigger = 12;
	buttons[3][1].rotation = 0;
	buttons[3][1].inverted = 1; 
	
	buttons[4][0].xTile = 6;
	buttons[4][0].yTile = 6;
	buttons[4][0].xTrigger = 13;
	buttons[4][0].yTrigger = 2;
	buttons[4][0].rotation = 0;
	buttons[4][0].inverted = 0;
	
	buttons[4][1].xTile = 6;
	buttons[4][1].yTile = 8;
	buttons[4][1].xTrigger = 15;
	buttons[4][1].yTrigger = 14;
	buttons[4][1].rotation = 180;
	buttons[4][1].inverted = 1;
	
	buttons[5][0].xTile = 3;
	buttons[5][0].yTile = 9;
	buttons[5][0].xTrigger = 1;
	buttons[5][0].yTrigger = 9;
	buttons[5][0].rotation = 180;
	buttons[5][0].inverted = 1;

	buttons[5][1].xTile = 12;
	buttons[5][1].yTile = 11;
	buttons[5][1].xTrigger = 14;
	buttons[5][1].yTrigger = 14;
	buttons[5][1].rotation = 0;
	buttons[5][1].inverted = 1;
	
	buttons[6][0].xTile = 1;
	buttons[6][0].yTile = 6;
	buttons[6][0].xTrigger = 15;
	buttons[6][0].yTrigger = 13;
	buttons[6][0].rotation = 0;
	buttons[6][0].inverted = 1;
	

}

int buttonTestEntity(entity entities[], int currentLevel, int curr, int dir);
void buttonChangeOutput(int currentLevel, int curr, int detect);

void buttonTest(entity entities[], int currentLevel){
	for(int curr = 0; curr < maxButtons; curr++) {
		if ((buttons[currentLevel][curr].xTile != 0) | (buttons[currentLevel][curr].yTile != 0)) {
			int detect;
			if (buttons[currentLevel][curr].rotation == 0) {
				detect = buttonTestEntity(entities, currentLevel, curr, -1);
			}
			else if (buttons[currentLevel][curr].rotation == 180) {
				detect = buttonTestEntity(entities, currentLevel, curr, 1);
			}
			buttonChangeOutput(currentLevel, curr, detect);
		}
	}
}

int buttonTestEntity(entity entities[], int currentLevel, int curr, int dir){
	for (int ent = 0; ent < maxEntities; ent++) {
		if (entities[ent].ai > 0) {
			int xPixel = entities[ent].xPos / 1000;
			int xTile = xPixel / 8;
			int xOffset = xPixel % 8;
			int yPixel = entities[ent].yPos / 1000;
			int yTile = (yPixel) / 8;
			int yOffset = yPixel % 8;
			if ((yOffset == 0) & (yTile == buttons[currentLevel][curr].yTile + dir)) {
				if ((xTile == buttons[currentLevel][curr].xTile) | ((xOffset != 0) & (xTile == buttons[currentLevel][curr].xTile + 1))) {
					return 1;
				}
			}
		}
	}
	return 0;
}

void buttonChangeOutput(int currentLevel, int curr, int detect){
	int* cPt = coordConvert(buttons[currentLevel][curr].xTile, buttons[currentLevel][curr].yTile);
	if (buttons[currentLevel][curr].rotation == 0) {
		if (detect == 0) {
			ST7735_DrawBitmap(cPt[0], cPt[1], objbuttonoff0, 8, 8);
		}
		else{
			ST7735_DrawBitmap(cPt[0], cPt[1], objbuttonon0, 8, 8);
		}
	}
	else if (buttons[currentLevel][curr].rotation == 180) {
		if (detect == 0) {
			ST7735_DrawBitmap(cPt[0], cPt[1], objbuttonoff180, 8, 8);
		}
		else{
			ST7735_DrawBitmap(cPt[0], cPt[1], objbuttonon180, 8, 8);
		}
	}
	int off = ((detect == 0) & (buttons[currentLevel][curr].inverted == 0)) | ((detect == 1) & (buttons[currentLevel][curr].inverted == 1));
	int* cPt2 = coordConvert(buttons[currentLevel][curr].xTrigger, buttons[currentLevel][curr].yTrigger);
	int type = levelData[currentLevel][buttons[currentLevel][curr].yTrigger][buttons[currentLevel][curr].xTrigger];
	if ((type == 7) | (type == 205)){
		if (off) {
			levelData[currentLevel][buttons[currentLevel][curr].yTrigger][buttons[currentLevel][curr].xTrigger] = 205;
			ST7735_DrawBitmap(cPt2[0], cPt2[1], objzappersmalloff0, 8, 8);
		}
		else{
			levelData[currentLevel][buttons[currentLevel][curr].yTrigger][buttons[currentLevel][curr].xTrigger] = 7;
			ST7735_DrawBitmap(cPt2[0], cPt2[1], objzappersmall0, 8, 8);
		}
	}
	if ((type == 8) | (type == 206)){
		if (off) {
			levelData[currentLevel][buttons[currentLevel][curr].yTrigger][buttons[currentLevel][curr].xTrigger] = 206;
			ST7735_DrawBitmap(cPt2[0], cPt2[1], objzappersmalloff90, 8, 8);
		}
		else{
			levelData[currentLevel][buttons[currentLevel][curr].yTrigger][buttons[currentLevel][curr].xTrigger] = 8;
			ST7735_DrawBitmap(cPt2[0], cPt2[1], objzappersmall90, 8, 8);
		}
	}
}
