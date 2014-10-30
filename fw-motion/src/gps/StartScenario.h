/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file StartScenario.h
 * @author Team GPS
 * @brief In diesem Modul können die verschiedenen Szenarien gestartet werden.
 **/

#ifndef STARTSCENARIO_H_
#define STARTSCENARIO_H_

#include "state_eagle_eye.h"
#include "help_methods.h"
#include "navigation.h"
#include "mapMatrix.h"
#include "Bahnplanung_Offroad.h"

void StartScenario(gps_reducedData_t* ownCoords, gps_reducedData_t* aimCoords, uint8_t scenario);
void StartMapNavigationOffroad(gps_reducedData_t* ownCoords, gps_reducedData_t* aimCoords);
int8_t get_targetReached(void);


#endif /* STARTSCENARIO_H_ */
