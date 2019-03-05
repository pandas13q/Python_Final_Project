// SpaceInvaders.c
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the EE319K Lab 10

// Last Modified: 3/6/2015 
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014

   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2014

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PE2/AIN1
// Slide pot pin 3 connected to +3.3V 
// DAC implemented on PA2 - PA7
// Input switches on PD0 - PD2
// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "Random.h"
#include "TExaS.h"
#include "ADC.h"
#include "DAC.h"
#include "Tileset.h"
#include "levels.h"
#include "Globals.h"
#include "portInit.h"
#include "Conversions.h"
#include "Timers.h"
#include "movement.h"
#include "buttons.h"
#include "sound.h"
#include "AlienAI.h"

int8_t currentLevel = 0;
int deaths = 0;
int8_t holding = 0;
uint8_t count = 0;
static int dead = 0;
int status = 0;
int buttonCount = 0;

entity entities[maxEntities]; // The game can display up to 'maxEntities' (defined in globals.h) entities at a time
#define player entities[0]		// The player character uses entity slot 0

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds
void newLevel(void);
void boxDel(void);
void boxNew(void);

int detect(){
	int returnVal = 0;
	int playerXTile[2];
	int playerYTile[2];
	int playerXPixel = player.xPos / 1000;
	playerXTile[0] = playerXPixel / 8;
	playerXTile[1] = playerXTile[0];
	int playerXOffset = playerXPixel % 8;
	int playerYPixel = player.yPos/ 1000;
	playerYTile[0] = playerYPixel / 8;
	playerYTile[1] = playerYTile[0];
	int playerYOffset = playerYPixel % 8;
	if (playerXOffset != 0){
		playerXTile[1]++;
	}
	if (playerYOffset != 0){
		playerYTile[1]++;
	}
	for(int iii = 1; iii < maxEntities; iii++) {
		int XTile[2];
		int YTile[2];
		int XPixel = entities[iii].xPos / 1000;
		XTile[0] = XPixel / 8;
		XTile[1] = XTile[0];
		int XOffset = XPixel % 8;
		int YPixel = entities[iii].yPos/ 1000;
		YTile[0] = YPixel / 8;
		YTile[1] = YTile[0];
		int YOffset = YPixel % 8;
		if (XOffset != 0){
			XTile[1]++;
		}
		if (YOffset != 0){	
			YTile[1]++;
		}
		if (((playerXTile[0] == XTile[0]) | (playerXTile[0] == XTile[1]) | (playerXTile[1] == XTile[0]) | (playerXTile[1] == XTile[1])) & ((playerYTile[0] == YTile[0]) | (playerYTile[0] == YTile[1]) | (playerYTile[1] == YTile[0]) | (playerYTile[1] == YTile[1]))) {
			if (entities[iii].ai > 2) {
				return 1;
			}
			if (entities[iii].ai == 2) {
				returnVal = 2;
			}
		}
	}
	return returnVal;
}

void SysTick_Handler() {
soundTick();
}

void Timer0A_Handler(void){
	TIMER0_ICR_R = TIMER_ICR_TATOCINT;
	if (count != 0) {
		count--;
		return;
	}
	count = 24;
  for (int ent = 1; ent < maxEntities; ent ++) {
		int *cPt = posUpdate(entities, ent, deaths, currentLevel);
		entities[ent].xPos = cPt[0];
		entities[ent].yPos = cPt[1];
		entities[ent].xVel = cPt[2];
		entities[ent].yVel = cPt[3];
		if ((cPt[4] != 0) & (entities[ent].ai !=2)){
			entities[ent].ai = 0;
			int *cPt = entityConvert(entities[ent].xPos, entities[ent].yPos);
			ST7735_DrawBitmap(cPt[0], cPt[1], specialgrey, 8, 8);
		}
	}
	int *cPt = posUpdate(entities, 0, deaths, currentLevel);
	player.xPos = cPt[0];
	player.yPos = cPt[1];
	player.xVel = cPt[2];
	player.yVel = cPt[3];
	status = detect();
	if (cPt[4] != 0){
		deaths++;
		dead = 1;
	}
	buttonTest(entities, currentLevel);
	int *cPts = entityConvert(player.xPos, player.yPos);
	if (holding == 1)
		ST7735_DrawBitmap((cPts[0] + 5), (cPts[1] - 3), specialdotblue, 1, 1);
	if (status == 1){
		deaths++;
		dead = 1;
	}
	if ((status == 2) & (holding == 0)){
		ST7735_DrawBitmap((cPts[0] + 5), (cPts[1] - 3), specialdotgreen, 1, 1);
	}	
			if (((GPIO_PORTD_DATA_R & 0x04) >> 2) & (buttonCount==0)) {
			buttonCount = 100;
			if ((status == 2) & (holding == 0)){
				holding = 1;
				boxDel();
			}
			else if (holding == 1) {
				holding = 0;
				boxNew();
			}
		}
		else if (~(GPIO_PORTD_DATA_R & 0x04) & (buttonCount > 0)){
			buttonCount--;
		}
}

void Timer1A_Handler(void){
	TIMER1_ICR_R = TIMER_ICR_TATOCINT;
	sound1();
}
void Timer2A_Handler(void){
	TIMER2_ICR_R = TIMER_ICR_TATOCINT;
	sound2();
}

void Timer3A_Handler(void){
	TIMER3_ICR_R = TIMER_ICR_TATOCINT;
	sound3();
}

// Draw a new level to the screen
void newLevel(){
	holding = 0;
	for (int ent = 0; ent < maxEntities; ent++) {
		entities[ent].ai = 0;
		entities[ent].xVel = 0;
		entities[ent].yVel = 0;
	}
	player.ai = 1;
	int entPt = 1;
	ST7735_FillScreen(0x8410); // set screen to grey
	for(int8_t outy = 0; outy < 16; outy++) {
		for (int8_t outx = 0; outx < 20; outx++) {
			int *cPt = coordConvert(outx, outy);
			int type = levelData[currentLevel][outy][outx];
			if (type == 1){
				ST7735_DrawBitmap(cPt[0], cPt[1], objwall, 8, 8);
			}
			if (type == 2){
				ST7735_DrawBitmap(cPt[0], cPt[1], objmagnet, 8, 8);
			}
			if (type == 3){
				ST7735_DrawBitmap(cPt[0], cPt[1], objspike0, 8, 8);
			}
			if (type == 4){
				ST7735_DrawBitmap(cPt[0], cPt[1], objspike90, 8, 8);
			}
			if (type == 5){
				ST7735_DrawBitmap(cPt[0], cPt[1], objspike180, 8, 8);
			}
			if (type == 6){
				ST7735_DrawBitmap(cPt[0], cPt[1], objspike270, 8, 8);
			}
			if (type == 7){
				ST7735_DrawBitmap(cPt[0], cPt[1], objzappersmall0, 8, 8);
			}
			if (type == 8){
				ST7735_DrawBitmap(cPt[0], cPt[1], objzappersmall90, 8, 8);
			}
			if (type == 9){
				ST7735_DrawBitmap(cPt[0], cPt[1], objbuttonoff0, 8, 8);
			}
			if (type == 10){
				ST7735_DrawBitmap(cPt[0], cPt[1], objbuttonoff180, 8, 8);
			}
			if (type == 101){
				ST7735_DrawBitmap(cPt[0], cPt[1], spritebox0, 8, 8);
				entities[entPt].xPos = outx * 8000;
				entities[entPt].yPos = outy * 8000;
				entities[entPt].ai = 2;
				entPt++;
			}
			if (type == 102){
				ST7735_DrawBitmap(cPt[0], cPt[1], spritebox180, 8, 8);
				entities[entPt].xPos = outx * 8000;
				entities[entPt].yPos = outy * 8000;
				entities[entPt].ai = 2;
				entPt++;
			}
			if (type == 103){
				ST7735_DrawBitmap(cPt[0], cPt[1], objbuttonoff0, 8, 8);
			}
			if (type == 104){
				ST7735_DrawBitmap(cPt[0], cPt[1], objbuttonoff180, 8, 8);
			}
			if (type == 205){
				ST7735_DrawBitmap(cPt[0], cPt[1], objzappersmalloff0, 8, 8);
			}
			if (type == 206){
				ST7735_DrawBitmap(cPt[0], cPt[1], objzappersmalloff90, 8, 8);
			}
			if (type == 202){
				ST7735_DrawBitmap(cPt[0], cPt[1], spriteastror0, 8, 8);
				player.xPos = outx * 8000;
				player.yPos = outy * 8000;
			}
			if (type == 109) {
				ST7735_DrawBitmap(cPt[0], cPt[1], spritealien0, 8, 8);
				entities[entPt].xPos = outx * 8000;
				entities[entPt].yPos = outy * 8000;
				entities[entPt].ai = 3;
				entities[entPt].xVel = 1200;
				entPt++;
			}
			if (type == 110) {
				ST7735_DrawBitmap(cPt[0], cPt[1], spritealien180, 8, 8);
				entities[entPt].xPos = outx * 8000;
				entities[entPt].yPos = outy * 8000;
				entities[entPt].ai = 3;
				entities[entPt].xVel = 1200;
				entPt++;
			}
			if (type == 107){
				ST7735_DrawBitmap(cPt[0], cPt[1], spritemonster0, 8, 8);
				entities[entPt].xPos = outx * 8000;
				entities[entPt].yPos = outy * 8000;
				entities[entPt].ai = 101;
				entities[entPt].xVel = 2000;
				entPt++;
			}
		}
	}
	ST7735_SetCursor(0,0);
	ST7735_OutString("Deaths:");
	ST7735_SetCursor(7,0);
	if (deaths < 10000){
		LCD_OutDec(deaths);
	}
	else{
		ST7735_OutString("Lots");
	}
}

int main(void){
	DisableInterrupts();
  TExaS_Init();  // set system clock to 80 MHz
	portDInit();
	portBInit();
	timerInit();
	ADC_Init();
	buttonInit();
	setSysTick(0x16E360);
	ST7735_InitR(INITR_REDTAB);
	player.ai = 1;
	ST7735_DrawBitmap(0,155,intro,128,160);
	while ((GPIO_PORTD_DATA_R & 0xFF) == 0){};
	newLevel();
	EnableInterrupts();
  while(1){
		if (player.xPos >= 152000){
			currentLevel++;
			if (currentLevel < levels){
				DisableInterrupts();
				newLevel();
				EnableInterrupts();
			}
			else {
				TIMER0_CTL_R = 0;
				TIMER1_CTL_R = 0;
				TIMER2_CTL_R = 0;
				TIMER3_TAILR_R = noteConvert(-36);
				TIMER3_CTL_R = 1;
				ST7735_FillScreen(0x0000);
				ST7735_SetCursor(0,0);
				ST7735_OutString("[SIGNAL LOST]");
				while(1){};
			}
		}
		if (dead == 1){
			DisableInterrupts();
			newLevel();
			dead = 0;
			EnableInterrupts();
		}
  }
}

void boxDel(){
	int delStatus = 0;
	for (int ent = 1; delStatus == 0; ent++){
		if (entities[ent].ai == 2){
			int32_t xDiff = entities[ent].xPos - player.xPos;
			int32_t yDiff = entities[ent].yPos - player.yPos;
			if ((xDiff < 8000) & (xDiff > -8000) & (yDiff < 8000) & (yDiff > -8000)){
				entities[ent].xVel = 0;
				entities[ent].yVel = 0;
				entities[ent].ai = 0;
				int *cPt = entityConvert(entities[ent].xPos, entities[ent].yPos);
				ST7735_DrawBitmap(cPt[0], cPt[1], specialgrey, 8, 8);
				delStatus = 1;
			}
			if (ent>maxEntities){
				while(1){
			ST7735_FillScreen(0xFFFF);}
			}
		}
	}
}

void boxNew(){
	int ent = 1;
	for (; entities[ent].ai != 0; ent++){};
	entities[ent].ai = 2;
	entities[ent].xPos = player.xPos;
	entities[ent].yPos = player.yPos;
	entities[ent].xVel = player.xVel;
	entities[ent].yVel = player.yVel;
}
