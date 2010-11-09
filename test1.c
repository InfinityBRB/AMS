#include <stdio.h>
#include <regc515c.h>
#include <stub.h>

void AksenMain(void) {
	lcd_puts("Hallo");
	motor_richtung((unsigned char)0, (unsigned char)0);
	motor_richtung((unsigned char)1, (unsigned char)0);
	motor_pwm((unsigned char)0, (unsigned char)2);
	motor_pwm((unsigned char)1, (unsigned char)2);
	while(1);
}