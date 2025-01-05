#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "servo.h"

volatile int servo_active = 1;

void init_servo() {
	DDRE |= (1 << SERVO_PIN);
	TCCR3A = 0x82;
	TCCR3B = 0x1B;
	ICR3 = 4999;
}

void servoStop() {
	if (servo_active) {
		TCCR3A &= ~((1 << COM3A1) | (1 << COM3A0));
		servo_active = 0;
	}
}

void servoStart() {
	if (!servo_active) {
		TCCR3A |= (1 << COM3A1);
		servo_active = 1;
	}
}

void move_servo_fast() {
	static int direction = 1;
	static int servo_position = 150;
	
	servo_position += (direction * 10);
	
	if (servo_position >= 630) {
		direction = -1;
		servo_position = 630;
		} else if (servo_position <= 150) {
		direction = 1;
		servo_position = 150;
	}
	OCR3A = servo_position;
	_delay_ms(100);
}