/**
 * (c) Real-time systems project seminar, TU Darmstadt
 * 
 * @file regler.c
 * @author Felix Euteneuer
 * @brief TODO - insert a brief description of that module
 **/

/*
 * Include files
 */

#include "regler.h"
#include "api.c"
#include "base_type.h"
#include "wireless/wireless.h"

/*
 * Global pre-processor symbols/macros ('#define')
 */

/*
 * Global type definitions ('typedef')
 */

/*
 * Global and local variable declarations
 */


/*
 * Global and local functions
 */
void Regler_set_Kp(double p) {
   Kp = p;
}

void Regler_set_Ki(double i) {
   Ki = i;
}

void Regler_set_Kd(double d) {
   Kd = d;
}

void Regler_output(int16_t y){
	Drive_SetServo(y); 				//Beispielfunktion zum Senden eines Analogsignals für die Stellgröße y
}

uint16_t Regler_get_sensor(){
   return us_getRightDistance(); 				//Beispielfunktion zum Auslesen eines analogen Sensorsignals
}



/**
 * Get the output of the PID-Controller
 * @param sollwert The distance between the car and the wall on the right side you want to control
 * @param istwert The actual distance between the car and the wall on the right side
 */
int16_t Regler_pid(int16_t sollwert, int16_t istwert){
    static int16_t esum = 0; 			//esum ist die Summierung aller Regelabweichungen
    static int16_t e_alt = 0; 			//ist die vorherige Regelabweichun
    static int16_t e_aelter = 0;
    //static int16_t counter = 0;
    static int16_t e;				//Die Regelabweichung

    int16_t y_d = 0;
    int16_t y_p = 0, y_i = 0; 			//Die einzelnen Stellgrößen
    int16_t y_gesamt; 					//Die Gesamtstellgröße


   // int16_t e;

    e = istwert - sollwert; 		//Berechnung der Regelabweichung nach e(t) = w(t) - x(t)

    y_p = Kp * e; 					//Der Proportional-Anteil nach y(t) = Kp * e(t)

    esum = esum + e; 				//Jede Regelabweichung wird durch das Integral über die Zeit aufsummiert
    y_i = Ki * esum; 				//Der Integral-Anteil nach y(t) = Ki * e(t)

    /*counter++;
    if(counter == 1){
    	e_alt = e;
    }
    if(counter == 3){
    	y_d= Kd * (e - e_alt);
    	counter = 0;
    }*/

    e_aelter=e_alt;
    e_alt = e;

    y_d= Kd * (e - e_aelter);

    //y_d= Kd * (e - e_alt); 		//Der Differential-Anteil nach y(t) = Kd * (d e(t) / dt)
    //e_alt = e; 					//Der Durchlauf ist beendet, die aktuelle Regelabweichung ist die alte für den nächsten Durchlauf

    wirelessFormattedDebugMessage(WI_IF_AMB8420, "Differenz: %d", y_d);

    y_gesamt =  y_p + y_i + y_d; 	//Alle Stellgrößen zur Gesamtstellgröße addieren

    if(y_gesamt<-100){
    	return -100;
    }
    if(y_gesamt>100){
    	return 100;
    }
    return y_gesamt; 				//und diese zurückgegben
}

/*int16_t Regler_get_fehler(){
   return e; 				//Beispielfunktion zum Auslesen eines analogen Sensorsignals
}

int16_t Regler_get_fdif(){
   return (e-e_alt); 				//Beispielfunktion zum Auslesen eines analogen Sensorsignals
}*/


/**
 * @brief Put a message in the transmit buffer.
 * @param param one
 * @param param two
 * @return result
 */
//uint8_t ModuleFoo_DooFoo(uint8_t* data, uint8_t len) {
//	return 1;
//}
