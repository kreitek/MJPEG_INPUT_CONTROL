#ifndef __CONTROL_H
#define __CONTROL_H

#include <limits.h>

#define LED_PIN 0
#define MOTOR1_PIN 1
#define MOTOR2_PIN 11

#define BLINK 0
#define MOTOR1 1
#define MOTOR2 2
#define STOP_MOTORS 3
#define TEST_MOTORS 4
#define BACKWARD 5
#define FORWARD 6
#define TURN_LEFT 7
#define TURN_RIGHT 8
#define LED_ON 9

#define MAX_TIME_BEFORE_STOP_MOTORS ((long int)1000)

long int time4NextStop = LONG_MAX;

long int now_in_msec();
void control_inic();
int proc_control(int command_id, int group, int value);

#endif
