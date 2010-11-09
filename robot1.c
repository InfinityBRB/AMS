#include <stdio.h>
#include <regc515c.h>
#include <stub.h>

#define WHITE 180

/* states */
#define STATE_PENDING 		(unsigned char) 0
#define STATE_FORWARD 		(unsigned char) 1
#define STATE_TURN_LEFT 	(unsigned char) 2
#define STATE_TURN RIGHT 	(unsigned char) 3
#define STATE_TIMEOUT 		(unsigned char) 0xFF

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

unsigned char state = STATE_PENDING;
unsigned char navPoint = NAV_START;
/* initiating params */
unsigned char motorPwr[2] = {0, 0};
unsigned char motorDir[2] = {0, 0};

unsigned char getSensor(unsigned char pin) {
	return (analog(pin) > WHITE);
	// unsigned char i = analog(pin);
	// i = (i > WHITE) ? 0 : 1;
}

void AksenMain(void) {
	lcd_puts("Seite: ");
	/* setting defaults */
	motor_richtung(0, motorDir[0]);
	motor_richtung(1, motorDir[1]);
	motor_pwm(0, motorPwr[0]);
	motor_pwm(1, motorPwr[1]);
	// richtungserkennung
	
}

void MainLeft(void) {

}

void MainRight(void) {

}