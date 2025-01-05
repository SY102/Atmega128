#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "ultrasonic.h"



void trigger_sensor() {
	TCCR1B = 0x03;
	PORTC &= ~(1 << TRIG);
	_delay_us(10);
	PORTC |= (1 << TRIG);
	_delay_us(10);
	PORTC &= ~(1 << TRIG);
}

unsigned int measure_distance() {
	trigger_sensor();
	
	while (!(PINC & (1 << ECHO)));
	TCNT1 = 0x0000;
	while (PINC & (1 << ECHO));
	TCCR1B = 0x00;

	return (unsigned int)(SOUND_VELOCITY * (TCNT1 * 4 / 2) / 1000);
}