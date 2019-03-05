// Contains code relevant to the movement of entities
// Movement functions are generally executed as part of the ISR for Timer 0

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "AlienAI.h"
#include "globals.h"
#include "conversions.h"
#include "ADC.h"
#include "levels.h"
#include "Tileset.h"
#include "ST7735.h"
#include "AlienAI.h"


// Track if an entity is 'dead' - dead entities are removed unless they are considered indestructible.
// If the player character is 'dead', the level restarts.
int dead;


// Function prototypes
int groundCheck(entity entities[], int ent, int gravity, int currentLevel, int useCurrentPos);
int playerMove(void);
int wallCheck(entity entities[], int ent, int currentLevel, int useCurrentPos);
int magCheck(entity entities[], int ent, int currentLevel);
void entDisplay(entity entities[], int ent, int gravity, int magStatus);

// Main function to calculate new position for an entity, based on movement and collision with objects
int* posUpdate(entity entities[], int ent, int deaths, int currentLevel) {
	dead = 0;
	int16_t xSend; // Define variables that will be used for the manipulation and return of the velocity vectors
	int16_t ySend;
	uint8_t xSet = 0;
	uint8_t ySet = 0;
	if (entities[ent].ai != 0){ 							// An AI value of 0 refers to an empty entity slot
		int gravity = gravConvert(ADC_In()); 		// Read data from the potentiometer and convert it into a gravity value from -1 to +1
		int *cPt = entityConvert(entities[ent].xPos, entities[ent].yPos);
		ST7735_DrawBitmap(cPt[0], cPt[1], specialgrey, 8, 8);	// Draw a grey square (to clear) the current position of an entity
		unsigned int groundStatus = groundCheck(entities, ent, gravity, currentLevel, 0); // Check to see if an entity is touching the "ground"
		unsigned int magStatus = magCheck(entities, ent, currentLevel);							// Check to see if an entity is touching the top or bottom of a magnet
		if (entities[ent].ai < 100){	// Non-flying entities that are not touching the ground or a magnet can not control their movement
			if ((groundStatus != 0) | (magStatus != 0)){
				if (ent == 0) {
					entities[ent].xVel = playerMove();
				}
				else{
					entities[ent].xVel = AIMoveX(entities, ent, currentLevel);
				}
			}
			if (groundStatus == 0){
				entities[ent].yVel += gravConst * gravity; // An entity not touching the ground accelerates due to gravity
			}
		}
		if (entities[ent].ai > 100) { // AIs with values over 100 define flying entities, which ignore gravity
				entities[ent].xVel = AIMoveX(entities, ent, currentLevel);
				entities[ent].yVel = AIMoveY(entities, ent, currentLevel);
		}
		if (magStatus != 0) {	// Entities that are touching the top or bottom of a magnet can not move vertically
			entities[ent].yVel = 0;
		}
		if (entities[ent].yVel > termVelocity) {	// Make sure that an entity is not travelling at an unsafe speed
			entities[ent].yVel = termVelocity;			// This makes physics calculations more stable, and also serves as a balancing mechanic
		}
		if (entities[ent].yVel < (0 - termVelocity)) {
			entities[ent].yVel = 0 - termVelocity;
		}
		//if ((entities[ent].xVel > -250) & (entities[ent].xVel < 250)){ // Very slow velocities are set to 0
		//	entities[ent].xVel = 0;																			 // This makes maneuvering slightly easier for the player
		//}
		//if ((entities[ent].yVel > -250) & (entities[ent].yVel < 250)){
		//	entities[ent].yVel = 0;
		//}
		xSend = entities[ent].xVel;	// Possible output value for X velocity, assuming no collision occurs
		ySend = entities[ent].yVel; // Possible output value for Y velocity, assuming no collision occurs
		int16_t xTemp = entities[ent].xVel;	// Define "temporary" velocities for the purposes of collision detection
		int16_t yTemp = entities[ent].yVel;
		if (entities[ent].xVel > 1000){
			entities[ent].xVel = 1000;
		}
		if (entities[ent].xVel < -1000) {
			entities[ent].xVel = -1000;
		}
		if (entities[ent].yVel > 1000){
			entities[ent].yVel = 1000;
		}
		if (entities[ent].yVel < -1000) {
			entities[ent].yVel = -1000;
		}
		while ((xTemp != 0) | (yTemp != 0)){	// Collision detection
			if (xTemp >= 1000){
				xTemp -= 1000;
			}
			else if (xTemp <= -1000){
				xTemp += 1000;
			}
			else{
				entities[ent].xVel = xTemp;
				xTemp = 0;
			}
			int xStat = wallCheck(entities, ent, currentLevel, 1);
			if (xStat == 0){
				entities[ent].xPos += entities[ent].xVel;
			}
			else if (xTemp == 0){
				xSet = 1;	// If a movement ends in collision, then the relevant velocity vector needs to be set to 0
			}
			if (yTemp >= 1000){
				yTemp -= 1000;
			}
			else if (yTemp <= -1000){
				yTemp += 1000;
			}
			else{
				entities[ent].yVel = yTemp;
				yTemp = 0;
			}
			int yStat = groundCheck(entities, ent, gravity, currentLevel, 1);
			if (yStat == 0){
				entities[ent].yPos += entities[ent].yVel;
			}
			else if (yTemp == 0) {
				ySet = 1; // If a movement ends in collision, then the relevant velocity vector needs to be set to 0
			}
		}
		entities[ent].xVel = xSend; // Restore the velocity vectors for the purposes of calculating the rotation and directon of an entity
		entities[ent].yVel = ySend; // See entDisplay
		if (xSet == 1) {	// Set velocity vectors to 0 if movements resulted in collision
			xSend = 0;
		}
		if (ySet == 1) {
			ySend = 0;
		}
		entDisplay(entities, ent, gravity, magStatus);
	}
	static int r[5];
	r[0] = entities[ent].xPos;
	r[1] = entities[ent].yPos;
	r[2] = xSend;
	r[3] = ySend;
	r[4] = dead;
	return r;
}

// If useCurrentPos = 0, test to see if an entity is standing on a valid ground tile, and use gravity to find movDir
// If useCurrentPos = 1, detect collisions, testing the current position offset by the Y velocity vector
// Returns a 16 bit integer, or 0 if no tile is detected
// Bits 0-7 refer to the type of the detected tile 
// Bits 8-15 refer to the type of tile immediately to its right, if the entity is horizontally offset from the tile grid
int groundCheck(entity entities[], int ent, int gravity, int currentLevel, int useCurrentPos){
	int detect = 0;
	int movDir; // Direction of movement - based off of gravity or the Y velocity vector, depending on useCurrentPos
	if ((gravity == 0) & (useCurrentPos == 0)) {
		return 0;
	}
	if (gravity < 0) {
		movDir = -1;
	}
	if (gravity > 0) {
		movDir = 1;
	}
	if (useCurrentPos == 1){
		movDir = 0;
	}
	int xPixel = entities[ent].xPos / 1000;	// Find an entity's position on the pixel and tile grids
	int xTile = xPixel / 8;
	int xOffset = xPixel % 8;	// Find the entity's offset from the tile grid
	int yPixel = (entities[ent].yPos + (useCurrentPos * entities[ent].yVel)) / 1000;
	int yTile = (yPixel) / 8;
	if ((entities[ent].yVel > 0) & (useCurrentPos = 1)){ // For collision detection
		yTile = (yPixel + 7) / 8;
	}
	int yOffset = yPixel % 8;
	if ((yOffset != 0) & (useCurrentPos == 0)) {
		return 0;
	}
	int touchTile = levelData[currentLevel][yTile + movDir][xTile]; // Check relevant tile
	if ((touchTile > 0) & (touchTile < 100)) {
		if ((touchTile >= 3) & (touchTile <= 8)) {
			dead = 1;
		}
		detect = 1;
	}
	if (xOffset != 0) {	// Check adjacent tile to the right if there is an X offset
		int touchTileOff = levelData[currentLevel][yTile + movDir][xTile + 1];
		if ((touchTileOff > 0) & (touchTileOff < 100)) {
			if ((touchTileOff >= 3) & (touchTileOff <= 8)) {
				dead = 1;
			}
			detect = 1;
		}
	}
	if (detect == 1) {
		return(levelData[currentLevel][yTile + movDir][xTile] + (levelData[currentLevel][yTile + movDir][xTile + 1] << 8));
	}
	return 0;
}

// Accept input from PD0-1 and return an X velocity
// If both buttons are pressed, the velocity is 0
int playerMove(){
	int move = 0;
	if ((GPIO_PORTD_DATA_R & 0x01) != 0) {
		move -= 1000;
	}
	if ((GPIO_PORTD_DATA_R & 0x02) != 0) {
		move += 1000;
	}
	return move;
}

// If useCurrentPos = 0, test for adjacent walls, using the X velocity vector for direction - Currently unused functionality
// If useCurrentPos = 1, detect collisions, testing the current position offset by the X velocity vector
// Returns a 16 bit integer, or 0 if no tile is detected
// Bits 0-7 refer to the type of the detected tile 
// Bits 8-15 refer to the type of tile immediately below it, if the entity is vertically offset from the tile grid
int wallCheck(entity entities[], int ent, int currentLevel, int useCurrentPos){
	int detect;
	int movDir;
	if (entities[ent].xVel == 0) {
		return 0;
	}
	if (entities[ent].xVel > 0) {
		movDir = 1;
	}
	if (entities[ent].xVel < 0) {
		movDir = -1;
	}
	if (useCurrentPos == 1){
		movDir = 0;
	}
	int xPixel = (entities[ent].xPos + (useCurrentPos * entities[ent].xVel)) / 1000; // Find an entity's position on the pixel and tile grids
	int xTile = xPixel / 8;
	if ((entities[ent].xVel > 0) & (useCurrentPos == 1)){ // For collision detection
		xTile = (xPixel + 7) / 8;
	}
	int xOffset = xPixel % 8; // Find the entity's offset from the tile grid
	int yPixel = entities[ent].yPos / 1000;
	int yTile = yPixel / 8;
	int yOffset = yPixel % 8;
	if ((xOffset != 0) & (useCurrentPos == 0)) {
		return 0;
	}
	int touchTile = levelData[currentLevel][yTile][xTile + movDir]; // Check relevant tile
	if ((touchTile > 0) & (touchTile < 100)) {
		if ((touchTile >= 3) & (touchTile <= 8)) {
			dead = 1;
		}
		detect = 1;
	}
	if (yOffset != 0) { // Check adjacent tile below if there is a Y offset
		int touchTileOff = levelData[currentLevel][yTile + 1][xTile + movDir];
		if ((touchTileOff > 0) & (touchTileOff < 100)) {
			if ((touchTileOff >= 3) & (touchTileOff <= 8)) {
				dead = 1;
			}
			detect = 1;
		}
	}
	if (detect == 1) {
		return(levelData[currentLevel][yTile][xTile + movDir] + (levelData[currentLevel][yTile + 1][xTile + movDir] << 8));
	}
	return 0;
}

// Check for magnets immediately above or below an entity
// Return 0 for no magnets, 1 for a magnet below, 2 for a magnet above, 3 for magnets below and above
int magCheck(entity entities[], int ent, int currentLevel){
	int magStatus = 0;	// Calculate positions on the pixel and tile grids and offset from the tile grid
	int xPixel = entities[ent].xPos / 1000;
	int xTile = xPixel / 8;
	int xOffset = xPixel % 8;
	int yPixel = entities[ent].yPos / 1000;
	int yTile = yPixel / 8;
	int yOffset = yPixel % 8;
	if(entities[ent].ai==101){
		return 0;
	}
	if (yOffset != 0) {
		return 0;
	}
	if (levelData[currentLevel][yTile + 1][xTile] == 2){
		magStatus |= 0x01;
	}
	if (levelData[currentLevel][yTile - 1][xTile] == 2){
		magStatus |= 0x02;
	}
	if (xOffset != 0) {
		if (levelData[currentLevel][yTile + 1][xTile + 1] == 2){
			magStatus |= 0x01;
		}
		if (levelData[currentLevel][yTile - 1][xTile + 1] == 2){
			magStatus |= 0x02;
		}
	}
	return magStatus;
}

// Draw an entity to the LCD in its new position
void entDisplay(entity entities[], int ent, int gravity, int magStatus){
	int *cPt = entityConvert(entities[ent].xPos, entities[ent].yPos);
	if (entities[ent].ai == 1){
		if ((entities[ent].xVel < 0) & ((gravity >= 0) | (magStatus & 0x1))){
			ST7735_DrawBitmap(cPt[0], cPt[1], spriteastrol0, 8, 8);
		}
		else if ((entities[ent].xVel >= 0) & ((gravity < 0) | (magStatus == 0x2))){
			ST7735_DrawBitmap(cPt[0], cPt[1], spriteastror180, 8, 8);
		}
		else if ((entities[ent].xVel < 0) & ((gravity < 0) | (magStatus == 0x2))){
			ST7735_DrawBitmap(cPt[0], cPt[1], spriteastrol180, 8, 8);
		}
		else{
			ST7735_DrawBitmap(cPt[0], cPt[1], spriteastror0, 8, 8);
		}
	}
	if (entities[ent].ai == 2){
		if ((gravity < 0) | (magStatus == 0x2)) {
			ST7735_DrawBitmap(cPt[0], cPt[1], spritebox180, 8, 8);
		}
		else{
			ST7735_DrawBitmap(cPt[0], cPt[1], spritebox0, 8, 8);
		}
	}
	if (entities[ent].ai == 3){
		
		if ((gravity < 0) | (magStatus == 0x2)) {
			ST7735_DrawBitmap(cPt[0], cPt[1], spritealien180, 8, 8);
		}
		else{
			ST7735_DrawBitmap(cPt[0], cPt[1], spritealien0, 8, 8);
		}
	}
	if (entities[ent].ai == 101){
		ST7735_DrawBitmap(cPt[0], cPt[1], spritemonster0, 8, 8);
	}
}
