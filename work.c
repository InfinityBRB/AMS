#include <stdio.h>
#include <regc515c.h>
#include <stub.h>

#define WHITE 110

/* states */
#define STATE_PENDING			(unsigned char) 0
#define STATE_FORWARD_NORMAL	(unsigned char) 1
#define STATE_FORWARD_BLIND		(unsigned char) 2
#define STATE_FORWARD_TYRE		(unsigned char) 3
/*
#define STATE_TURN_LEFT_START	(unsigned char) 4
#define STATE_TURN_RIGHT_START	(unsigned char) 5
#define STATE_TURN_OUTER_START	(unsigned char) 6
#define STATE_TURN_INNER_START	(unsigned char) 7
#define STATE_TURN_LEFT			(unsigned char) 8
#define STATE_TURN_RIGHT		(unsigned char) 9
#define STATE_TURN_OUTER		(unsigned char) 10
#define STATE_TURN_INNER		(unsigned char) 11
*/
#define STATE_TURN_START		(unsigned char) 4
#define STATE_TURN				(unsigned char) 5
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

/* TURN for navigation sys */
#define TURN_OUTER (signed char) 1
#define TURN_NONE  (signed char) 0
#define TURN_INNER (signed char) -1

#define SENSOR_NUMBER 9

/* runtime in milliseconds - for the game: 120000 */
#define RUNTIME 7500

/* motor idetifier */
#define MOTOR_RIGHT 0
#define MOTOR_LEFT  1

unsigned char state = STATE_PENDING;
unsigned char navPoint = NAV_START;
unsigned char navLine = 1;
/* initiating params */
unsigned char motorPwr[2] = {0, 0};
unsigned char motorDir[2] = {0, 0};

unsigned char normalSpeed = 0;

unsigned char sensor[SENSOR_NUMBER];
unsigned char *innerSen, *outerSen;

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
		motor_pwm(motor, normalSpeed - 5);
		motorPwr[motor] = normalSpeed - 5;
	}
}

void clear_correction(void) {
	motor_pwm(0, normalSpeed);
	motor_pwm(1, normalSpeed);
	motorPwr[0] = normalSpeed;
	motorPwr[1] = normalSpeed;
}

unsigned char isTwoOfThreeBlack(unsigned char *sens) {
	return ((IS_BLACK(sens[0]) && IS_BLACK(sens[1])) || (IS_BLACK(sens[0]) && IS_BLACK(sens[2])) || (IS_BLACK(sens[2]) && IS_BLACK(sens[1])));
}

signed char getNav(void) {
	if (navLine) {
		if (IS_WHITE(innerSen[0]) && IS_WHITE(innerSen[1]) && IS_WHITE(innerSen[2]) && IS_WHITE(outerSen[0]) && IS_WHITE(outerSen[1]) && IS_WHITE(outerSen[2])) {
			navLine = 0;
		}
		return TURN_NONE;
	}
	if (navPoint == NAV_START) {
		if (isTwoOfThreeBlack(outerSen)) {
			navLine = 1;
			navPoint++;
			return TURN_OUTER;
		}
	}
	return TURN_NONE;
}

void AksenMain(void)
{
	unsigned long timeoutAt;
	unsigned char innerMotor, outerMotor;
	signed char turn;
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
		innerMotor = 1;
		outerMotor = 0;
		innerSen = sensor + 6;
		outerSen = sensor;
	} else if (IS_BLACK(sensor[6]) && IS_BLACK(sensor[7]) && IS_BLACK(sensor[8])) {
		lcd_puts("links");
		innerMotor = 0;
		outerMotor = 1;
		innerSen = sensor;
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
	//while(digital_in(0) == 1);
	/* für licht: */
	//do{ updateSensors(); } while(IS_BLACK(sensor[3]) || IS_BLACK(sensor[4]) || IS_BLACK(sensor[5]));
	/* für schalter oder licht */
	do{ updateSensors(); } while((IS_BLACK(sensor[3]) || IS_BLACK(sensor[4]) || IS_BLACK(sensor[5])) && (digital_in(0) == 1));
	
	/*****************/
	/* startsequence */
	/*****************/
	timeoutAt = akt_time() + RUNTIME;
	normalSpeed = 10;
	motor_pwm(0, normalSpeed);
	motor_pwm(1, normalSpeed);
	motorPwr[0] = normalSpeed;
	motorPwr[1] = normalSpeed;
	state = STATE_FORWARD_NORMAL;
	turn = 0;
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
		
		if (state == STATE_FORWARD_NORMAL) {
			
			turn = getNav();
			
			if (turn == TURN_OUTER) {
				//state = STATE_TURN_OUTER_START;
				state = STATE_TURN_START;
				motor_pwm(outerMotor, 0);
				motorPwr[outerMotor] = 0;
				motor_pwm(innerMotor, 4);
				motorPwr[innerMotor] = 4;
				continue;
			}
			
			if (IS_BLACK(sensor[4]) && IS_WHITE(sensor[3]) && IS_WHITE(sensor[5])) {
				clear_correction();
			} else if (IS_BLACK(sensor[3])) {
				correction(0);
			} else if (IS_BLACK(sensor[5])) {
				correction(1);
			}
			continue;
		}
		
		/*if (state == STATE_TURN_OUTER_START) {
			if (IS_WHITE(sensor[3]) && IS_WHITE(sensor[4]) && IS_WHITE(sensor[5])) {
				state = STATE_TURN_OUTER;
			}
			continue;
		}
		
		if (state == STATE_TURN_INNER_START) {
			if (IS_WHITE(sensor[3]) && IS_WHITE(sensor[4]) && IS_WHITE(sensor[5])) {
				state = STATE_TURN_INNER;
			}
			continue;
		}
		
		if (state == STATE_TURN_INNER) {
			if (IS_BLACK(sensor[3]) || IS_BLACK(sensor[4]) || IS_BLACK(sensor[5])) {
				clear_correction();
			}
		}*/
		
		if (state == STATE_TURN_START) {
			if (IS_WHITE(sensor[3]) && IS_WHITE(sensor[4]) && IS_WHITE(sensor[5])) {
				state = STATE_TURN;
			}
			continue;
		}
		
		if (state == STATE_TURN) {
			if (IS_BLACK(sensor[3]) || IS_BLACK(sensor[4]) || IS_BLACK(sensor[5])) {
				clear_correction();
			}
			continue;
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