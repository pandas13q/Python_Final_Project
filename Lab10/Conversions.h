// Provides functions that allow for the conversion of raw data for internal use, or vice versa.
#ifndef _ConversionsH_
#define _ConversionsH_

int* coordConvert(int xpos, int ypos); // Converts internal coordinates to those used to display to the screen

int* entityConvert(int xpos, int ypos); // Converts entity coordinates to those used to display to the screen

int gravConvert(int rawADC); // Converts data from the DAC/slider into a biased gravity value, ranging from -1 to 1.

#endif
