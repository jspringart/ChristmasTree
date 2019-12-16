/*
 Name:		ChristmasTree.ino
 Created:	12/28/2018 10:00:54 AM
 Author:	Jason Springart
 Desc:
	Mode:
		  1 - Static
		  2 - Fade
*/

#include <TimerOne.h>

enum lightcolor {
	WHITE,
	MULTI,
	BOTH,
	OFF
};

enum show {
	STATIC_WHITE,
	STATIC_MULTI,
	FADE_WHITE,
	FADE_MULTI,
	FADE_SEQ_BOTH,
	FADE_BOTH,
	SHOW_OFF
};

const bool DEBUG = false;
const unsigned long STARTUP_DELAY = 5000000;
const int MAX_PERIOD = 2560;
const int MIN_PERIOD = 0;
const int REVERSE_DELAY = 50;

// pin mapping
int lightsPinA = 13; // WHITE
int lightsPinB = 12; // MULTI
int lightsPinEnable = 6;

int onDelay = MIN_PERIOD;
int offDelay = MAX_PERIOD;
bool fadeUp = true;
int fadeCount = 0; 
lightcolor color = WHITE;
bool seqBothColors = false;
int pin = lightsPinA;

// pause
bool pauseBit = false;
int pauseCount = 0;
int pauseUp = 1000;
int pauseDown = 2000;

// show
show action = STATIC_WHITE;

void setup() {
	pinMode(lightsPinA, OUTPUT);
	pinMode(lightsPinB, OUTPUT);
	pinMode(lightsPinEnable, OUTPUT);

	// start serial
	Serial.begin(9600);

	digitalWrite(lightsPinA, LOW);
	digitalWrite(lightsPinB, LOW);
	digitalWrite(lightsPinEnable, HIGH);

	// setup variables
	Timer1.initialize(STARTUP_DELAY);
	Timer1.attachInterrupt(startup);
}

void loop() {
	if (DEBUG) {
		displayDebugInfo();
	}
}

void startup() {
	// Load from eeprom
	// Select show
	switch (action)	{
	case STATIC_WHITE:
		color = WHITE;
		seqBothColors = false;
		Timer1.setPeriod(5000000);
		Timer1.attachInterrupt(static_color);
		break;

	case STATIC_MULTI:
		color = MULTI;
		seqBothColors = false;
		Timer1.setPeriod(5000000);
		Timer1.attachInterrupt(static_color);
		break;

	case FADE_WHITE:
		color = WHITE;
		seqBothColors = false;
		Timer1.setPeriod(offDelay);
		Timer1.attachInterrupt(fade);
		break;

	case FADE_MULTI:
		color = MULTI;
		seqBothColors = false;
		Timer1.setPeriod(offDelay);
		Timer1.attachInterrupt(fade);
		break;

	case FADE_SEQ_BOTH:
		color = WHITE;
		seqBothColors = true;
		Timer1.setPeriod(offDelay);
		Timer1.attachInterrupt(fade);
		break;

	case FADE_BOTH:
		color = BOTH;
		seqBothColors = false;
		Timer1.setPeriod(offDelay);
		Timer1.attachInterrupt(fade);
		break;
	}
}

void fade() {
	if (pauseBit) {
		pauseCount++;
		if (fadeUp && (pauseCount >= pauseUp)) {
			pauseBit = false;
			pauseCount = 0;
		}else if (!fadeUp && (pauseCount >= pauseDown)) {
			pauseBit = false;
			pauseCount = 0;
		}
	}
	else if (!pauseBit) {
		if (fadeUp) {
			offDelay--;
			onDelay++;
			if (offDelay == REVERSE_DELAY) {
				fadeUp = false;
				pauseBit = true;
				fadeCount++;
				return;
			}
		}
		else {
			offDelay++;
			onDelay--;
			if (onDelay == REVERSE_DELAY) {
				fadeUp = true;
				pauseBit = true;
				fadeCount++;
				return;
			}
		}
	}

	if ((bool)digitalRead(pin) == true)
	{
		Timer1.setPeriod(offDelay);
	}
	else
	{
		Timer1.setPeriod(onDelay);
	}

	if (seqBothColors)
	{
		if (fadeCount == 2)
		{
			fadeCount = 0;
			if (pin == lightsPinA) {
				pin = lightsPinB;
			}
			else
			{
				pin = lightsPinA;
			}
		}
	}

	// colors
	if (color == BOTH)	{
		digitalWrite(lightsPinA, !(bool)digitalRead(lightsPinA));
		digitalWrite(lightsPinB, !(bool)digitalRead(lightsPinB));
	} else {
		digitalWrite(pin, !(bool)digitalRead(pin));
	}	
}

void static_color() {
	switch (color) {
	case WHITE:
		digitalWrite(lightsPinA, HIGH);
		digitalWrite(lightsPinB, LOW);
		break;

	case MULTI:
		digitalWrite(lightsPinA, LOW);
		digitalWrite(lightsPinB, HIGH);
		break;
	}
}

////void setLightColor(lightcolor lights) {
////
////	switch (lights)
////	{
////	case WHITE:
////		PORTB = B00010000;
////		break;
////	case MULTI:
////		PORTB = B00100000;
////		break;
////	case OFF:
////		PORTB = B00000000;
////		PORTB = B00000000;
////		break;
////	default:
////		break;
////	}
////}

void pause() {
	//digitalWrite(lightsPinA, LOW);
	//digitalWrite(lightsPinB, LOW);
	//show
	pauseCount++;
	/*if (pauseCount == 2) {
		
		
	}*/
	
	//Timer1.detachInterrupt();
	if (fadeUp)
	{
		Timer1.setPeriod(onDelay);
	}
	else
	{
		Timer1.setPeriod(offDelay);
	}

	digitalWrite(lightsPinA, !(bool)digitalRead(lightsPinA));
	digitalWrite(lightsPinB, !(bool)digitalRead(lightsPinB));
	
	if (pauseCount == 10000)
	{
		pauseCount = 0;
		//Timer1.attachInterrupt(testLOW);
	}	
}

void displayDebugInfo() {
	//if ((currentMicros - previousDebugMicros >= (debugInterval * 1000000L))) {
		String debugInfo = "DEBUG: ";

		//debugInfo += "IVAL=" + String(debugInterval) + " ";
		//debugInfo += "SHOW=" + String(action) + " ";
		debugInfo += "ONDEL=" + String(onDelay) + " ";
		debugInfo += "OFFDEL=" + String(offDelay) + " ";

		Serial.println(debugInfo);

		//previousDebugMicros = micros();
	//}
}

































//unsigned long currentMicros;
//unsigned long previousDebugMicros;
//unsigned long previousTestMicros;
//double debugInterval;

//

//int delay1 = 0;
//int delay2 = period;
//int pin = lightsPinA;
//int action = 0;
//
//void setup() {

//


//
//	currentMicros = 0;
//	previousDebugMicros = 0;
//	debugInterval = 1; // in seconds
//}
//
//unsigned long currentFadeMicros = 0;
//unsigned long previousFadeMicros = 0;
//
//void loop() {
//	currentMicros = micros();
//	currentFadeMicros = micros();
//	checkSerial();
//

//}
//
//void checkSerial() 
//{
//	String data = "";
//	while (Serial.available() > 0)
//	{
//		data += Serial.readString();
//		if (data.endsWith("\n"))
//		{
//			data.replace("\n", "");
//			splitSerialData(data);
//		}
//	}
//}
//
//void splitSerialData(String data)
//{
//	String tempData = data;
//	tempData.trim();
//	while (tempData.length() > 0)
//	{
//		int index = tempData.indexOf(";");
//		String subString = "";
//		if (index == 0 || index == -1)
//		{			
//			subString = tempData.substring(0);
//		}
//		else
//		{
//			subString = tempData.substring(0, index + 1);
//		}
//		tempData.replace(subString, "");
//		parseSerialData(subString);
//	}
//}
//
//void parseSerialData(String data)
//{
//	if (data.substring(0, 2) == "sh") {
//		action = atoi(data.substring(2).c_str());
//		switch (action)
//		{
//		case 0:
//			//Timer1.detachInterrupt();
//			//Timer1.attachInterrupt(seqFade);
//			break;
//		case 1:
//			//Timer1.detachInterrupt();
//			//Timer1.attachInterrupt(fadeIntoBlack);
//			break;
//
//		case 2:
//			//Timer1.detachInterrupt();
//			//Timer1.attachInterrupt(on);
//			break;
//		}
//	}
//	/*if (data.substring(0, 2) == "ac")
//	{
//		int a = atoi(data.substring(2).c_str());
//		action = a;
//	}
//	else if (data.substring(0, 2) == "d1")
//	{
//		float f = data.substring(2).toFloat();
//		delay1 = f * 1000L;
//	}
//	else if (data.substring(0, 2) == "d2")
//	{
//		float f = data.substring(2).toFloat();
//		delay2 = f * 1000L;
//	}
//	else if (data.substring(0, 2) == "fc")
//	{
//		float f = data.substring(2).toFloat();
//		fadeColorInterval = f * 1000L;
//		tempFadeColorInterval = fadeColorInterval;
//	}
//	else if (data.substring(0, 2) == "br")
//	{
//		int a = atoi(data.substring(2).c_str());
//		lightBrightness = a;
//	}
//	else if (data.substring(0, 2) == "dd") {
//		int d = atoi(data.substring(2).c_str());
//		delay1 = d;
//		delay2 = d;
//	}
//	else if (data.substring(0, 2) == "ac") {
//		int a = atoi(data.substring(2).c_str());
//		if (a == 3)
//		{
//			delay1 = 2000;
//			delay2 = 0;
//		}
//		action = a;
//	}
//	
//	else if (data.substring(0, 2) == "pw") {
//		int a = atoi(data.substring(2).c_str());
//		powerState = a;
//	}
//	
//	else if (data.substring(0, 2) == "cl") {
//		int a = atoi(data.substring(2).c_str());
//		colorState = a;
//	}*/
//}
//

//
//
//
//void test() {
//	
//	if (currentMicros - previousTestMicros >= 5000L) {	
//		if (fadeUp)
//		{
//			delay1++;
//			delay2--;
//		}
//		else if (!fadeUp)
//		{
//			delay1--;
//			delay2++;
//		}
//		if (delay1 == period)
//		{
//			fadeUp = false;
//			//delayMicroseconds(1000000);
//		}
//		else if (delay1 == 0)
//		{
//			fadeUp = true;
//			//delayMicroseconds(1000000);
//		}
//
//		if (currentFadeMicros - previousFadeMicros >= delay1) {
//			digitalWrite(lightsPinA, !(bool)digitalRead(lightsPinA));
//			digitalWrite(lightsPinB, !(bool)digitalRead(lightsPinB));
//			previousFadeMicros = micros();
//		}
//
//		//after delay2 turn hight
//		if (currentFadeMicros - previousFadeMicros >= delay2) {
//			digitalWrite(lightsPinA, HIGH);
//			digitalWrite(lightsPinB, LOW);
//			previousFadeMicros = micros();
//		}
//
//		/*digitalWrite(lightsPinA, HIGH);
//		digitalWrite(lightsPinB, LOW);
//		delayMicroseconds(delay1);
//		digitalWrite(lightsPinA, !(bool)digitalRead(lightsPinA));
//		digitalWrite(lightsPinB, !(bool)digitalRead(lightsPinB));
//		delayMicroseconds(delay2);
//	}*/
//	}
//}
//
//void seqFade()
//{
//	if (fadeUp)
//	{
//		delay1++;
//		delay2--;
//	}
//	else if(!fadeUp)
//	{
//		delay1--;
//		delay2++;
//	}
//	if (delay1 == period)
//	{
//		fadeUp = false;
//		
//	}
//	else if(delay1 == 0)
//	{
//		fadeUp = true;
//		if (pin == lightsPinA) {
//			pin = lightsPinB;
//		}
//		else {
//			pin = lightsPinA;
//		}
//	}
//
//	if (currentMicros - previousFadeMicros >= delay1) {
//		digitalWrite(pin, LOW);
//	}
//
//	if (currentMicros - previousFadeMicros >= delay1) {
//		digitalWrite(pin, HIGH);
//	}
//
//	previousFadeMicros = micros();
//
//	/*delayMicroseconds(delay1);
//	digitalWrite(pin, LOW);
//	delayMicroseconds(delay2);*/
//}
//
//void fadeIntoBlack()
//{
//	if (fadeUp)
//	{
//		delay1++;
//		delay2--;
//	}
//	else if (!fadeUp)
//	{
//		delay1--;
//		delay2++;
//	}
//	if (delay1 == period)
//	{
//		fadeUp = false;
//		//delayMicroseconds(1000000);
//	}
//	else if (delay1 == 0)
//	{
//		fadeUp = true;
//		//delayMicroseconds(1000000);
//	}
//
//	digitalWrite(lightsPinA, HIGH);
//	digitalWrite(lightsPinB, LOW);
//	delayMicroseconds(delay1);
//	digitalWrite(lightsPinA, !(bool)digitalRead(lightsPinA));
//	digitalWrite(lightsPinB, !(bool)digitalRead(lightsPinB));
//	delayMicroseconds(delay2);
//}
//
//void on()
//{	
//	digitalWrite(lightsPinA, HIGH);
//	digitalWrite(lightsPinB, LOW);
//}
//

////void setLightColor(lightcolor lights);
////
//
////
//
////
////// timing
//
////unsigned long previousShowMicros;
////double showInterval;
////
////// leds
////bool lightsOn = true;
////lightcolor ledColor = WHITE;
////
////// show
////long showNumber;
////unsigned long delay1 = 0;
////unsigned long delay2 = 100000;
////bool fadeUp = true;
////
////void setup() {
////	// setup pins
////	pinMode(lightsPinA, OUTPUT);
////	pinMode(lightsPinB, OUTPUT);
////	pinMode(lightsPinEnable, OUTPUT);
////
//
////	previousShowMicros = 0;
////	showInterval = 100000;
////
////	randomSeed(analogRead(0));
////
////	
////
////	// initalize lights
////	enableLights(lightsOn);
////
////	// initalize light color
////	setLightColor(ledColor);
////}
////
////void loop() {
////	
////	analogWrite(lightsPinA, 255);
////	/*showNumber = random(3);
////
////	setLightColor(ledColor);
////	show2();*/
////	
//
////}
////
//
////
////String getLightColor() {
////	switch (ledColor)
////	{
////	case WHITE:
////		return "white";
////		break;
////	case MULTI:
////		return "multi";
////		break;
////	case OFF:
////		return "off";
////		break;
////	default:
////		break;
////	}
////}
////
////String getLightStatus() {
////	if (lightsOn)
////		return "on";
////
////	if (!lightsOn)
////		return "off";
////}
////

////
////void enableLights(bool value)
////{
////	digitalWrite(lightsPinEnable, value);
////}
////
////void show() {
////	if ((currentMicros - previousShowMicros >= (showInterval))) {
////		
////		ledColor = (lightcolor)showNumber;
////
////		previousShowMicros = micros();
////	}
////}
////
////void show2() {
////	if ((currentMicros - previousShowMicros >= (showInterval))) {
////
////		if (fadeUp) {
////			delay1++;
////			delay2--;
////			if (delay1 >= 100000)
////			{
////				fadeUp = false;
////			}
////		}
////		else
////		{
////			delay1--;
////			delay2++;
////			if (delay1 <= 0)
////			{
////				fadeUp = true;
////			}
////		}		
////
////		if (ledColor == OFF) {
////			ledColor = WHITE;
////			showInterval = delay1;
////		}
////		else
////		{
////			ledColor = OFF;
////			showInterval = delay2;
////		}		
////
////		previousShowMicros = micros();
////	}
////}
////
////
////
////
////
////
////
////
//////
////
//////const int MAX_BRIGHTNESS = 255;
//////const int MIN_BRIGHTNESS = 0;
//////const int MAX_ACTION = 4;
//////const int MIN_ACTION = 0;
////
//////
//////// timing
////
//////unsigned long previousBlinkColorMicros;
//////unsigned long previousFadeBrightnessMicros;
//////unsigned long previousFadeColorMicros;
//////unsigned long previousResetMicros;
//////
//////// variables
//////bool lightColorState;	// TRUE = WHITE; FALSE = COLOR
//////int lightBrightness;	// 255 = ON; 0 = OFF
//////int action;
////
//////unsigned long colorChanageInterval;
//////unsigned long fadeColorInterval;
//////bool fadeUp;
//////unsigned long tempFadeColorInterval;
//////
//////void setup()
//////{
////
//////
////
//////
//////	// setup variables
////
//////	previousBlinkColorMicros = 0;
//////	previousFadeBrightnessMicros = 0;
//////	previousFadeColorMicros = 0;
//////
//////	// load from eeprom?
//////	lightColorState = true;
//////	lightBrightness = 255;
//////	action = -1;
//////	delay1 = 1000L * 1000L;
//////	delay2 = 1000L * 1000L;
//////	colorChanageInterval = delay1;
//////	fadeColorInterval = delay1;
//////	fadeUp = true;
//////	tempFadeColorInterval = fadeColorInterval;
//////}
//////
//////void loop()
//////{
//////	checkSerial();
//////
////
//////	setAction();
//////	setLightBrightness(lightBrightness);
//////	setLightColor(lightColorState);
//////
//////	if (DEBUG)
//////	{
//////		debugInfo();
//////	}
//////}
//////
//
//////
//
//////
//
//////
//////void setAction()
//////{
//////	switch (action)
//////	{
//////		case 0:
//////			blinkLightColor();
//////			break;
//////
//////		case 1:
//////			fadeBrightness();
//////			break;
//////
//////		case 2:
//////			blinkLightColor();
//////			fadeColor();
//////			break;
//////
//////		case 3:
//////			fadeColorReset();
//////			break;
//////
//////		default:
//////			break;
//////	}
//////}
//////
//////void blinkLightColor()
//////{
//////	if (currentMicros - previousBlinkColorMicros >= colorChanageInterval)
//////	{
//////		lightColorState = !lightColorState;
//////
//////		if (lightColorState)
//////		{
//////			colorChanageInterval = delay1;
//////		}
//////		else
//////		{
//////			colorChanageInterval = delay2;
//////		}
//////
//////		previousBlinkColorMicros = micros();
//////	}
//////}
//////
//////void fadeBrightness()
//////{
////////	if (currentMicros - previousMillis >= fadeInterval) {
////////
////////		if (fadeUp) {
////////			brightness++;
////////			if (brightness >= MAX_BRIGHTNESS) {
////////				fadeUp = !fadeUp;
////////			}
////////			
////////		} else {
////////			brightness--;
////////			if (brightness <= MIN_BRIGHTNESS) {
////////				fadeUp = !fadeUp;
////////			}
////////		}
////////
////////		previousMillis = millis();
////////	}
//////}
//////
//////void fadeColor()
//////{
//////	if (currentMicros - previousFadeColorMicros >= fadeColorInterval)
//////	{
//////		if (fadeUp)
//////		{
//////			delay1--;
//////			delay2++;
//////			/*if ((delay2 / tempFadeColorInterval) * 100 > 50)
//////			{
//////				fadeColorInterval += 1;
//////			}*/
//////			if (delay1 <= 0)
//////			{
//////				//fadeUp = !fadeUp;
//////				previousResetMicros = micros();
//////				action = 3;
//////				//fadeColorInterval = tempFadeColorInterval;
//////			}
//////		}
//////		else
//////		{
//////			delay1++;
//////			delay2--;
//////			/*if ((delay1 / tempFadeColorInterval) * 100 > 50)
//////			{
//////				fadeColorInterval += 1;
//////			}*/
//////			if (delay2 <= 0)
//////			{
//////				//fadeUp = !fadeUp;
//////				previousResetMicros = micros();
//////				action = 3;
//////				//fadeColorInterval = tempFadeColorInterval;
//////			}
//////		}
//////		previousFadeColorMicros = micros();
//////	}
//////}
//////
//////void fadeColorReset()
//////{
//////	if (currentMicros - previousResetMicros >= 5000000L)
//////	{
//////		fadeUp = !fadeUp;
//////		action = 2;
//////	}
//
////////int colorInterval;
////////int fadeInterval;
////////
////////// settings
////////int action;
////////int delay1;
////////int delay2;
////////bool powerState;
////////bool colorState;
////////int brightness;
////////bool fadeUp;
////////
////////// the setup function runs once when you press reset or power the board
////////void setup() {
////////
////////	// set settings to startup values (maybe read from eeprom)
////////	action = 1;
////////	delay1 = 1000;
////////	delay2 = 1000;
////////	powerState = true;
////////	colorState = true;
////////	brightness = 255;
////////	fadeUp = true;
////////
////////	powerInterval = delay1;
////////	colorInterval = delay1;
////////	fadeInterval = 60;
////////}
////////
////////// the loop function runs over and over again until power down or reset
////////void loop() {
////////	checkSerial();
////////
////////	currentMicros = micros();
////////	setAction();
////////
////////	setLightBrightness(brightness);
////////	setLightColor(colorState);	
////////}