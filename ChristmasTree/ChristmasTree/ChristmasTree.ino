/*
 Name:		ChristmasTree.ino
 Created:	12/28/2018 10:00:54 AM
 Author:	Jason Springart
*/

enum STATE {
	STATIC_WHITE,
	STATIC_MULTI,
	FADE_WHITE,
	FADE_MULTI,
	FADE_SEQ,
	FADE_BOTH,
	FADE_BURST,
	OFF
};

enum COLOR {
	WHITE,
	MULTI,
	BOTH
};

volatile uint8_t *dutyCyclePointer;

unsigned long currentMicros;

unsigned long previousStateMicros;
unsigned long stateInterval;

unsigned long previousDebugMicros;
unsigned long debugInterval;

STATE machineState;
COLOR ledColor;
int brightness;


// ?testing
bool fadeUp;

void setup() {
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);

	TCCR0A = 0; //reset the register
	TCCR0B = 0; //reset the register

	TCCR0A = 0b10100011; // fast pwm mode
	TCCR0B = 0b00000011; // prescaler 64

	// ?Figure out which pins are which colors
	OCR0A = 0; //duty cycle for pin 6
	OCR0B = 0; //duty cycle for pin 5
	dutyCyclePointer = &OCR0A;

	stateInterval = 500000;
	debugInterval = 500000;
	machineState = FADE_BURST;
	ledColor = WHITE;
	brightness = 0;
	//fadeUp = true;

	// load startup values
	startup();

	Serial.begin(9600);	
}

void startup() {
	switch (machineState) {
	case STATIC_WHITE:
		ledColor = WHITE;
		brightness = 255;
		break;

	case STATIC_MULTI:
		ledColor = MULTI;
		brightness = 255;
		break;

	case FADE_BURST:
		ledColor = WHITE;
		brightness = 0;
		stateInterval = 8000;
		break;

	case OFF:
		brightness = 0;
		break;

	default:
		break;
	}
}

void loop() {
	currentMicros = micros();
	checkSerial();

	if (currentMicros - previousStateMicros >= stateInterval) {
		stateMachine();
		previousStateMicros = micros();
	}	

	if (currentMicros - previousDebugMicros >= debugInterval) {
		displayDebugInfo();
		previousDebugMicros = micros();
	}
}

void displayDebugInfo() {
	String debugInfo = "DEBUG: ";

	debugInfo += "SINT=" + String(stateInterval) + " ";
	debugInfo += "DINT=" + String(debugInterval) + " ";
	debugInfo += "STAT=" + String(machineState) + " ";
	debugInfo += "COLR=" + String(ledColor) + " ";
	debugInfo += "BRIT=" + String(brightness) + " ";

	Serial.println(debugInfo);
}

void checkSerial() {
	String data = "";
	while (Serial.available() > 0) {
		data += Serial.readString();
		if (data.endsWith("\n")) {
			data.replace("\n", "");			
		}
		splitSerialData(data);
	}
}

void splitSerialData(String data) {
	String tempData = data;
	tempData.trim();
	while (tempData.length() > 0) {
		int index = tempData.indexOf(";");
		String subString = "";
		if (index == 0 || index == -1) {			
			subString = tempData.substring(0);
		}
		else {
			subString = tempData.substring(0, index + 1);
		}
		tempData.replace(subString, "");
		parseSerialData(subString);
	}
}

void parseSerialData(String data) {	
	String command = data.substring(0, 2);
	String value = data.substring(2);
	if (command == "br") {
		brightness = atoi(value.c_str());
	}
	else if (command == "st") {
		machineState = (STATE)atoi(value.c_str());
	}
	else if (command == "si") {
		stateInterval = value.toDouble() * 1000000;
	}
	else if (command == "di") {
		debugInterval = value.toDouble() * 1000000;
	}
//	if (data.substring(0, 2) == "sh") {
//		int show = atoi(data.substring(2).c_str());
//		switch (show)
//		{
//		case 0:
//			action = STATIC_WHITE;
//			startup();
//			break;
//
//		case 1:
//			action = STATIC_MULTI;
//			startup();
//			break;
//
//		case 2:
//			action = FADE_WHITE;
//			startup();
//			break;
//
//		case 3:
//			action = FADE_MULTI;
//			startup();
//			break;
//
//		case 4:
//			action = FADE_SEQ_BOTH;
//			startup();
//			break;
//
//		case 5:
//			action = FADE_BOTH;
//			startup();
//			break;
//
//		case 6:
//			action = SHOW_OFF;
//			startup();
//			break;
//		}
//	}
}

void stateMachine() {
	switch (machineState) {
	case STATIC_WHITE:
		setLedState();
		break;

	case STATIC_MULTI:
		setLedState();
		break;

	case FADE_BURST:
		fadeBurst();
		break;

	case OFF:
		offState();
		break;

	default:
		break;
	}
}

void setLedState() {
	switch (ledColor) {
	case WHITE:
		OCR0A = brightness;
		OCR0B = 0;
		break;

	case MULTI:
		OCR0A = 0;
		OCR0B = brightness;
		break;

	default:
		offState();
		break;
	}
}

void offState() {
	OCR0A = 0;
	OCR0B = 0;
}

bool pause = false;
int pauseCounter = 0;
int pauseDelay = 100;
int pauseDelay1 = 200;
int pauseDelay2 = 100;
void fadeBurst() {
	/*if ((*dutyCyclePointer) == 255) {
		(*dutyCyclePointer) = 0;
		if (ledColor == WHITE) {
			ledColor = MULTI;
		}
		else {
			ledColor = WHITE;
		}
	}
	(*dutyCyclePointer)++;	*/
	if (pause) {
		if (pauseCounter >= pauseDelay) {
			pause = false;
			pauseCounter = 0;
		}
		else {
			pauseCounter++;
		}		
	}
	else {
		if (brightness == 255) {
			brightness = 1;
			if (ledColor == WHITE) {
				ledColor = MULTI;
				//pauseDelay = pauseDelay1;
			}
			else {
				ledColor = WHITE;
				//pauseDelay = pauseDelay2;
			}
			pause = true;
		}		
		setLedState();
		brightness++;
	}	
}





void setColor()
{
	/*if (color)
	{
		dutyCyclePointer = &OCR0A;
		color = false;
	}
	else
	{
		dutyCyclePointer = &OCR0B;
		color = true;
	}*/
}