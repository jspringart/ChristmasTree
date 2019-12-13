/*
 Name:		ChristmasTree.ino
 Created:	12/28/2018 10:00:54 AM
 Author:	Jason Springart
*/

#include <TimerOne.h>

unsigned int duty = 0;

void setup() {
	pinMode(10, OUTPUT);
	Timer1.initialize(500000);
	Timer1.pwm(10, duty);
	Timer1.attachInterrupt(callback);
}

void loop() {

}

void callback()
{
	if (duty = 1024) {
		duty = 0;
	}
	else {
		duty++;
	}
	Timer1.setPwmDuty(duty);
}
//enum lightcolor {
//	WHITE,
//	MULTI,
//	OFF
//};
//void setLightColor(lightcolor lights);
//
//const bool DEBUG = true;
//
//// pin mapping
//int lightsPinA = 10;
//int lightsPinB = 11;
//int lightsPinEnable = 6;
//
//// timing
//unsigned long currentMicros;
//unsigned long previousDebugMicros;
//double debugInterval;
//unsigned long previousShowMicros;
//double showInterval;
//
//// leds
//bool lightsOn = true;
//lightcolor ledColor = WHITE;
//
//// show
//long showNumber;
//unsigned long delay1 = 0;
//unsigned long delay2 = 100000;
//bool fadeUp = true;
//
//void setup() {
//	// setup pins
//	pinMode(lightsPinA, OUTPUT);
//	pinMode(lightsPinB, OUTPUT);
//	pinMode(lightsPinEnable, OUTPUT);
//
//	// setup variables
//	currentMicros = 0;
//	previousDebugMicros = 0;
//	debugInterval = 1; // in seconds
//	previousShowMicros = 0;
//	showInterval = 100000;
//
//	randomSeed(analogRead(0));
//
//	// start serial
//	Serial.begin(9600);
//
//	// initalize lights
//	enableLights(lightsOn);
//
//	// initalize light color
//	setLightColor(ledColor);
//}
//
//void loop() {
//	currentMicros = micros();
//	analogWrite(lightsPinA, 255);
//	/*showNumber = random(3);
//
//	setLightColor(ledColor);
//	show2();*/
//	
//	if (DEBUG) {
//		displayDebugInfo();
//	}
//}
//
//void displayDebugInfo() {
//	if ((currentMicros - previousDebugMicros >= (debugInterval * 1000000L))) {
//		String debugInfo = "DEBUG: ";
//
//		debugInfo += "IVAL=" + String(debugInterval) + " ";
//		debugInfo += "ENAB=" + getLightStatus() + " ";
//		debugInfo += "COLO=" + getLightColor() + " ";
//		debugInfo += "DELA1=" + String(delay1) + " ";
//		debugInfo += "DELA2=" + String(delay2) + " ";
//
//		Serial.println(debugInfo);
//
//		previousDebugMicros = micros();
//	}
//}
//
//String getLightColor() {
//	switch (ledColor)
//	{
//	case WHITE:
//		return "white";
//		break;
//	case MULTI:
//		return "multi";
//		break;
//	case OFF:
//		return "off";
//		break;
//	default:
//		break;
//	}
//}
//
//String getLightStatus() {
//	if (lightsOn)
//		return "on";
//
//	if (!lightsOn)
//		return "off";
//}
//
//void setLightColor(lightcolor lights) {
//
//	switch (lights)
//	{
//	case WHITE:
//		PORTB = B00010000;
//		break;
//	case MULTI:
//		PORTB = B00100000;
//		break;
//	case OFF:
//		PORTB = B00000000;
//		PORTB = B00000000;
//		break;
//	default:
//		break;
//	}
//}
//
//void enableLights(bool value)
//{
//	digitalWrite(lightsPinEnable, value);
//}
//
//void show() {
//	if ((currentMicros - previousShowMicros >= (showInterval))) {
//		
//		ledColor = (lightcolor)showNumber;
//
//		previousShowMicros = micros();
//	}
//}
//
//void show2() {
//	if ((currentMicros - previousShowMicros >= (showInterval))) {
//
//		if (fadeUp) {
//			delay1++;
//			delay2--;
//			if (delay1 >= 100000)
//			{
//				fadeUp = false;
//			}
//		}
//		else
//		{
//			delay1--;
//			delay2++;
//			if (delay1 <= 0)
//			{
//				fadeUp = true;
//			}
//		}		
//
//		if (ledColor == OFF) {
//			ledColor = WHITE;
//			showInterval = delay1;
//		}
//		else
//		{
//			ledColor = OFF;
//			showInterval = delay2;
//		}		
//
//		previousShowMicros = micros();
//	}
//}
//
//
//
//
//
//
//
//
////
//
////const int MAX_BRIGHTNESS = 255;
////const int MIN_BRIGHTNESS = 0;
////const int MAX_ACTION = 4;
////const int MIN_ACTION = 0;
//
////
////// timing
//
////unsigned long previousBlinkColorMicros;
////unsigned long previousFadeBrightnessMicros;
////unsigned long previousFadeColorMicros;
////unsigned long previousResetMicros;
////
////// variables
////bool lightColorState;	// TRUE = WHITE; FALSE = COLOR
////int lightBrightness;	// 255 = ON; 0 = OFF
////int action;
//
////unsigned long colorChanageInterval;
////unsigned long fadeColorInterval;
////bool fadeUp;
////unsigned long tempFadeColorInterval;
////
////void setup()
////{
//
////
//
////
////	// setup variables
//
////	previousBlinkColorMicros = 0;
////	previousFadeBrightnessMicros = 0;
////	previousFadeColorMicros = 0;
////
////	// load from eeprom?
////	lightColorState = true;
////	lightBrightness = 255;
////	action = -1;
////	delay1 = 1000L * 1000L;
////	delay2 = 1000L * 1000L;
////	colorChanageInterval = delay1;
////	fadeColorInterval = delay1;
////	fadeUp = true;
////	tempFadeColorInterval = fadeColorInterval;
////}
////
////void loop()
////{
////	checkSerial();
////
//
////	setAction();
////	setLightBrightness(lightBrightness);
////	setLightColor(lightColorState);
////
////	if (DEBUG)
////	{
////		debugInfo();
////	}
////}
////
////void checkSerial() 
////{
////	String data = "";
////	while (Serial.available() > 0)
////	{
////		data += Serial.readString();
////		if (data.endsWith("\n"))
////		{
////			data.replace("\n", "");
////			splitSerialData(data);
////		}
////	}
////}
////
////void splitSerialData(String data)
////{
////	String tempData = data;
////	tempData.trim();
////	while (tempData.length() > 0)
////	{
////		int index = tempData.indexOf(";");
////		String subString = "";
////		if (index == 0 || index == -1)
////		{			
////			subString = tempData.substring(0);
////		}
////		else
////		{
////			subString = tempData.substring(0, index + 1);
////		}
////		tempData.replace(subString, "");
////		parseSerialData(subString);
////	}
////}
////
////void parseSerialData(String data)
////{
////	if (data.substring(0, 2) == "ac")
////	{
////		int a = atoi(data.substring(2).c_str());
////		action = a;
////	}
////	else if (data.substring(0, 2) == "d1")
////	{
////		float f = data.substring(2).toFloat();
////		delay1 = f * 1000L;
////	}
////	else if (data.substring(0, 2) == "d2")
////	{
////		float f = data.substring(2).toFloat();
////		delay2 = f * 1000L;
////	}
////	else if (data.substring(0, 2) == "fc")
////	{
////		float f = data.substring(2).toFloat();
////		fadeColorInterval = f * 1000L;
////		tempFadeColorInterval = fadeColorInterval;
////	}
////	else if (data.substring(0, 2) == "br")
////	{
////		int a = atoi(data.substring(2).c_str());
////		lightBrightness = a;
////	}
//////	else if (data.substring(0, 2) == "dd") {
//////		int d = atoi(data.substring(2).c_str());
//////		delay1 = d;
//////		delay2 = d;
//////	}
//////	else if (data.substring(0, 2) == "ac") {
//////		int a = atoi(data.substring(2).c_str());
//////		if (a == 3)
//////		{
//////			delay1 = 2000;
//////			delay2 = 0;
//////		}
//////		action = a;
//////	}
//////	
//////	else if (data.substring(0, 2) == "pw") {
//////		int a = atoi(data.substring(2).c_str());
//////		powerState = a;
//////	}
////	
//////	else if (data.substring(0, 2) == "cl") {
//////		int a = atoi(data.substring(2).c_str());
//////		colorState = a;
//////	}
////}
////
////void setAction()
////{
////	switch (action)
////	{
////		case 0:
////			blinkLightColor();
////			break;
////
////		case 1:
////			fadeBrightness();
////			break;
////
////		case 2:
////			blinkLightColor();
////			fadeColor();
////			break;
////
////		case 3:
////			fadeColorReset();
////			break;
////
////		default:
////			break;
////	}
////}
////
////void blinkLightColor()
////{
////	if (currentMicros - previousBlinkColorMicros >= colorChanageInterval)
////	{
////		lightColorState = !lightColorState;
////
////		if (lightColorState)
////		{
////			colorChanageInterval = delay1;
////		}
////		else
////		{
////			colorChanageInterval = delay2;
////		}
////
////		previousBlinkColorMicros = micros();
////	}
////}
////
////void fadeBrightness()
////{
//////	if (currentMicros - previousMillis >= fadeInterval) {
//////
//////		if (fadeUp) {
//////			brightness++;
//////			if (brightness >= MAX_BRIGHTNESS) {
//////				fadeUp = !fadeUp;
//////			}
//////			
//////		} else {
//////			brightness--;
//////			if (brightness <= MIN_BRIGHTNESS) {
//////				fadeUp = !fadeUp;
//////			}
//////		}
//////
//////		previousMillis = millis();
//////	}
////}
////
////void fadeColor()
////{
////	if (currentMicros - previousFadeColorMicros >= fadeColorInterval)
////	{
////		if (fadeUp)
////		{
////			delay1--;
////			delay2++;
////			/*if ((delay2 / tempFadeColorInterval) * 100 > 50)
////			{
////				fadeColorInterval += 1;
////			}*/
////			if (delay1 <= 0)
////			{
////				//fadeUp = !fadeUp;
////				previousResetMicros = micros();
////				action = 3;
////				//fadeColorInterval = tempFadeColorInterval;
////			}
////		}
////		else
////		{
////			delay1++;
////			delay2--;
////			/*if ((delay1 / tempFadeColorInterval) * 100 > 50)
////			{
////				fadeColorInterval += 1;
////			}*/
////			if (delay2 <= 0)
////			{
////				//fadeUp = !fadeUp;
////				previousResetMicros = micros();
////				action = 3;
////				//fadeColorInterval = tempFadeColorInterval;
////			}
////		}
////		previousFadeColorMicros = micros();
////	}
////}
////
////void fadeColorReset()
////{
////	if (currentMicros - previousResetMicros >= 5000000L)
////	{
////		fadeUp = !fadeUp;
////		action = 2;
////	}
////}
////
//
////
//
////
////void debugInfo()
////{
//
////}
////
////
////
////
////
////
////
//////int colorInterval;
//////int fadeInterval;
//////
//////// settings
//////int action;
//////int delay1;
//////int delay2;
//////bool powerState;
//////bool colorState;
//////int brightness;
//////bool fadeUp;
//////
//////// the setup function runs once when you press reset or power the board
//////void setup() {
//////
//////	// set settings to startup values (maybe read from eeprom)
//////	action = 1;
//////	delay1 = 1000;
//////	delay2 = 1000;
//////	powerState = true;
//////	colorState = true;
//////	brightness = 255;
//////	fadeUp = true;
//////
//////	powerInterval = delay1;
//////	colorInterval = delay1;
//////	fadeInterval = 60;
//////}
//////
//////// the loop function runs over and over again until power down or reset
//////void loop() {
//////	checkSerial();
//////
//////	currentMicros = micros();
//////	setAction();
//////
//////	setLightBrightness(brightness);
//////	setLightColor(colorState);	
//////}
////
//////d11000; d21000; ac1
//////
//////d10; d210; ac3; fa1000
//////
//////d11.5; d20; ac2; fc1.5
//////
//////d11000000; d21000000; ac0