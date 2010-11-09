#include <stdio.h>
#include <regc515c.h>
#include <stub.h>

#define WHITE 180

unsigned char getSensor(unsigned char pin) {
	return (analog(pin) > WHITE);
	// unsigned char i = analog(pin);
	// i = (i > WHITE) ? 0 : 1;
}

void AksenMain(void) {
	lcd_puts("Hallo");
	while(1) {
		lcd_setxy(1,0);
		lcd_ubyte(getSensor(8));
		lcd_ubyte(getSensor(9));
		lcd_ubyte(getSensor(10));
	}
	while(1);
}