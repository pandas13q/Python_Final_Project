#include <stdint.h>

const int8_t maxEntities = 10; // Maximum non-player entities that can be in one level
// This number can be increased if needed, but this is likely to result in a significant performance impact.

const int grav = 1; // Maximum value for gravity = grav * 1000
const int termVelocity = 7000; // Terminal velocity, roughly equivalent to 14 tiles per second.
// Establishing a terminal velocity of less than 8 pixels / update is necessary to prevent bullet-through-paper problems.
