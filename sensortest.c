#include <stdio.h>
#include <regc515c.h>
#include <stub.h>

#define WHITE 110

unsigned char getSensor(unsigned char pin) {
	return (analog(pin) > WHITE);
}

void AksenMain(void) {
	lcd_puts("Hallo");
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
	}
}