#include <stdio.h>
#include <regc515c.h>
#include <stub.h>

#define WHITE 110

/* states */
#define STATE_PENDING			(unsigned char) 0
#define STATE_FORWARD_NORMAL	(unsigned char) 1
#define STATE_FORWARD_BLIND		(unsigned char) 2
#define STATE_FORWARD_TYRE		(unsigned char) 3
#define STATE_TURN_LEFT			(unsigned char) 4
#define STATE_TURN RIGHT		(unsigned char) 5
#define STATE_TIMEOUT			(unsigned char) 0xFF

/* Nav-Points */
#define NAV_START 	(unsigned char) 0
#define NAV_START1 	(unsigned char) 1
#define NAV_START2 	(unsigned char) 2
#define NAV_START3 	(unsigned char) 3
#define NAV_A 		(unsigned char) 4
#define NAV_A1 		(unsigned char) 5
#define NAV_B 		(unsigned char) 6
#define NAV_B1 		(unsigned char) 7
#define NAV_C		(unsigned char) 8
#define NAV_C1		(unsigned char) 9
#define NAV_D		(unsigned char) 10
#define NAV_D1		(unsigned char) 11
#define NAV_E		(unsigned char) 12
#define NAV_E1		(unsigned char) 13
#define NAV_F		(unsigned char) 14
#define NAV_F1		(unsigned char) 15
#define NAV_F2		(unsigned char) 16
#define NAV_F3		(unsigned char) 17

#define SENSOR_NUMBER 9

/* runtime in milliseconds - for the game: 120000 */
#define RUNTIME 2000

unsigned char state = STATE_PENDING;
unsigned char navPoint = NAV_START;
/* initiating params */
unsigned char motorPwr[2] = {0, 0};
unsigned char motorDir[2] = {0, 0};

unsigned char normalSpeed = 0;

unsigned char sensor[SENSOR_NUMBER];

#define IS_WHITE(x) ((x) == 0)
#define IS_BLACK(x) ((x) == 1)

void stop(void)
{
	motorDir[0] = 1- motorDir[0];
	motorDir[1] = 1- motorDir[1];
	motor_richtung(0, motorDir[0]);
	motor_richtung(1, motorDir[1]);
	sleep(50);
	motor_pwm(0, 0);
	motor_pwm(1, 0);
	motorPwr[0, 0];
	motorPwr[1, 0];
}

void updateSensors(void)
{
	int i;
	for (i = 0; i < SENSOR_NUMBER; i++) {
		sensor[i] = (analog(i) > WHITE);
	}
}

void correction(unsigned char motor) {
	unsigned char other = 1 - motor;
	if (motorPwr[other] < normalSpeed) {
		motor_pwm(other, normalSpeed);
		motorPwr[other] = normalSpeed;
	} else if (motorPwr[motor] == normalSpeed) {
		motor_pwm(motor, normalSpeed - 1);
		motorPwr[motor] = normalSpeed - 1);
	}
}

void clear_correction(void) {
	motor_pwm(0, normalSpeed);
	motor_pwm(1, normalSpeed);
	motorPwr[0] = normalSpeed;
	motorPwr[1] = normalSpeed;
}

void AksenMain(void)
{
	unsigned long timeoutAt;
	unsigned char *innerSen, *outerSen;
	unsigned char innerMotor, outerMotor;
	lcd_puts("Seite: ");
	/* setting defaults */
	motor_richtung(0, motorDir[0]);
	motor_richtung(1, motorDir[1]);
	motor_pwm(0, motorPwr[0]);
	motor_pwm(1, motorPwr[1]);
	updateSensors();
	/**********************/
	/* Richtungserkennung */
	/**********************/
	if (IS_BLACK(sensor[0]) && IS_BLACK(sensor[1]) && IS_BLACK(sensor[2])) {
		lcd_puts("rechts");
		innerMotor = 0;
		outerMotor = 1;
		innerSen = sensor + 6;
		outerSen = sensor;
	} else if (IS_BLACK(sensor[6]) && IS_BLACK(sensor[7]) && IS_BLACK(sensor[8])) {
		lcd_puts("links");
		innerMotor = 1;
		outerMotor = 0;
		innerSen = sensor
		outerSen = sensor + 6;
	} else {
		lcd_cls();
		lcd_puts("ERROR: Seite");
		lcd_setxy(1,0);
		lcd_puts("nicht erkannt");
		while(1);
	}
	/*if (dip_pin(0)) { // dip0 auf 1 // LINKS
		lcd_puts("links");
		innerSen = sensor
		outerSen = sensor + 6;
	} else {
		lcd_puts("rechts");
		innerSen = sensor + 6;
		outerSen = sensor;
	}*/
	/*************************/
	/* warte auf startsignal */
	/*************************/
	/* für sofort-Start: */
	//while(0);
	/* für schalter-start: */
	while(digital_in(0) == 0);
	/* für realstart: */
	//do{ updateSensors(); } while(IS_BLACK(sensor[3]) || IS_BLACK(sensor[4]) || IS_BLACK(sensor[5]));
	
	/*****************/
	/* startsequence */
	/*****************/
	timeoutAt = akt_time() + RUNTIME;
	normalSpeed = 4;
	motor_pwm(0, normalSpeed);
	motor_pwm(1, normalSpeed);
	motorPwr(0, normalSpeed);
	motorPwr(1, normalSpeed);
	state = STATE_FORWARD_NORMAL;
	/*************/
	/* Main Loop */
	/*************/
	while(1) {
		updateSensors();
		if (akt_time() >= timeoutAt) { // programm ende
			stop();
			state = STATE_TIMEOUT;
			break;
		}
		
		if (STATE_FORWARD_NORMAL) {
			if (IS_BLACK(sensor[3]) && IS_BLACK(sensor[4]) && IS_BLACK(sensor[5])) {
				//kurve?
				if (IS_BLACK(innerSen[0]) || IS_BLACK(innerSen[1]) || IS_BLACK(innerSen[2])) {
					
				}
			}
			
			if (IS_BLACK(sensor[4]) && IS_WHITE(sensor[3]) && IS_WHITE(sensor[5]) {
				clear_correction();
			} else if (IS_BLACK(sensor[3])) {
				correction(0);
			} else if (IS_BLACK(sensor[5])) {
				correction(1);
			}
		}
		
	}
	/********************/
	/* user information */
	/********************/
	lcd_cls();
	lcd_setxy(0,0);
	lcd_puts("Programm Ende");
	/*********************/
	/* Freezing Programm */
	/*********************/
	while(1);
}