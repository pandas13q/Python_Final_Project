//Contains definitions and constants for game

#ifndef _GlobalsH_
#define _GlobalsH_
#include <stdint.h>

// Template for entity
// Tracks position and velocity in X and Y directions
// AI influences entity movement. 1 to disable. 0 if no entity. AI behaviors are defined in AlienAI.c

struct entitystr{ // Defines the position and behavior of an entity
	uint32_t xPos;
	uint32_t yPos;
	int16_t xVel; // Velocities are divided by 1000 to get displacement in pixels
	int16_t yVel; // This allows for fine manipulation, especially when dealing with fractional gravity
	int8_t ai;
};

typedef struct entitystr entity;

#define maxEntities 7 // Maximum non-player entities that can be in one level
// This number can be increased if needed, but this is likely to result in a significant performance impact.

#define gravConst 1 // Maximum value for gravity = gravConst * 500
#define termVelocity 4000 // Terminal velocity
// Establishing a terminal velocity of less than 8 pixels / update is necessary to prevent bullet-through-paper problems.

struct buttonstr{ // Defines the position and behavior of a button
	uint8_t xTile;
	uint8_t yTile;
	uint8_t xTrigger;
	uint8_t yTrigger;
	uint16_t rotation;
	uint8_t inverted;
};

typedef struct buttonstr button;

// Number of levels in the game
#define levels 7

#define maxButtons 7 // Maximum number of buttons that can be included in one level
// This number should be able to be increased with a moderate performance impact

#endif
