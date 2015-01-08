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
#include "api.h"
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

void Regler_set_sollwert(int16_t w){
	sollwert = w;
}

int16_t Regler_get_sollwert(){
	return sollwert;
}

int16_t Regler_get_tempSollwert(){
	return tempSollwert;
}

void Regler_output(int16_t y){
	Drive_SetServo(y); 				//Beispielfunktion zum Senden eines Analogsignals für die Stellgröße y
}

int16_t Regler_get_sensor(){
   return us_getRightDistance(); 				//Beispielfunktion zum Auslesen eines analogen Sensorsignals
}

/**
 * Get the output of the PID-Controller
 * @param sollwert The distance between the car and the wall on the right side you want to control
 * @param istwert The actual distance between the car and the wall on the right side
 */
int16_t Regler_pid(int16_t istwert){
    static int16_t esum = 0; 			//esum ist die Summierung aller Regelabweichungen
    static int16_t e_alt = 0; 			//ist die vorherige Regelabweichun
    //static int16_t e_aelter = 0;
    static int16_t e;				//Die Regelabweichung



    int16_t y_d = 0, y_p = 0, y_i = 0; 			//Die einzelnen Stellgrößen
    int16_t y_gesamt;		 					//Die Gesamtstellgröße und der Istwert, welcher dem Mittelwert, der Messwerte aus dem Ringbufffer entspricht.

    //Ringbuffer_input(momentanwert);				// Ringbuffer bekommt Momentanwert
    //istwert = Ringbuffer_mittelwert();	// Ringbuffer gibt Mittelwert über x Momentanwerte zurück

  //  regulierSollwert();
    e = istwert - sollwert; 		//Berechnung der Regelabweichung nach e(t) = w(t) - x(t)

    y_p = Kp * e; 					//Der Proportional-Anteil nach y(t) = Kp * e(t)

    esum = esum + e; 				//Jede Regelabweichung wird durch das Integral über die Zeit aufsummiert
    y_i = Ki * esum; 				//Der Integral-Anteil nach y(t) = Ki * e(t)

    //e_aelter=e_alt;
    //e_alt = e;

    //y_d= Kd * (e - e_aelter);

    y_d= Kd * (e - e_alt); 		//Der Differential-Anteil nach y(t) = Kd * (d e(t) / dt)
    e_alt = e; 					//Der Durchlauf ist beendet, die aktuelle Regelabweichung ist die alte für den nächsten Durchlauf

   // wirelessFormattedDebugMessage(WI_IF_AMB8420, "P=%d  \n  D=%d  I=%d\n", y_p, y_d, y_i);

    y_gesamt =  y_p + y_i + y_d; 	//Alle Stellgrößen zur Gesamtstellgröße addieren

    if(Regler_get_sensor() - Regler_get_sollwert() > 150 || Regler_get_sollwert() - Regler_get_sensor() > 150){

        if(y_gesamt< 0){
        	return -25;
        }
        if(y_gesamt> 0){
        	return 15;
        }



    }else{


    if(y_gesamt<-100){
    	return -100;
    }
    if(y_gesamt>100){
    	return 100;
    }
    return y_gesamt; 				//und diese zurückgegben

    }
}


