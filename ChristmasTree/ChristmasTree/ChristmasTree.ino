/*
 Name:		ChristmasTree.ino
 Created:	12/28/2018 10:00:54 AM
 Author:	Jason Springart
*/

// pin mapping
int lightsPinA = 13;
int lightsPinB = 12;
int lightsPinEnable = 6;

// constants
const int MAX_BRIGHTNESS = 255;
const int MIN_BRIGHTNESS = 0;
const int MAX_ACTION = 4;
const int MIN_ACTION = 0;
const bool DEBUG = false;

// timing
unsigned long currentMicros;
unsigned long previousBlinkColorMicros;
unsigned long previousFadeBrightnessMicros;
unsigned long previousFadeColorMicros;
unsigned long previousResetMicros;

// variables
bool lightColorState;	// TRUE = WHITE; FALSE = COLOR
int lightBrightness;	// 255 = ON; 0 = OFF
int action;
unsigned long delay1;
unsigned long delay2;
unsigned long colorChanageInterval;
unsigned long fadeColorInterval;
bool fadeUp;
unsigned long tempFadeColorInterval;

void setup()
{
	// start serial
	Serial.begin(9600);

	// setup pins
	pinMode(lightsPinA, OUTPUT);
	pinMode(lightsPinB, OUTPUT);
	pinMode(lightsPinEnable, OUTPUT);

	// setup variables
	currentMicros = 0;
	previousBlinkColorMicros = 0;
	previousFadeBrightnessMicros = 0;
	previousFadeColorMicros = 0;

	// load from eeprom?
	lightColorState = true;
	lightBrightness = 255;
	action = -1;
	delay1 = 1000L * 1000L;
	delay2 = 1000L * 1000L;
	colorChanageInterval = delay1;
	fadeColorInterval = delay1;
	fadeUp = true;
	tempFadeColorInterval = fadeColorInterval;
}

void loop()
{
	checkSerial();

	currentMicros = micros();
	setAction();
	setLightBrightness(lightBrightness);
	setLightColor(lightColorState);

	if (DEBUG)
	{
		debugInfo();
	}
}

void checkSerial() 
{
	String data = "";
	while (Serial.available() > 0)
	{
		data += Serial.readString();
		if (data.endsWith("\n"))
		{
			data.replace("\n", "");
			splitSerialData(data);
		}
	}
}

void splitSerialData(String data)
{
	String tempData = data;
	tempData.trim();
	while (tempData.length() > 0)
	{
		int index = tempData.indexOf(";");
		String subString = "";
		if (index == 0 || index == -1)
		{			
			subString = tempData.substring(0);
		}
		else
		{
			subString = tempData.substring(0, index + 1);
		}
		tempData.replace(subString, "");
		parseSerialData(subString);
	}
}

void parseSerialData(String data)
{
	if (data.substring(0, 2) == "ac")
	{
		int a = atoi(data.substring(2).c_str());
		action = a;
	}
	else if (data.substring(0, 2) == "d1")
	{
		float f = data.substring(2).toFloat();
		delay1 = f * 1000L;
	}
	else if (data.substring(0, 2) == "d2")
	{
		float f = data.substring(2).toFloat();
		delay2 = f * 1000L;
	}
	else if (data.substring(0, 2) == "fc")
	{
		float f = data.substring(2).toFloat();
		fadeColorInterval = f * 1000L;
		tempFadeColorInterval = fadeColorInterval;
	}
	else if (data.substring(0, 2) == "br")
	{
		int a = atoi(data.substring(2).c_str());
		lightBrightness = a;
	}
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
	
//	else if (data.substring(0, 2) == "cl") {
//		int a = atoi(data.substring(2).c_str());
//		colorState = a;
//	}
}

void setAction()
{
	switch (action)
	{
		case 0:
			blinkLightColor();
			break;

		case 1:
			fadeBrightness();
			break;

		case 2:
			blinkLightColor();
			fadeColor();
			break;

		case 3:
			fadeColorReset();
			break;

		default:
			break;
	}
}

void blinkLightColor()
{
	if (currentMicros - previousBlinkColorMicros >= colorChanageInterval)
	{
		lightColorState = !lightColorState;

		if (lightColorState)
		{
			colorChanageInterval = delay1;
		}
		else
		{
			colorChanageInterval = delay2;
		}

		previousBlinkColorMicros = micros();
	}
}

void fadeBrightness()
{
//	if (currentMicros - previousMillis >= fadeInterval) {
//
//		if (fadeUp) {
//			brightness++;
//			if (brightness >= MAX_BRIGHTNESS) {
//				fadeUp = !fadeUp;
//			}
//			
//		} else {
//			brightness--;
//			if (brightness <= MIN_BRIGHTNESS) {
//				fadeUp = !fadeUp;
//			}
//		}
//
//		previousMillis = millis();
//	}
}

void fadeColor()
{
	if (currentMicros - previousFadeColorMicros >= fadeColorInterval)
	{
		if (fadeUp)
		{
			delay1--;
			delay2++;
			/*if ((delay2 / tempFadeColorInterval) * 100 > 50)
			{
				fadeColorInterval += 1;
			}*/
			if (delay1 <= 0)
			{
				//fadeUp = !fadeUp;
				previousResetMicros = micros();
				action = 3;
				//fadeColorInterval = tempFadeColorInterval;
			}
		}
		else
		{
			delay1++;
			delay2--;
			/*if ((delay1 / tempFadeColorInterval) * 100 > 50)
			{
				fadeColorInterval += 1;
			}*/
			if (delay2 <= 0)
			{
				//fadeUp = !fadeUp;
				previousResetMicros = micros();
				action = 3;
				//fadeColorInterval = tempFadeColorInterval;
			}
		}
		previousFadeColorMicros = micros();
	}
}

void fadeColorReset()
{
	if (currentMicros - previousResetMicros >= 5000000L)
	{
		fadeUp = !fadeUp;
		action = 2;
	}
}

void setLightColor(bool value)
{
	if (value)
	{
		PORTB = B00100000;
	}
	else
	{
		PORTB = B00010000;
	}
}

void setLightBrightness(int value)
{
	analogWrite(lightsPinEnable, value);
}

void debugInfo()
{
	Serial.print(delay1);
	Serial.print("-");
	Serial.print(delay2);
	Serial.print("-");
	Serial.println(action);
}







//int colorInterval;
//int fadeInterval;
//
//// settings
//int action;
//int delay1;
//int delay2;
//bool powerState;
//bool colorState;
//int brightness;
//bool fadeUp;
//
//// the setup function runs once when you press reset or power the board
//void setup() {
//
//	// set settings to startup values (maybe read from eeprom)
//	action = 1;
//	delay1 = 1000;
//	delay2 = 1000;
//	powerState = true;
//	colorState = true;
//	brightness = 255;
//	fadeUp = true;
//
//	powerInterval = delay1;
//	colorInterval = delay1;
//	fadeInterval = 60;
//}
//
//// the loop function runs over and over again until power down or reset
//void loop() {
//	checkSerial();
//
//	currentMicros = micros();
//	setAction();
//
//	setLightBrightness(brightness);
//	setLightColor(colorState);	
//}