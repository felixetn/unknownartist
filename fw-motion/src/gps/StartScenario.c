/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file StartScenario.c
 * @author Team GPS
 * @brief In diesem Modul können die verschiedenen Szenarien gestartet werden.
 **/

#include "StartScenario.h"
#include "../cartography/create_map.h"
#include "GPS_Handler.h"
#include "../cartography/slam/SLAM_Algorithm.h"
#include "../api/seg.h"

static uint8_t start = 0;
static int8_t targetReached = 0;

/**
 * Entsprechend dem im TUIO Client angegeben Szenario werden die entsprechenden Methoden aufgerufen
 * Input: Startkoordinaten, Zielkoordinanten, Art des Szenarios
 */
void StartScenario(gps_reducedData_t* ownCoords, gps_reducedData_t* aimCoords, uint8_t scenario){

	switch (scenario){
		case CONTROL_START:
		//	storeElement(6);
			eagleEye(ownCoords, aimCoords);
		//	Drive_SetMotor(2);
			break;
		case CONTROL_FOLLOW_OFFROAD:
			if(start==0) map_init_values();		//initialisierung der Karte
			StartMapNavigationOffroad(ownCoords, aimCoords);
			break;
		case CONTROL_FOLLOW_LOWEST_ID:
			eagleEye(ownCoords, aimCoords);
			break;
		case CONTROL_FOLLOW_OFFROAD_DYN:
			StartMapNavigationOffroad(ownCoords, aimCoords);
			break;
		case CONTROL_CARTOGRAPHY:
			//Cartographie aufrufen;
			start_create_map(ownCoords, aimCoords);
			break;
		case CONTROL_SLAM:
			set_ownCoords(0,0,0);
			start_create_map(get_ownCoords(), get_ownCoords());
			break;
	}
}

int8_t get_targetReached(){
	return targetReached;
}

/*
 * Starte die Kartenbasierte Navigation (Offroad)
 * Input: Startkoordinaten, Zielkoordinanten
 */
void StartMapNavigationOffroad(gps_reducedData_t* ownCoords, gps_reducedData_t* aimCoords){
	static gps_reducedData_t differenceVector;
	static gps_reducedData_t newaim;
	uint16_t diffx, diffy;
	static uint8_t x[50], y[50];	//größe erst später festlegen? Ist erst in tree_getpath bekannt
	static uint8_t i = 0;
	static uint8_t l = 0;
	static int16_t aimaltx = -1, aimalty = -1;

	#ifdef DEBUG
		printf("Scenario Offroad:\n\r");
	#endif
		ownCoords = get_ownCoords();
		/*//reset: neues Ziel anfahren
		if (aimaltx != -1){
			diffx = giveDifferenzBetrag(ownCoords->x,  aimaltx);
			diffy = giveDifferenzBetrag(ownCoords->y,  aimalty);
			if (start == 1 && (diffx < 400 && diffy < 400)) {
				diffx = giveDifferenzBetrag(aimCoords->x,  aimaltx);
				diffy = giveDifferenzBetrag(aimCoords->y,  aimalty);
				if (diffx > 400 && diffy > 400){
					tree_delete();
					start = 0;	//Ab jetzt darf neue Route berechnet werden
				}
			}
			//Abfrage: Wenn sich Zielort verändert berechne neue Route
		}
	*/
//		Seg_Hex(0x04);
		if (start == 0){ //soll hier nur im ersten durchlauf zur initialisierung rein
			start = 1;
			aimaltx = aimCoords->x;
			aimalty = aimCoords->y;

			if (calc_Offroad(&newaim, ownCoords, aimCoords) == 0)
				//falls kein Hindernis auf der Strecke entdeckt
				//initiales drehen damit möglichst direkt zum Ziel gefahren werden kann
				look_in_direction(aimCoords->x, aimCoords->y,ownCoords->x, ownCoords->y,ownCoords->angle);
			else if(tree_isEmpty() == 0){
				tree_get_shortest_path(x,y,&i,CoordinatesToMap(aimCoords->x), CoordinatesToMap(aimCoords->y));
//				convertToCharArraySurroundSmall('z',i, 10, pkg);
//					Bluetooth_Layer3_SendPacket(pkg, sizeof(pkg), PT_GPS_DEBUG_INFO);
				if(i > 0){
//					convertToCharArraySurroundSmall('g',x[i-1], y[i-1], pkg);
//					Bluetooth_Layer3_SendPacket(pkg, sizeof(pkg), PT_GPS_DEBUG_INFO);
					//initiales drehen damit möglichst direkt zum ersten Wegpunkt gefahren werden kann
					look_in_direction( (int16_t)MapToCoordinates(x[i-1]), (int16_t)MapToCoordinates(y[i-1]),ownCoords->x, ownCoords->y,ownCoords->angle);
					setCoordinates(&newaim, aimCoords->cam_id, aimCoords->tag_id, (int16_t)MapToCoordinates(x[i-1]), (int16_t)MapToCoordinates(y[i-1]), aimCoords->angle, aimCoords->isWorld);
					i--;
				}
				else{
					turn_to_angle(aimCoords->angle, ownCoords->angle);
					targetReached = 1;
				}
			}
		}

		l++;
		//Wenn es noch Zwischenpunkte gibt
		if(tree_isEmpty() == 0 && targetReached == 0){
			Seg_Hex(0x04);
			os_wait(1000);
			diffx = giveDifferenceAbsolute(newaim.x, ownCoords->x);
			diffy = giveDifferenceAbsolute(newaim.y, ownCoords->y);
			if(diffx < 1000 && diffy < 1000){	//Gehe zu nächsten Wegpunkt wenn nahe genug an aktuellem Ziel
				//Zum nächsten Zwischenpunkt hin orientieren
				look_in_direction( (int16_t)MapToCoordinates(x[i-1]), (int16_t)MapToCoordinates(y[i-1]),ownCoords->x, ownCoords->y,ownCoords->angle);
				setCoordinates(&newaim, aimCoords->cam_id, aimCoords->tag_id, (int16_t)MapToCoordinates(x[i-1]), (int16_t)MapToCoordinates(y[i-1]), aimCoords->angle, aimCoords->isWorld);
				if(i > 0) i--;
				else{
					turn_to_angle(aimCoords->angle, ownCoords->angle);
					targetReached = 1;
				}
			}
			calculateDifferenceVector(&differenceVector, ownCoords, &newaim);
		}
		//Fahre zu den Zielkoordiaten
		else{
			Seg_Hex(0x09);
			os_wait(1000);
			calculateDifferenceVector(&differenceVector, ownCoords, aimCoords);
		}
		//Navigiere zum nächsten Wegpunkt
		Seg_Hex(0x05);
		os_wait(1000);
		navigate(&differenceVector);

	#ifdef DEBUG
		printf("Scenario Offroad - calculations done\n\r");
	#endif
}
