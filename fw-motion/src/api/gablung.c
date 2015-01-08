/**
 * (c) Real-time systems project seminar, TU Darmstadt
 * 
 * @file gablung.c
 * @author Daniel	
 * @brief TODO - insert a brief description of that module
 **/

/*
 * Include files
 */

#include "base_type.h"
#include "navigation.h"
#include "drive.h"
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
int8_t inRechtsKurve = 0;
int8_t inLinksKurve = 0;
int8_t faehrtGeradeaus = 0;
uint32_t start;
/*
 * Global and local functions
 */

void Gablung_entscheideRichtung(){
	Transition_setInEinerKurve(1);
	if (inRechtsKurve == 1){
		Gablung_rechtsKurve();
	}

	else if (inLinksKurve == 1){
		Gablung_linksKurve();
	}

	else if (faehrtGeradeaus == 1){
		Gablung_geradeUeberDieKreuzung();
	}
else{
		start = Drive_GetTotalDrivenDistance();

	switch(Navigation_getElement_i(0)){
		//Soll links fahren
		case 0:
				if(Transitionen_getKannLinks() == 1){
					Gablung_linksKurve();
				}else if(Transitionen_getKannVorne() == 1){
					Gablung_geradeUeberDieKreuzung();
				}else{
					Gablung_rechtsKurve();
				}
				break;


		//Soll geradeaus fahren
		case 1:
				if(Transitionen_getKannVorne() == 1){
					Gablung_geradeUeberDieKreuzung();
				}else if(Transitionen_getKannRechts() == 1){
					Gablung_linksKurve();
				}else{
					Gablung_rechtsKurve();
				}
				break;

		//Soll rechts fahren
		case 2:
				if(Transitionen_getKannRechts() == 1){
					Gablung_rechtsKurve();
				}else if(Transitionen_getKannVorne() == 1){
					Gablung_geradeUeberDieKreuzung();
				}else{
					Gablung_linksKurve();
				}
				break;
	}
}
	//Default
	//geradeUeberDieKreuzung();

}




	void Gablung_rechtsKurve(void){
		Transition_setInEinerKurve(1);
		inRechtsKurve = 1;
		 wirelessFormattedDebugMessage(WI_IF_AMB8420, "RechtsKurve");
		Drive_SetServo(100);
		//Drive_SetMotor(1);
		if((Drive_GetTotalDrivenDistance() - start) > 1178){
			//Drive_SetMotor(0);
			Drive_SetServo(-100);

			//Drive_SetMotor(-1);
			Transition_setInEinerKurve(0);
			inRechtsKurve = 0;
			Navigation_Kurve_gefahren();

		}
		//TODO
	}
	void Gablung_linksKurve(void){
		Transition_setInEinerKurve(1);
		inLinksKurve = 1;
		 wirelessFormattedDebugMessage(WI_IF_AMB8420, "LinksKurve");
		Drive_SetServo(-100);
				//Drive_SetMotor(1);
				if((Drive_GetTotalDrivenDistance() - start) > 1178){
					//Drive_SetMotor(0);
					Drive_SetServo(100);

					//Drive_SetMotor(-1);
					Transition_setInEinerKurve(0);
					inLinksKurve = 0;
					Navigation_Kurve_gefahren();
				}
				//TODO
	}
	void Gablung_geradeUeberDieKreuzung(){
		Transition_setInEinerKurve(1);
		faehrtGeradeaus = 1;
		 wirelessFormattedDebugMessage(WI_IF_AMB8420, "GeradeAus");
		Drive_SetServo(0);
				//Drive_SetMotor(1);
				if((Drive_GetTotalDrivenDistance() - start) > 1800){

					Transition_setInEinerKurve(0);
					faehrtGeradeaus = 0;
					Navigation_Kurve_gefahren();
				}
				//TODO
	}





/**
 * @brief Put a message in the transmit buffer.
 * @param param one
 * @param param two
 * @return result
 */
uint8_t ModuleFoo_DooFoo(uint8_t* data, uint8_t len) {
	return 1;
}
