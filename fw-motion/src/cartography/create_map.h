/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file map.h
 * @author Tobias
 * @brief Modul zum erstellen einer Karte der Umgebung. In diesem Modul werden Sensoren verknüpft
 * und der Ablauf des Programms gesteuert
 **/

#ifndef CREATE_MAP_H_
#define CREATE_MAP_H_

#include "../gps/commonLibraries.h"
#include "../freertos/include/FreeRTOS.h"
#include "../freertos/include/task.h"
#include "../api/api.h"
#include "../api/defines.h"

#define RANGEFRONT 30
#define RANGESIDE 50

extern xTaskHandle xHandleCreateMap;
extern boolean_t checkDrivenArea;


void init_create_map(void);
boolean_t createMap_isActivated(void);
void set_CoordinatesFlag(boolean_t f);
boolean_t get_CoordinatesFlag(void);

void start_create_map(gps_reducedData_t*, gps_reducedData_t*);
void stop_create_map(void);


#endif /* CREATE_MAP_H_ */
