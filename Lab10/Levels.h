#ifndef _LevelsH_
#define _LevelsH_
#include <stdint.h>
#include "Globals.h"

// Numeric definitions to make level arrays easier to read and create
// IDs 1-100 refer to objects (static, usually have collision)
// 101-200 refer to sprites and NPCs (static or mobile, no collision)
// 200+ refer to special (including sprites for the player character)
// 0 is used to denote an empty space

#define wall 1
#define magnet 2
#define spike0 3
#define spike90 4
#define spike180 5
#define spike270 6
#define zappersmall0 7
#define zappersmall90 8
#define buttonoff0 9
#define buttonoff180 10
#define buttonon0 11
#define buttonon180 12
#define box0 101
#define box180 102
#define monster0 107
#define alien0 109
#define alien180 110
#define astrol0 201
#define astror0 202
#define astrol180 203
#define astror180 204
#define zappersmalloff0 205
#define zappersmalloff90 206

// Define an array which contains level data
extern uint8_t levelData[levels][16][20];

#endif
