#include <stdio.h>
#include <regc515c.h>
#include <stub.h>

#define WHITE 110

unsigned char getSensor(unsigned char pin) {
	return (analog(pin) > WHITE);
}

void AksenMain(void) {
	unsigned char s0, s1, s2, s3, s4, s5, s6, s7, s8;
	lcd_puts("Hallo");
	motor_richtung((unsigned char)0, (unsigned char)0);
	motor_richtung((unsigned char)1, (unsigned char)0);
	motor_pwm((unsigned char)0, (unsigned char)10);
	motor_pwm((unsigned char)1, (unsigned char)10);
	while(1) {
		s0 = getSensor(0);
		s1 = getSensor(1);
		s2 = getSensor(2);
		s3 = getSensor(3);
		s4 = getSensor(4);
		s5 = getSensor(5);
		s6 = getSensor(6);
		s7 = getSensor(7);
		s8 = getSensor(8);
		lcd_setxy(1,0);
		lcd_ubyte(s0);
		lcd_ubyte(s1);
		lcd_ubyte(s2);
		lcd_ubyte(s3);
		lcd_ubyte(s4);
		lcd_ubyte(s5);
		lcd_ubyte(s6);
		lcd_ubyte(s7);
		lcd_ubyte(s8);
		if (s0 || s1 || s2 || s3 || s4 || s5 || s6 || s7 || s8) {
			motor_richtung((unsigned char)0, (unsigned char)1);
			motor_richtung((unsigned char)1, (unsigned char)1);
			sleep(100);
			motor_pwm((unsigned char)0, (unsigned char)0);
			motor_pwm((unsigned char)1, (unsigned char)0);
			motor_richtung((unsigned char)0, (unsigned char)0);
			motor_richtung((unsigned char)1, (unsigned char)0);
			break;
		}
	}
	while(1) {/*
		lcd_cls();
		lcd_ubyte(analog(0));
		lcd_setxy(0, 4);
		lcd_ubyte(analog(1));
		lcd_setxy(0, 8);
		lcd_ubyte(analog(2));
		lcd_setxy(0, 12);
		lcd_ubyte(analog(3));
		lcd_setxy(1, 0);
		lcd_ubyte(analog(4));
		lcd_setxy(1, 4);
		lcd_ubyte(analog(5));
		lcd_setxy(1, 8);
		lcd_ubyte(analog(6));
		lcd_setxy(1, 12);
		lcd_ubyte(analog(7));
		sleep(50);/**/
		
		lcd_setxy(1,0);
		lcd_ubyte(getSensor(0));
		lcd_ubyte(getSensor(1));
		lcd_ubyte(getSensor(2));
		lcd_ubyte(getSensor(3));
		lcd_ubyte(getSensor(4));
		lcd_ubyte(getSensor(5));
		lcd_ubyte(getSensor(6));
		lcd_ubyte(getSensor(7));
		lcd_ubyte(getSensor(8));
		/*lcd_setxy(1,10);
		lcd_puts("   ");
		lcd_setxy(1,10);
		lcd_ubyte(analog(0));*/
	}
}