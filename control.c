#include "control.h"
#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

#define DEBUG
#define WIRINGPI

struct timeval tmp;
int valueLed = HIGH;

char str[512];
char num[10];

void control_inic()
{
#ifdef WIRINGPI
	wiringPiSetup();
	pinMode(LED_PIN, OUTPUT);

	pinMode(MOTOR1_PIN, PWM_OUTPUT);
	pinMode(MOTOR2_PIN, PWM_OUTPUT);

	softPwmCreate(MOTOR1_PIN, 0, 50);
	softPwmCreate(MOTOR2_PIN, 0, 50);
#endif
}

void forward()
{
#ifdef DEBUG
	fprintf(stderr, "Fordward action\n");
#endif
	softPwmWrite(MOTOR1_PIN, 20);
	softPwmWrite(MOTOR2_PIN, 8);

}

void backward()
{
#ifdef DEBUG
	fprintf(stderr, "Backward action\n");
#endif
	softPwmWrite(MOTOR1_PIN, 8);
	softPwmWrite(MOTOR2_PIN, 20);
}

void turn_left()
{
#ifdef DEBUG
	fprintf(stderr, "Turn left action\n");
#endif
	softPwmWrite(MOTOR1_PIN, 20);
	softPwmWrite(MOTOR2_PIN, 20);
}

void turn_right()
{
#ifdef DEBUG
	fprintf(stderr, "Turn right action\n");
#endif
	softPwmWrite(MOTOR1_PIN, 8);
	softPwmWrite(MOTOR2_PIN, 8);
}

void stop_motors()
{
#ifdef DEBUG
	fprintf(stderr, "Stop action\n");
#endif
	softPwmWrite(MOTOR1_PIN, 0);
	softPwmWrite(MOTOR2_PIN, 0);
}

void save_time()
{
	time4NextStop = now_in_msec() + MAX_TIME_BEFORE_STOP_MOTORS;
#ifdef DEBUG
	fprintf(stderr, "Proximo stop en %lu\n", time4NextStop);
#endif
}

long int now_in_msec()
{
	gettimeofday(&tmp, 0);
	return (tmp.tv_sec * 1000) + (long int)((double)tmp.tv_usec / 1000.0f);
}

void tostring(char str[], int num)
{
    int i, rem, len = 0, n;
 
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}

void executeAndReturn(const char *command, char *resul)
{
        FILE *fp;
        char buff[256];
	strcpy (resul, "");
        fp = popen(command, "r");
        if (fp == NULL)
                printf("Error: popen(%s...", command);

        while (fgets(buff, sizeof(buff), fp) != NULL)
                strcat(resul, buff);

        int status = pclose(fp);
        if (status == -1) {
                printf("Error: pclose");
        }
}

int proc_control(int command_id, int group, int value)
{
	int i;
	switch (command_id) 
	{
		case BLINK:
			valueLed = (valueLed == HIGH) ? LOW : HIGH;
#ifdef WIRINGPI
			digitalWrite(LED_PIN, valueLed);
#endif
#ifdef DEBUG
			fprintf(stderr, "LED: %i\n", valueLed);
#endif
			return valueLed;
#ifdef WIRINGPI
		case MOTOR1:
			softPwmWrite(MOTOR1_PIN, value);
			save_time();
			break;
		case MOTOR2:
			softPwmWrite(MOTOR2_PIN, value);
			save_time();
			break;
		case STOP_MOTORS:
			stop_motors();
			time4NextStop = LONG_MAX;
#ifdef DEBUG
			fprintf(stderr, "STOP MOTORS: %i\n", value);
#endif
			break;
		case TEST_MOTORS:
#ifdef DEBUG
			fprintf(stderr, "test motors action\n");
#endif
			for (i = 0; i < 50; i++)
			{
				softPwmWrite(MOTOR1_PIN, i);
				softPwmWrite(MOTOR2_PIN, i);
				delay(50);
			}
			delay(100);
			stop_motors();
			break;
		case FORWARD:
			forward();
			save_time();
			break;
		case BACKWARD: 
			backward();
			save_time();
			break;
		case TURN_LEFT:
			turn_left();
			save_time();
			break;
		case TURN_RIGHT:
			turn_right();
			save_time();
			break;
#endif
		case LED_ON:
			valueLed = HIGH;
#ifdef WIRINGPI
			digitalWrite(LED_PIN, valueLed);
#endif
#ifdef DEBUG
			fprintf(stderr, "LED: %i\n", valueLed);
#endif
			break;
		default:
			fprintf(stderr, "Received command \"%i\" with value \"%i\", command aren't recognized. The command are likely that don't implement yet.\n", command_id, value);
	}

	return command_id;
}
