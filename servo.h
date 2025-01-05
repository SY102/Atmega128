#ifndef SERVO_H
#define SERVO_H

#include <avr/io.h>
#include <util/delay.h>

#define SERVO_PIN 3 // 서보 모터 연결 핀

void init_servo();        // 서보 모터 초기화
void servoStop();        // 서보 모터 정지
void servoStart();       // 서보 모터 시작
void move_servo_fast();   // 서보 모터 빠르게 움직이기

#endif // SERVO_H
