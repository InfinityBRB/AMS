#include <stdio.h>
#include <regc515c.h>
#include <stub.h>

#define WHITE 180

unsigned char getSensor(unsigned char pin) {
	return (analog(pin) > WHITE);
}

void AksenMain(void) {
	unsigned char s1, s2, s3;
	lcd_puts("Hallo");
	motor_richtung((unsigned char)0, (unsigned char)0);
	motor_richtung((unsigned char)1, (unsigned char)0);
	motor_pwm((unsigned char)0, (unsigned char)2);
	motor_pwm((unsigned char)1, (unsigned char)2);
	while(1) {
		s1 = getSensor(8);
		s2 = getSensor(9);
		s3 = getSensor(10);
		lcd_setxy(1,0);
		lcd_ubyte(s1);
		lcd_ubyte(s2);
		lcd_ubyte(s3);
		if (s1 && s2 && s3) {
			motor_pwm((unsigned char)0, (unsigned char)0);
			motor_pwm((unsigned char)1, (unsigned char)0);
			motor_richtung((unsigned char)0, (unsigned char)1);
			motor_richtung((unsigned char)1, (unsigned char)1);
			motor_pwm((unsigned char)0, (unsigned char)2);
			motor_pwm((unsigned char)1, (unsigned char)2);
			sleep(100);
			motor_pwm((unsigned char)0, (unsigned char)0);
			motor_pwm((unsigned char)1, (unsigned char)0);
			motor_richtung((unsigned char)0, (unsigned char)0);
			motor_richtung((unsigned char)1, (unsigned char)0);
			break;
		}
	}
	while(1) {
		lcd_setxy(1,0);
		lcd_ubyte(getSensor(8));
		lcd_ubyte(getSensor(9));
		lcd_ubyte(getSensor(10));
	}
}