// Converts internal X and Y tile coordinates (measured as 8x8 pixel spaces) into those output to the screen
// This is required since the game is in a landscape orientation, while the ST7735 assumes a portrait orientation
int* coordConvert(int xpos, int ypos) {
	static int r[2];
	r[0] = 8 * ypos;
	r[1] = 160 - (8 * xpos);
	return r;
}

// Converts entity coordinates (which are recorded in thousandths of a pixel) into those output to the screen
int* entityConvert(int xpos, int ypos) {
	static int r[2];
	r[0] = ypos / 1000;
	r[1] = 160 - (xpos / 1000);
	return r;
}

// Converts raw ADC data into a gravity value from -1 to +1, with a wide band of 0 gravity
// ADC data ranges from 0 to 4095
int gravConvert(int rawADC){
	rawADC = 4095 - rawADC;
	if (rawADC > 1800 & rawADC < 2300) {
		return 0;
	}
	int convADC = rawADC * 0.2882;
	convADC -= 500;
	return convADC;
}
