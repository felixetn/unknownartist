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

void gablung_entscheideRichtung(){
	switch(richtungen[0]){
		case 0:
				if(kannLinks == 1){
					linksKurve();
				}else if(kannVorne){
					geradeUeberDieKreuzung();
				}else{
					rechtsKurve();
				}
				break;

		case 1:
				if(kannVorne == 1){
					geradeUeberDieKreuzung()
				}else if(kannLinks){
					linksKurve();
				}else{
					rechtsKurve();
				}
				break;

		case 2:
				if(kannRechts == 1){
					RechtsKurve();
				}else if(kannVorne){
					geradeUeberDieKreuzung();
				}else{
					linksKurve();
				}
				break;
	}
	geradeUeberDieKreuzung();
}

void rechtsKurve(){
	inEinerKurve == 1;
	//TODO
}
void linksKurve(){
	inEinerKurve == 1;
	//TODO
}
void geradeUeberDieKreuzung(){
	inEinerKurve == 1;
	if (us_getLeftDistance()+us_getRightDistance() > 4000){
		Drive_SetMotor(1);
	}else{
		inEinerKurve = 0;
	}
	//TODO
}

/**
 * @brief Put a message in the transmit buffer.
 * @param param one
 * @param param two
 * @return result
 */
//uint8_t ModuleFoo_DooFoo(uint8_t* data, uint8_t len) {
	//return 1;
//}
