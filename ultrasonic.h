/*
 * ultrasonic.h
 *
 * Created: 2024-12-05 오전 4:56:07
 *  Author: xodrb
 */ 


#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#define TRIG 6		//트리거 PC6번
#define ECHO 7		//에코 PC7번
#define SOUND_VELOCITY 340UL		//초음파센서 속도


void trigger_sensor();
unsigned int measure_distance();




#endif /* ULTRASONIC_H_ */