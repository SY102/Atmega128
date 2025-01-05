
#define F_CPU 16000000ul
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "i2c.h"
#include "clcd.h"
#include "buzzer.h"
#include "stdlib.h"
#include "servo.h"
#include "ultrasonic.h"


#define WARNING_DISTANCE 100
#define STEP_BACK_DISTANCE 200
#define DANGER_ZONE_DISTANCE 300
#define DARK_THRESHOLD   341  // 어두운 상태 기준

volatile uint8_t mode = 0;

void init_adc() {
	ADMUX = 0x40;  // AVCC 기준 전압, ADC0 채널 사용
	ADCSRA = 0x87;
	DDRF = 0x00;  // PORTF를 입력으로 설정 (ADC 입력) // ADC 활성화, 프리스케일러: 128
}


void security_mode(unsigned int distance) {
	if (distance < WARNING_DISTANCE) {
		servoStop();
		PORTA = 0b00000001;
		S_Error();
		display_message("!FINAL WARNING!", "!BACK OFF!");
		} else if (distance <= STEP_BACK_DISTANCE) {
		servoStop();
		PORTA = 0b00000100;
		S_S4();
		display_message("STEP BACK!", "WARNING");
		} else if (distance <= DANGER_ZONE_DISTANCE) {
		servoStop();
		PORTA = 0b00010000;
		S_Start();
		display_message("DANGER ZONE", "");
		} else {
		servoStart();
		move_servo_fast();
		PORTA = 0b01000000;
		display_message("HELLO!!!", "");
	}
}



void welcome_mode(unsigned int distance) {
	if (distance <= WARNING_DISTANCE ) {
		servoStop();
		PORTA = 0xFF;
		_delay_ms(500);
		PORTA = 0x00;
		_delay_ms(500);
		S_Start();
		display_message("Welcome!!!", "");
		} else {
		servoStart();
		move_servo_fast();
		PORTA = 0b01000000;
		display_message("HELLO!!!", "");
	}
}

unsigned short read_adc() {
	ADCSRA |= (1 << ADSC);
	while (!(ADCSRA & (1 << ADIF)));
	ADCSRA |= (1 << ADIF);

	return (ADCL | (ADCH << 8));
}


void first_meet1(){
	char i;
	for(i=0; i<=7; i++){
		PORTA = 0x01 << i;
		_delay_ms(100);
	}
}

void first_meet2(){
	char i;
	for(i=0; i<=7; i++){
		PORTA = 0x80 >> i;
		_delay_ms(100);
	}
}



ISR(INT4_vect){
	mode = 1;
}

ISR(INT5_vect){
	mode = 2;
}



int main(void) {
	unsigned int distance;
	unsigned int adc_value;
	
	sei(); // 전역 인터럽트 활성화

	
	i2c_lcd_init();
	init_adc();
	init_servo();
	
	
	EIMSK = (1 << INT4) | (1 << INT5);
	EICRB = (1 << ISC41) | (1 << ISC51);

	DDRA = 0xFF;
	DDRC = ((DDRC | (1 << TRIG)) & ~(1 << ECHO));
	DDRB = (1 << Buzzer_Pin);
	
	init_servo();

	while (1) {
		switch (mode) {
			case 1:
			adc_value = read_adc();
			distance = measure_distance();
			if (adc_value <= DARK_THRESHOLD) {
				security_mode(distance);          //밤일땐 도난방지모드
				} else {
				welcome_mode(distance);           //낮일땐 환영모드
			}
			break;

			case 2:
			PORTA = 0xFF;
			distance = measure_distance();
			display_distance(distance);
			_delay_ms(100);
			break;
			
			default:
			first_meet1();
			_delay_ms(100);
			first_meet2();
			_delay_ms(100);
			break;
		}
	}

	return 0;
}
