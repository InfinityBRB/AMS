#include <stdio.h>
#include <regc515c.h>
#include <stub.h>

#define WHITE 110
//linker/rechter Motor vorwärts/rückwärts definieren
#define motLeft 1
#define motRight 0
#define forward 0
#define backward 1

//Sensoren von links nach rechts definieren z.B.: LEFT_RIGHT ist der rechte von der linken Sensorgruppe
#define LEFT 8
#define LEFT_BACK 7

#define MID_LEFT 5
#define MID_MID 4
#define MID_RIGHT 3

#define RIGHT 2
#define RIGHT_BACK 1

//Anzahl Sensoren für Bodenerkennung
#define SENSOR_WHITE 9

// Laufzeit in ms: 120000 für unser Spiel
#define RUNTIME 40000
#define LINESPERR 250
#define KURVSPERR 1100

//Motorzustände definieren
unsigned int motPowLeft = 0;
unsigned int motPowRight = 0;
unsigned int motGearLeft = forward;
unsigned int motGearRight = forward;

unsigned char sensor[SENSOR_WHITE];

//Abbiegungen Zähler
unsigned int abbrechts = 0;
unsigned int ablinks = 0;

//Linienzähler und Sperrzeit zum Zählen
unsigned int count = 0;
unsigned long sperrtimeout = 0;
unsigned long nachkurvetimeout = 0;
//Zwischenvariable dazu
unsigned int linel = 0;
unsigned int liner = 0;
//Kurvenzähler
unsigned int kurven = 0;


//Setzt die Motorkraft für den linken und rechten Motor 
void setMotPow(unsigned left, unsigned right){
motor_pwm(motLeft,left);
motor_pwm(motRight,right);
}
//Setzt den Motorgang(V/R) für den linken und rechten Motor
void setMotGear(unsigned left, unsigned right){
motor_richtung(motLeft,left);
motor_richtung(motRight,right);
}
//Methode zum starten(warten auf Startlampe) 
void start(void){
while(analog(9) < WHITE) {
		lcd_puts("<3 Ich will starten <3");
	}
}
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
	if (sensor[MID_RIGHT] == 0 && sensor[MID_LEFT] == 1){
		left=1;
	}
	return left;
}
//Roboter zu weit rechts erkennen 0=nicht zu weit rechts, 1=zu weit rechts
unsigned char muchRight(void){
	unsigned char right = 0;
	if (sensor[MID_LEFT] == 0 && sensor[MID_RIGHT] == 1){
		right=1;
	}
	return right;
}
//Prüfung ob zu weit links oder rechts und reagieren mit Kurskorrektur ->Folgen der Linie
void followLine(void){
	updateSensorsWhite();
	if(muchLeft()){
		setMotPow(motPowLeft,0);
		//lcd_puts("right");
	}else if (muchRight()){
		setMotPow(0,motPowRight);
		//lcd_puts("left");
	}else{
		setMotPow(motPowLeft,motPowRight);
	}
	
}

//Rotation um 90° um eine Ecke auf der Streckenmarkierung, Übergabe der Richtung als deutsches Wort links/rechts in einem String
void rotate(unsigned char richtung){
	//updateSensorsWhite();
	//nach rechts??
	if(richtung == 'rechts'){
		setMotPow(motPowLeft,0);
			setMotGear(forward,backward);
			setMotPow(motPowLeft,10);
			sleep(500);
			do{
				updateSensorsWhite();
			}while(sensor[MID_MID]);
			setMotPow(motPowLeft,0);
			setMotGear(forward,forward);		
		setMotPow(motPowLeft,motPowRight);
	
	//nach links??
	}else if (richtung == 'links'){
		setMotPow(0,motPowRight);
			setMotGear(backward,forward);
			setMotPow(10,motPowRight);
			sleep(500);
			do{
				updateSensorsWhite();
			}while(sensor[MID_MID]);
			setMotPow(0,motPowRight);
			setMotGear(forward,forward);
 		setMotPow(motPowLeft,motPowRight);
	}	
	followLine();
	kurven++;
	count = 0;
	lcd_cls();
	lcd_uint(count);
	nachkurvetimeout = akt_time() + KURVSPERR;
}

void countLines(void){
	if(sensor[LEFT_BACK] == 0 && linel == 0 && liner == 0 && akt_time()>=nachkurvetimeout && akt_time()>=sperrtimeout){
		linel = 1;
	}else if(sensor[LEFT_BACK] == 1 && linel == 1 && liner == 0){
		count++;
		linel =0;
		liner =0;
		sperrtimeout = akt_time() + LINESPERR;
		lcd_cls();
		lcd_uint(count);
	}		
	if(sensor[RIGHT_BACK] == 0 && liner == 0 && linel == 0 && akt_time()>=nachkurvetimeout && akt_time()>=sperrtimeout){
		liner = 1;		
	}else if(sensor[RIGHT_BACK] == 1 && liner == 1 && linel == 0){
		count++;
		linel =0;
		liner =0;
		sperrtimeout = akt_time() + LINESPERR;
		lcd_cls();
		lcd_uint(count);
	}
}

void AksenMain(void) {
	unsigned long timeoutAt = 0;
	
	//start();
	lcd_puts("<3 Ich will starten <3");
	timeoutAt = akt_time() + RUNTIME;
	
	motPowLeft=8;
	motPowRight=8;
	do {
		if (akt_time() >= timeoutAt) {
			setMotPow(0,0);
			break;
		}
		followLine();
		countLines();
		//Zählen der jeweiligen Linien bis zur nächsten Kurve Linien mit anschließender passender Rotation
		if(count == 1 && kurven == 0){
			rotate('links');
		}
		if(count == 2 && kurven == 1){
			rotate('links');
		}
		if(count == 1 && kurven == 2){
			rotate('rechts');
		}
		if(count == 2 && kurven == 3){
			rotate('rechts');
		}
		if(count == 2 && kurven == 4){
			rotate('links');
		}
		if(count == 2 && kurven == 5){
			rotate('links');
		}
		if(count == 2 && kurven == 6){
			rotate('rechts');
		}
		if(count == 2 && kurven == 7){
			rotate('rechts');
		}
		
		
	}while(1);
	while(1);
} 