#include "BitwiseMap.h"


//arrays which contain active places of respective generation
DrivenArea_Places DrivenArea_activePlaces[MAPSIZE];
Obstacle_Places Obstacle_activePlaces[MAPSIZE];


/**
 * Initiates all arrays
 **/
void BitwiseMap_init()
{
    int i,j;
    for (i = 0; i < MAPSIZE; i++) {
    	for (j = 0; j < MAPSIZE/8; j++) {
    		DrivenArea_activePlaces[i].list[j] = 0;
    		Obstacle_activePlaces[i].list[j] = 0;
    	}
    }
}

