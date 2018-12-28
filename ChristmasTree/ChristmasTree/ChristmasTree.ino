/*
 Name:		ChristmasTree.ino
 Created:	12/28/2018 10:00:54 AM
 Author:	JasonSpringart
*/

// pin mapping
int lightsLineA = 1;
int lightsLineB = 2;
int lightsLineEnable = 13;

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);

	pinMode(lightsLineA, OUTPUT);
	pinMode(lightsLineB, OUTPUT);
	pinMode(lightsLineEnable, OUTPUT);
}

// the loop function runs over and over again until power down or reset
void loop() {
	checkSerial();
}

void checkSerial() {

}

void splitSerialData() {

}

void parseSerialData() {

}

void setLightPower() {

}

void setLightColor() {

}

void setLightBrightness() {

}