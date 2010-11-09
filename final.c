#include <stdio.h>
#include <regc515c.h>
#include <stub.h>

#define WHITE 110
//linker/rechter Motor vorwärts/rückwärts definieren
#define motLeft 0
#define motRight 1
#define forward 0
#define backward 1

//Sensoren von links nach rechts definieren z.B.: LEFT_RIGHT ist der rechte von der linken Sensorgruppe
#define LEFT_LEFT 6
#define LEFT_MID 7
#define LEFT_RIGHT 8

#define MID_LEFT 5
#define MID_MID 4
#define MID_RIGHT 3

#define RIGHT_LEFT 2
#define RIGHT_MID 1
#define RIGHT_RIGHT 0

//Anzahl Sensoren für Bodenerkennung
#define SENSOR_WHITE 9

// Laufzeit in ms: 120000 für unser Spiel
#define RUNTIME 9000


//Motorzustände definieren
unsigned int motPowLeft = 0;
unsigned int motPowRight = 0;
unsigned int motGearLeft = forward;
unsigned int motGearRight = forward;

unsigned char sensor[SENSOR_WHITE];

//Methode zum starten(warten auf Startlampe) 
void start(void){
while(analog(9) < WHITE) {
		lcd_puts("<3 Ich will starten <3");
	}
}

void setMotPow(unsigned left, unsigned right);

//Methode die in das Array sensors[] die Werte 1(weiß) oder 0(nich-weiß) für alle Liniensensoren schreibt
void updateSensorsWhite(void){
	int i;
	for (i = 0; i < SENSOR_WHITE; i++) {
		sensor[i] = (analog(i) < WHITE);
	}
}


//Roboter zu weit links erkennen 0=nicht zu weit links, 1=zu weit links
unsigned char muchLeft(void){
	unsigned char left = 0;
	updateSensorsWhite();
	if (sensor[MID_RIGHT] == 0 & sensor[MID_LEFT] == 1){
		left=1;
		lcd_cls();
		lcd_puts("right");
	}
	return left;
}
//Roboter zu weit rechts erkennen 0=nicht zu weit rechts, 1=zu weit rechts
unsigned char muchRight(void){
	unsigned char right = 0;
	updateSensorsWhite();
	if (sensor[MID_LEFT] == 0 & sensor[MID_RIGHT] == 1){
		right=1;
		lcd_cls();
		lcd_puts("left");
	}
	return right;
}
//Prüfung ob zu weit links oder rechts und reagieren mit Kurskorrektur ->Folgen der Linie
void followLine(void){
	if(muchLeft()){
		setMotPow(motPowLeft,motPowRight-2);
		sleep(50);
	}else if (muchRight()){
		setMotPow(motPowLeft-2,motPowRight);
		sleep(50);
	}
}



void setMotPow(unsigned left, unsigned right){
motor_pwm(motLeft,left);
motor_pwm(motRight,right);
}

void AksenMain(void) {
	unsigned long timeoutAt = 0;
	{
	//start();
	lcd_puts("<3 Ich will starten <3");
	timeoutAt = akt_time() + RUNTIME;
	}
	motPowLeft=8;
	motPowRight=8;
	do {
	setMotPow(motPowLeft,motPowRight);
	if (akt_time() >= timeoutAt) {
		setMotPow(0,0);
		break;
	}
	followLine();
	
	} while(1);
	
	while(1);
}