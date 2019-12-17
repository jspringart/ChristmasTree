/*
 Name:		ChristmasTree.ino
 Created:	12/28/2018 10:00:54 AM
 Author:	Jason Springart
*/

void setup() {
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
	TCCR0A = 0;//reset the register
	TCCR0B = 0;//reset tthe register
	TCCR0A = 0b10100011;// fast pwm mode
	TCCR0B = 0b00000011;// prescaler 64
	OCR0A = 50;//duty cycle for pin 6
	OCR0B = 50;//duty cycle for pin 5
}

void loop() {

}