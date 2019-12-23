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
	OFF,
	PAUSE
};

enum COLOR {
	WHITE,
	MULTI,
	BOTH
};

void changeState(STATE newState);

volatile uint8_t *dutyCyclePointer;

unsigned long currentMicros;

unsigned long previousStateMicros;
unsigned long stateInterval;

unsigned long previousDebugMicros;
unsigned long debugInterval;
bool debug = false;

STATE machineState;
STATE previousState;
COLOR ledColor;
int brightness;
int fadeUp;
int fadeCounter;
int bright1;
int bright2;

bool paused = false;
int pauseCounter = 0;
int pauseDelay;
int pauseDelay1 = 10;
int pauseDelay2 = 250;
int pauseDelay3 = 1;

//int counter;
unsigned long pauseBothCounter;
unsigned long pauseBothDelay;
bool pauseBothBit = false;

void setup() {
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);

	TCCR0A = 0; //reset the register
	TCCR0B = 0; //reset the register

	TCCR0A = 0b10100011; // fast pwm mode
	TCCR0B = 0b00000011; // prescaler 64

	OCR0A = 0; //duty cycle for pin 6
	OCR0B = 0; //duty cycle for pin 5
	dutyCyclePointer = &OCR0A;

	stateInterval = 500000;
	debugInterval = 500000;
	machineState = FADE_BOTH;
	ledColor = WHITE;
	brightness = 0;

	// TODO: load startup values from Eeprom
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

	case FADE_WHITE:
		pauseDelay1 = 100;
		pauseDelay2 = 50;
		ledColor = WHITE;
		brightness = 0;
		fadeUp = true;
		fadeCounter = 0;
		break;

	case FADE_MULTI:
		pauseDelay1 = 100;
		pauseDelay2 = 50;
		ledColor = MULTI;
		brightness = 0;
		fadeUp = true;
		fadeCounter = 0;
		break;

	case FADE_SEQ:
		pauseDelay1 = 25;
		pauseDelay2 = 100;
		ledColor = WHITE;
		brightness = 0;
		fadeUp = true;
		fadeCounter = 0;
		break;

	case FADE_BOTH:
		bright1 = 255;
		bright2 = 255;
		brightness = 255;
		pauseDelay1 = 0;
		pauseDelay2 = 300;
		stateInterval = 5;
		ledColor = MULTI;
		pauseBothCounter = 0;
		pauseBothBit = false;
		break;

	case FADE_BURST:
		pauseDelay1 = 10;
		pauseDelay2 = 250;
		ledColor = WHITE;
		brightness = 255;
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
		if (debug) {
			displayDebugInfo();
		}
		previousDebugMicros = micros();
	}
}

void displayDebugInfo() {
	String debugInfo = "DEBUG: ";

	debugInfo += "SI=" + String(stateInterval) + " ";
	debugInfo += "DI=" + String(debugInterval) + " ";
	debugInfo += "ST=" + String(machineState) + " ";
	debugInfo += "CO=" + String(ledColor) + " ";
	debugInfo += "BR=" + String(brightness) + " ";
	debugInfo += "PC=" + String(pauseCounter) + " ";
	debugInfo += "PD=" + String(pauseDelay) + " ";
	debugInfo += "P1=" + String(pauseDelay1) + " ";
	debugInfo += "P2=" + String(pauseDelay2) + " ";
	debugInfo += "P3=" + String(pauseDelay3) + " ";
	debugInfo += "FC=" + String(fadeCounter) + " ";
	debugInfo += "B1=" + String(bright1) + " ";
	debugInfo += "B2=" + String(bright2) + " ";
	debugInfo += "PBC=" + String(pauseBothCounter) + " ";
	debugInfo += "PBD=" + String(pauseBothDelay) + " ";

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
		brightness = value.toInt();
	}
	else if (command == "st") {
		machineState = (STATE)value.toInt();
		startup();
	}
	else if (command == "si") {
		stateInterval = value.toDouble() * 1000000;
	}
	else if (command == "di") {
		debugInterval = value.toDouble() * 1000000;
	}
	else if (command == "p1") {
		pauseDelay1 = value.toInt();
	}
	else if (command == "p2") {
		pauseDelay2 = value.toInt();
	}
	else if (command == "p3") {
		pauseDelay3 = value.toInt();
	}
	else if (command == "b1") {
		bright1 = value.toInt();
	}
	else if (command == "b2") {
		bright2 = value.toInt();
	}
}

void stateMachine() {
	switch (machineState) {
	case STATIC_WHITE:
		setLedState();
		break;

	case STATIC_MULTI:
		setLedState();
		break;

	case FADE_WHITE:
		fade();
		break;

	case FADE_MULTI:
		fade();
		break;

	case FADE_SEQ:
		fadeSeq();
		fade();
		break;

	case FADE_BOTH:
		fadeBoth();
		break;

	case FADE_BURST:
		fadeBurst();
		break;	

	case OFF:
		offState();
		break;

	case PAUSE:
		pause();
		break;

	default:
		break;
	}
}

void setLedState() {
	switch (ledColor) {
	case WHITE:
		OCR0A = 0;
		OCR0B = brightness;
		break;

	case MULTI:
		OCR0A = brightness;
		OCR0B = 0;
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

void fadeBurst() {
	if (brightness == 255)
	{
		pauseDelay = pauseDelay2;
		changeState(PAUSE);
	}
	if (brightness == 0) {
		pauseDelay = pauseDelay1;
		changeState(PAUSE);
		brightness = 255;
		if (ledColor == WHITE) {
			ledColor = MULTI;
		}
		else {
			ledColor = WHITE;
		}
		return;
	}
	else {
		brightness--;
	}
	setLedState();
}

void fade() {
	if (fadeUp)
	{
		if (brightness == 255) {
			fadeUp = false;
			fadeCounter++;
			pauseDelay = pauseDelay2;
			changeState(PAUSE);
		}
		else {
			brightness++;
		}
	}
	else {
		if (brightness == 0) {
			fadeUp = true;
			fadeCounter++;
			pauseDelay = pauseDelay1;
			changeState(PAUSE);
		}
		else {
			brightness--;
		}
	}
	setLedState();
}

void fadeSeq() {
	if (fadeCounter == 2) {
		fadeCounter = 0;
		if (ledColor == WHITE) {
			ledColor = MULTI;
		}
		else {
			ledColor = WHITE;
		}
	}
}



void fadeBoth() {
	
	if (!pauseBothBit) {
		flipColor();
		if (fadeUp) {
			if (pauseDelay1 == 300) {
				fadeUp = false;
				fadeCounter++;
				pauseBothBit = true;
				pauseBothDelay = 50000;
				return;
			}
			else {
				pauseDelay1++;
				pauseDelay2--;
				//pauseDelay = pauseDelay3;
				//changeState(PAUSE);		
			}
		}
		else {
			if (pauseDelay1 == 0) {
				fadeUp = true;
				fadeCounter++;
				pauseBothBit = true;
				pauseBothDelay = 50000;
				return;
				//for (int i = 0; i < 30000; i++)
				//{
				//	counter++;
				//	//flipColor();
				//}
			}
			else {
				pauseDelay1--;
				pauseDelay2++;				
				
				//pauseDelay = pauseDelay3;
				//changeState(PAUSE);		
			}
		}
	}
	else {
		pauseBoth();
	}	

	//setLedState();	
}

void pause() {
	if (pauseCounter >= pauseDelay) {
		machineState = previousState;
		pauseCounter = 0;
	}
	else {
		pauseCounter++;
	}
}

void flipColor() {
	changeState(PAUSE);
	
	if (ledColor == WHITE) {
		ledColor = MULTI;
		pauseDelay = pauseDelay1;
	}
	else {
		ledColor = WHITE;
		pauseDelay = pauseDelay2;
	}
	setLedState();	
}

void pauseBoth() {
	if (pauseBothCounter >= pauseBothDelay) {
		pauseBothCounter = 0;
		pauseBothBit = false;
	}
	else {
		pauseBothCounter++;
	}
	//flipColor();
}

void changeState(STATE newState) {
	if (machineState != PAUSE || newState != PAUSE) {
		previousState = machineState;
		machineState = newState;
	}	
}