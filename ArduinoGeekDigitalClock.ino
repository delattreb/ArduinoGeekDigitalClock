//
// Created by Bruno on 15/09/2017.
//

#include "var.h"
#include <Wire.h>
#include "libDS3231.h"
#include <Adafruit_NeoPixel.h>

// Var
Adafruit_NeoPixel pixels_hour = Adafruit_NeoPixel(NBPIXELS_HOUR, PIN_HOUR, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_minute = Adafruit_NeoPixel(NBPIXELS_MINUTE, PIN_MINUTE, NEO_GRB + NEO_KHZ800);

int delayval = 0;
int pix_hour, pix_minute, pix_seconde, pix_dixseconde, lastpix_dixseconde;
unsigned long startmillis;

libDS3231 rtc;
RtcDateTime now;
int color = 0, mode = 0;
boolean bfm_mode = false, bfm_color = false;

// Color
int colorTable[3][4][3] = { { { 0,0,255 }, { 0,0,255 }, { 220,0,255 }, { 100,0,0 } },
							{ { 255,45,0 }, { 255,45,0 }, { 255,200,0 }, { 0,100,0 } },
							{ { 255, 0, 0 }, { 255,0,0 }, { 255,45,0 }, { 0,0,100 } } };
uint32_t color_hour;
uint32_t color_minute;
uint32_t color_seconde;
uint32_t color_dixseconde;

//
// setup
//
void setup() {
	pinMode(PIN_MODE, INPUT);
	pinMode(PIN_COLOR, INPUT);

	setColor(color);

	rtc.init();
	startmillis = millis();

	// Init NeoPixel
	pixels_hour.setBrightness(BRIGHTNESS_HOUR);
	pixels_hour.begin();
	pixels_hour.show();
	pixels_minute.setBrightness(BRIGHTNESS_MINUTE);
	pixels_minute.begin();
	pixels_minute.show();
}

//
// loop
//
void loop() {
	readInput();
	setColor(color);
	now = rtc.getDateTime();

	// Calculation
	if (now.Hour() > 12)
		pix_hour = now.Hour() - 12;
	else
		pix_hour = now.Hour();
	pix_minute = abs(now.Minute() / (60.0 / NBPIXELS_MINUTE));
	pix_seconde = abs(now.Second() / (60.0 / NBPIXELS_SECONDE));
	pix_dixseconde = abs((((millis() - startmillis) / 17) % 60) / (60.0 / NBPIXELS_DIXSECONDE));

	// Display Hour
	pixels_hour.setPixelColor(pix_hour, color_hour);
	if (now.Hour() > 0)
		pixels_hour.setPixelColor(pix_hour - 1, 0);
	else
		pixels_hour.setPixelColor(NBPIXELS_HOUR - 1, 0);

	// Display minute
	pixels_minute.setPixelColor(pix_minute, color_minute);
	if (now.Minute() > 0 && (pix_minute - 1 != pix_seconde))
		pixels_minute.setPixelColor(pix_minute - 1, 0);
	else
		pixels_minute.setPixelColor(NBPIXELS_MINUTE - 1, 0);

	//TODO : tester si une led ne reste pas allumée. Si oui sortir le second bloc du if
	// Display second
	if (mode > 0) {
		if (pix_seconde != pix_minute)
			pixels_minute.setPixelColor(pix_seconde, color_seconde);
		if (now.Second() > 0 && (pix_seconde - 1 != pix_minute))
			pixels_minute.setPixelColor(pix_seconde - 1, 0);
		else
			pixels_minute.setPixelColor(NBPIXELS_SECONDE - 1, 0);
	}

	// TODO tester et ordonner comme pour les secondes
	// Display dixsecond
	if (mode > 1) {
		if (lastpix_dixseconde != pix_seconde && (lastpix_dixseconde != pix_minute))
			pixels_minute.setPixelColor(lastpix_dixseconde, 0);
		if (pix_dixseconde != pix_seconde && (pix_dixseconde != pix_minute))
			pixels_minute.setPixelColor(pix_dixseconde, color_dixseconde);
		lastpix_dixseconde = pix_dixseconde;
	}

	pixels_hour.show();
	pixels_minute.show();
	delay(delayval);
}


//
// readInput
//
void readInput()
{
	//Load input mode
	if (digitalRead(PIN_MODE) && !bfm_mode) {
		bfm_mode = true;
		mode += 1;
		if (mode > 2)
			mode = 0;
	}
	if (!digitalRead(PIN_MODE) && bfm_mode)
		bfm_mode = false;

	//Load color 
	if (digitalRead(PIN_COLOR) && !bfm_color) {
		bfm_color = true;
		color += 1;
		if (color > 2)
			color = 0;
	}
	if (!digitalRead(PIN_COLOR) && bfm_color)
		bfm_color = false;
}

//
// setColor
//
void setColor(int col)
{
	color_hour = pixels_hour.Color(colorTable[col][0][0], colorTable[col][0][1], colorTable[col][0][2], GAMMA);
	color_minute = pixels_hour.Color(colorTable[col][1][0], colorTable[col][1][1], colorTable[col][1][2], GAMMA);
	color_seconde = pixels_hour.Color(colorTable[col][2][0], colorTable[col][2][1], colorTable[col][2][2], GAMMA);
	color_dixseconde = pixels_hour.Color(colorTable[col][3][0], colorTable[col][3][1], colorTable[col][3][2], GAMMA);
}

//
// setRandomColor
//
uint32_t setRandomColor()
{
	unsigned int r, g, b, gamma;
	r = random(0, 255);
	g = random(0, 255);
	b = random(0, 255);
	gamma = random(0, 255);
	return pixels_hour.Color(r, g, b, 0);
}
