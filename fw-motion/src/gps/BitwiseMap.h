#ifndef BitwiseMap_H_
#define BitwiseMap_H_


#include "mapMatrix.h"

#define BIT_ISSET(x,n)          ((x[n>>3] & (1<<(n&7)))!=0)
#define BIT_SET(x,n)              x[n>>3] |=(1<<(n&7))
#define BIT_CLEAR(x,n)            x[n>>3] &=(1<<(n&7))^0xFF


#define DrivenArea_ISSET(gen, n)       BIT_ISSET(DrivenArea_activePlaces[gen].list, n)
#define DrivenArea_SET(  gen, n)       BIT_SET(  DrivenArea_activePlaces[gen].list, n)
#define DrivenArea_CLEAR(gen, n)       BIT_CLEAR(DrivenArea_activePlaces[gen].list, n)

#define Obstacle_ISSET(gen, n)       BIT_ISSET(Obstacle_activePlaces[gen].list, n)
#define Obstacle_SET(  gen, n)       BIT_SET(Obstacle_activePlaces[gen].list, n)
#define Obstacle_CLEAR(gen, n)       BIT_CLEAR(Obstacle_activePlaces[gen].list, n)


typedef struct DrivenArea_plc {
   char list[MAPSIZE/8];
} DrivenArea_Places;

typedef struct Obstacle_plc {
   char list[MAPSIZE/8];
} Obstacle_Places;


extern DrivenArea_Places DrivenArea_activePlaces[MAPSIZE];
extern Obstacle_Places Obstacle_activePlaces[MAPSIZE];

void BitwiseMap_init(void);

#endif
