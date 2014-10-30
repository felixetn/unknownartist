/**
 * (c) Real-time systems project seminar, TU Darmstadt
 * 
 * @file Bahnplanung_Offroad.h
 * @author Team GPS
 * @brief Dieses Modul berechnet verschiedene Wege um ein oder mehrere Hindernisse
 **/


#ifndef BAHNPLANUNG_OFFROAD_H_
#define BAHNPLANUNG_OFFROAD_H_

#include "help_methods.h"
#include "mapMatrix.h"
#include "commonLibraries.h"
#include "tree.h"

uint8_t calc_Offroad(gps_reducedData_t* diff, gps_reducedData_t* own, gps_reducedData_t* target);

#endif /* BAHNPLANUNG_OFFROAD_H_ */
