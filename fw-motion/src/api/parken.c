/**
 * (c) Real-time systems project seminar, TU Darmstadt
 * 
 * @file parken.c
 * @author Felix	
 * @brief TODO - insert a brief description of that module
 **/

/*
 * Include files
 */
#include "parken.h"
#include "regler.h"
#include "drive.h"
#include "regler.h"
#include <math.h>
#include "wireless/wireless.h"
/*
 * Global pre-processor symbols/macros ('#define')
 */

#define MIN_HINDERNIS_GROESSE 100
#define RUECKWAERTS_PARKLUEKENGROESSE 200

/*
 * Global type definitions ('typedef')
 */



/*
 * Global and local variable declarations
 */

static int16_t abstand;
static int16_t abstand_alt;
static int16_t differenz;
static int16_t differenz_speicher;
static int8_t parkzustand;
static int8_t parkzustand_erhoehen;
static int8_t init = 1;
static int8_t regeln = 1;
static uint32_t distanz;

/*
 * Global and local functions
 */

void PARKEN_init(){
	abstand = Regler_get_sensor();
	abstand_alt = Regler_get_sensor();
	parkzustand = 0;
	Drive_SetMotor(1);
}

void PARKEN_parallel(){
	// Zustand ausgeben
//	Seg_Dec(parkzustand);

	// Initialisierung zum Parken
	if(init == 1){
		PARKEN_init();
		init = 0;
	}

	// PID-Regler ON/OFF
	if(regeln == 1){
		Regler_output(Regler_pid(Regler_get_sensor()));
	}

	differenz = abstand - abstand_alt;

	 wirelessFormattedDebugMessage(WI_IF_AMB8420, "A = %d, B = %d, Differnez = %d",abstand, abstand_alt, differenz);

	// Hindernis auf der rechten Seite erkennen
	if((parkzustand == 0) && (differenz <= - MIN_HINDERNIS_GROESSE)){
		differenz_speicher = differenz;
		Regler_set_sollwert(Regler_get_sollwert() + differenz_speicher);
		parkzustand_erhoehen = 1;
	}

	// kein Hindernis mehr auf der rechten Seite
	if((parkzustand == 1) && (differenz >= MIN_HINDERNIS_GROESSE)){
		Regler_set_sollwert(Regler_get_sollwert() - differenz_speicher);
		parkzustand_erhoehen = 1;
		distanz = Drive_GetTotalDrivenDistance();
		//Drive_SetMotorForDistance(1, 1000);
	}

	// nächstes Hindernis steht auf der rechten Seite
	if((parkzustand == 2) && (differenz <= - MIN_HINDERNIS_GROESSE)){
		if(Drive_GetTotalDrivenDistance() - distanz <= 700){
			parkzustand = 1;
		}
		differenz_speicher = differenz;
		Regler_set_sollwert(Regler_get_sollwert() + differenz_speicher);
	}

	// rückwärts fahren und voll nach rechts einschlagen
	if((parkzustand == 2) && (Drive_GetTotalDrivenDistance() - distanz >= 1300)){
		regeln = 0;
		Drive_SetMotor(-1);
		Drive_SetServo(100);
		parkzustand_erhoehen = 1;
		distanz = Drive_GetTotalDrivenDistance();
	}

//	// gerade rückwärtsfahren
//	if((parkzustand == 3) && (Drive_GetTotalDrivenDistance() - distanz >= 750)){
//		Drive_SetMotor(-1);
//		Drive_SetServo(0);
//		parkzustand_erhoehen = 1;
//		distanz = Drive_GetTotalDrivenDistance();
//	}

	// rückwärts fahren und voll nach links einschlagen
	if((parkzustand == 3) && (Drive_GetTotalDrivenDistance() - distanz >= 750)){
			Drive_SetMotor(-1);
			Drive_SetServo(-100);
			parkzustand_erhoehen = 1;
			distanz = Drive_GetTotalDrivenDistance();
	}

	// Anhalten
	if((parkzustand == 4) && (Drive_GetTotalDrivenDistance() - distanz >= 600)){
			Drive_SetMotor(0);
			Drive_SetServo(0);
			parkzustand_erhoehen = 1;
	}

	// Abstände akktualisieren
	abstand_alt = abstand;
	abstand = Regler_get_sensor();

	// Parkzustand inkrementieren
	if(parkzustand_erhoehen == 1){
		parkzustand_erhoehen = 0;
		parkzustand++;
	}
}

void PARKEN_rueckwaerts(){
	// Zustand ausgeben
//	Seg_Dec(parkzustand);

	// Initialisierung zum Parken
	if(init == 1){
		PARKEN_init();
		init = 0;
	}

	// PID-Regler ON/OFF
	if(regeln == 1){
		Regler_output(Regler_pid(Regler_get_sensor()));
	}

	differenz = abstand - abstand_alt;

	 wirelessFormattedDebugMessage(WI_IF_AMB8420, "A = %d, B = %d, Differnez = %d",abstand, abstand_alt, differenz);

	 // Hindernis auf der rechten Seite erkennen
	if((parkzustand == 0) && (differenz <= - MIN_HINDERNIS_GROESSE)){
		differenz_speicher = differenz;
		Regler_set_sollwert(Regler_get_sollwert() + differenz_speicher);
		parkzustand_erhoehen = 1;
	}

	// kein Hindernis mehr auf der rechten Seite
	if((parkzustand == 1) && (differenz >= MIN_HINDERNIS_GROESSE)){
		Regler_set_sollwert(Regler_get_sollwert() - differenz_speicher);
		parkzustand_erhoehen = 1;
		distanz = Drive_GetTotalDrivenDistance();
		//Drive_SetMotorForDistance(1, 1000);
	}

	// nächstes Hindernis steht auf der rechten Seite
	if((parkzustand == 2) && (differenz <= - MIN_HINDERNIS_GROESSE)){
		if(Drive_GetTotalDrivenDistance() - distanz <= RUECKWAERTS_PARKLUEKENGROESSE){			// Parklücke zu klein
			parkzustand = 1;
		}
		differenz_speicher = differenz;
		Regler_set_sollwert(Regler_get_sollwert() + differenz_speicher);
	}

	// nächstes Hindernis verschwindet wieder auf der rechten Seite
	if((parkzustand == 2) && (differenz >= MIN_HINDERNIS_GROESSE)){
		Regler_set_sollwert(Regler_get_sollwert() - differenz_speicher);
	}

	// Parklücke gefunde: Einlenken
	if((parkzustand == 2) && (Drive_GetTotalDrivenDistance() - distanz >= RUECKWAERTS_PARKLUEKENGROESSE)){
		Drive_SetServo(-100);
		regeln = 0;
		distanz = Drive_GetTotalDrivenDistance();
		parkzustand_erhoehen = 1;
	}

	// rückwärts fahren und voll nach rechts einschlagen
	if((parkzustand == 3) && (Drive_GetTotalDrivenDistance() - distanz >= 650)){
		//regeln = 0;
		Drive_SetMotor(-1);
		Drive_SetServo(100);
		parkzustand_erhoehen = 1;
		distanz = Drive_GetTotalDrivenDistance();
	}

	// Anhalten
	if((parkzustand == 4) && (Drive_GetTotalDrivenDistance() - distanz >= 1000)){
		Drive_SetMotor(0);
		Drive_SetServo(0);
		parkzustand_erhoehen = 1;
	}

	// Abstände akktualisieren
	abstand_alt = abstand;
	abstand = Regler_get_sensor();

	// Parkzustand inkrementieren
	if(parkzustand_erhoehen == 1){
		parkzustand_erhoehen = 0;
		parkzustand++;
	}
}
