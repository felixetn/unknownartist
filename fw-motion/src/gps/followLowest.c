/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file Bahnplanung_Offroad.c
 * @author Team GPS
 * @brief Sammelt Informationen über die Tags im Bereich der Kameraerfassung und sortiert diese in aufsteigender Reihenfolge und ist zuständig für die Auswahl des nächsten Ziels
 **/

#include "../api/wireless/wireless.h"
#include "GPS_Handler.h"
#include "GPSmessagetypes.h"
#include "help_methods.h"
#include "navigation.h"
#include "stdlib.h"

#include "mapMatrix.h"

#define OWN_COORD_SET_FLAG 1
#define TARGET_COORD_SET_FLAG 2

//Berechnung für lowestId
static uint8_t *IDarray = NULL;
static uint8_t count=0;

static uint8_t actualArrayPostion=0;
static uint8_t mapInit=0;

/**
 * Initialisierungsphase, bei der alle erkannten Tags eingelesen werden und in einem Array der Reihe nach sortiert werden
 *
 */
void init_lowest_ID(wirelessMessage_t* msg){
	gps_reducedData_t* message = (gps_reducedData_t*)msg->data;
	uint8_t found=0;

	// Koordinatensystem hat ID 0-2 und Auto selbst ID 3, diese sollen keine Ziele darstellen
	if(message->tag_id>3){
		//Sortieralgorithmus
		uint8_t low=255;
		uint8_t Stelle_low=0;

		//Schleifen
		uint8_t j;
		uint8_t i;
		uint8_t z=0;
		uint8_t temp;

		temp=message->tag_id;

		// Suche nach schon vorhandenen Zielen
		for(i=0;i<count;i++)
		{
			if(IDarray[i]==temp)
			{
				found=1;
				break;
			}
		}

		// Neue Ziele in sortiertes Array einfügen
		if (found==0)
		{
			IDarray = (uint8_t*) realloc(IDarray,sizeof(uint8_t)*(count+1));	//Speicherplatz während Laufzeit erhöhen

			IDarray[count]=temp;
			count++;

			//Sortieralgorithmus
			//Sortieren
			for(j=0;j<count;j++)
			{
				low=255;
				for(i=j;i<count;i++)
				{
					if (IDarray[i]<low)
					{
						low=IDarray[i];
						Stelle_low=i;
					}
				}
				temp=IDarray[j];
				IDarray[j]=low;
				IDarray[Stelle_low]=temp;
			}
		}
		else
		{
			found=0;
		}

		targetID=IDarray[0];
	}

}

/*
 * Wenn ein Ziel erreicht ist, wird aus dem Array das nächste Ziel ausgewählt
 *
 */
void increase_lowest_ID(){

	if(actualArrayPostion<(count-1)){
		coordsValidFlags = coordsValidFlags & ~TARGET_COORD_SET_FLAG;
		actualArrayPostion++;
		targetID=IDarray[actualArrayPostion];
	}

}

/*
 * Dynamische Hindernisse auf der Karte verteilen, basierend auf dem Array der vorhandenen Tags
 */
void set_Obstacle(wirelessMessage_t* msg){
	gps_reducedData_t* message = (gps_reducedData_t*)msg->data;
	uint8_t temp;
	uint8_t i;

	//Karte einmal initialisieren
	if(mapInit==0){
		map_init();
		mapInit++;
	}

	//Niedrigste ID ist das Ziel, die restlichen IDs werden als dynamische Hindernisse gesetzt
	temp=message->tag_id;
	for(i=1;i<count;i++)
	{
		if(IDarray[i]==temp)
		{
			set_coord_obstacle(CoordinatesToMap(message->x), CoordinatesToMap(message->y));
			break;
		}
	}


}
