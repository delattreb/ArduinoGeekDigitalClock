//
// Created by Bruno on 15/09/2017.
//

#include "var.h"
#include <Wire.h>
#include "libDS3231.h"
#include <Adafruit_NeoPixel.h>
//#ifdef __AVR__
//#include <avr/power.h>
//#endif

uint32_t setRandomColor();

// Var
Adafruit_NeoPixel pixels_hour = Adafruit_NeoPixel(NBPIXELS_HOUR, PIN_HOUR, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_minute = Adafruit_NeoPixel(NBPIXELS_MINUTE, PIN_MINUTE, NEO_GRB + NEO_KHZ800);

// Color
uint32_t color_hour = pixels_hour.Color(0, 255, 0, GAMMA);
uint32_t color_minute = pixels_hour.Color(0, 0, 255, GAMMA);
uint32_t color_seconde = pixels_hour.Color(255, 127, 0, GAMMA);
uint32_t color_dixseconde = pixels_hour.Color(255, 0, 0, GAMMA);

int delayval = 0;
int pix_hour, pix_minute, pix_seconde, pix_dixseconde, lastpix_dixseconde;
unsigned long startmillis;

libDS3231 rtc;
RtcDateTime now;

void setup() {
	//#if defined (__AVR_ATtiny85__)
	//	if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
	//#endif
		//Init RTC
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

void loop() {
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

	// Display second
	if (pix_seconde != pix_minute)
		pixels_minute.setPixelColor(pix_seconde, color_seconde);
	if (now.Second() > 0 && (pix_seconde - 1 != pix_minute))
		pixels_minute.setPixelColor(pix_seconde - 1, 0);
	else
		pixels_minute.setPixelColor(NBPIXELS_SECONDE - 1, 0);

	// Display dixsecond
	if (lastpix_dixseconde != pix_seconde && (lastpix_dixseconde != pix_minute))
		pixels_minute.setPixelColor(lastpix_dixseconde, 0);
	if (pix_dixseconde != pix_seconde && (pix_dixseconde != pix_minute))
		pixels_minute.setPixelColor(pix_dixseconde, color_dixseconde);
	lastpix_dixseconde = pix_dixseconde;

	pixels_hour.show();
	pixels_minute.show();
	delay(delayval);
}

uint32_t setRandomColor()
{
	unsigned int r, g, b, gamma;
	r = random(0, 255);
	g = random(0, 255);
	b = random(0, 255);
	gamma = random(0, 255);
	return pixels_hour.Color(r, g, b, 0);
}
