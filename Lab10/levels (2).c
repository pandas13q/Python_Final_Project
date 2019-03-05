#include <stdint.h>
#include "levels.h"

// Levels for the game are defined in this array
// 'astror0' denotes the player's starting position

uint8_t levelData[levels][16][20] = {{
//Level 1
<<<<<<< .mine
//Solution: Sandbox mode
	{wall, buttonoff180, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall},
||||||| .r26336
	{wall, buttonoff180, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall},
=======
	{wall, buttonoff180, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, magnet, magnet, magnet, wall, wall},
>>>>>>> .r26421
	{wall, 0, 0, zappersmall0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, wall},
	{wall, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, box0, 0, wall},
	{wall, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, wall},
	{wall, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, wall},
	{wall, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, wall},
	{wall, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, wall},
	{wall, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, wall},
	{wall, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, wall},
	{wall, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, wall},
	{wall, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, wall},
	{wall, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, wall},
	{wall, 0, wall, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, wall, 0, wall},
	{wall, 0, wall, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, wall, 0, wall},
	{wall, astror0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall}},
//Level 2
//Solution: Grab box in top left, place on button in middle left, deactivate zapper, exit at bottom right
	{{wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall},
	{wall, 0, 0, 0, spike0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, spike90, wall},
	{wall, box0, 0, 0, 0, 0, 0, spike180, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, spike90, wall},
	{wall, wall, wall, wall, wall, wall, wall, wall, wall, 0, 0, 0, 0, 0, 0, 0, 0, 0, spike90, wall},
	{wall, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, spike90, wall},
	{wall, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, spike90, wall},
	{wall, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, spike90, wall},
	{wall, 0, 0, 0, 0, 0, 0, 0, 0, 0, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall},
	{wall, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, spike0, 0, 0, 0, 0, 0, 0, wall},
	{wall, buttonoff0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, wall},
	{wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, 0, 0, 0, 0, wall},
	{wall, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, wall},
	{wall, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, wall},
	{wall, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, wall, wall, wall, wall, wall, wall, wall, wall},
	{wall, astror0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,/*zappersmall0,*/ 0, 0, 0, 0, 0, 0, 0},
	{wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall, wall}}};
