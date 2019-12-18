/*
 Name:		ChristmasTree.ino
 Created:	12/28/2018 10:00:54 AM
 Author:	Jason Springart
*/

volatile uint8_t *dutyCyclePointer;

unsigned long currentMicros;
unsigned long previousMicros;
unsigned long interval;

bool fadeUp;
//int brightness; // 0 - 255
bool color = true;

void setup() {
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
	pinMode(13, INPUT);

	TCCR0A = 0;//reset the register
	TCCR0B = 0;//reset the register

	TCCR0A = 0b10100011;// fast pwm mode
	TCCR0B = 0b00000011;// prescaler 64

	OCR0A = 255;//duty cycle for pin 6
	OCR0B = 255;//duty cycle for pin 5
	dutyCyclePointer = &OCR0A;

	interval = 500000;
	fadeUp = true;
}


void loop() {
	currentMicros = micros();

	if (currentMicros - previousMicros >= interval) {
		stateMachine();
		previousMicros = micros();
	}	
}

byte test;
int counter = 0;
void stateMachine() {
	if ((*dutyCyclePointer) == 0) {
		(*dutyCyclePointer) = 255;
		setColor();
	}
	(*dutyCyclePointer)--;
}

void setColor()
{
	if (color)
	{
		dutyCyclePointer = &OCR0A;
		color = false;
	}
	else
	{
		dutyCyclePointer = &OCR0B;
		color = true;
	}
}