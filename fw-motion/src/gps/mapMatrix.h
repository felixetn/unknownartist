/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file map.h
 * @author Team GPS
 * @brief Dieses Modul stellt eine statische Karte mit Hiflsmethoden sowie eine Methode zur Kollisionsberechnung zur Verfügung
 **/

#include "help_methods.h"
#include "../api/api.h"



#ifndef MAP_H_
#define MAP_H_

// 32 * 4cm x/y positions per field
#define MAPSIZE 32
//100 fields per dimension
#define GRIDSIZE 100

//Idee um Hindernisse zwischenzuspeichern. Soll wall follow beschleunigen
typedef struct obstacle_left{
		int16_t x[DRIVELENGTH/5-1];
		int16_t y[DRIVELENGTH/5-1];
		uint16_t angle[DRIVELENGTH/5-1];
		uint16_t dist[DRIVELENGTH/5-1];
} obsLeft;

typedef struct obstacle_right{
		int16_t x[DRIVELENGTH/5-1];
		int16_t y[DRIVELENGTH/5-1];
		uint16_t angle[DRIVELENGTH/5-1];
		uint16_t dist[DRIVELENGTH/5-1];
} obsRight;

extern obsLeft obs_left;
extern obsRight obs_right;

void map_init(void);
void map_init_Cart(void);
void map_init_values(void);
void init_obstacles(void);
void set_obstacleLeft( int16_t x, int16_t y, uint16_t direction, uint16_t dist);
void set_obstacleRight( int16_t x, int16_t y, uint16_t direction, uint16_t dist);
void load_map(int16_t coord_x, int16_t coord_y, uint8_t type);
void store_Map(uint8_t type);
void delete_map(void);
void set_free_area(int16_t x, int16_t y, int16_t dist, int8_t direction, uint16_t angle, int8_t type);
int16_t divide_with_double(int16_t value);
void set_coord_driven(int16_t coord_x, int16_t coord_y);
void set_coord_obstacle_withoutZone(int16_t coord_x, int16_t coord_y);
void set_coord_obstacle(int16_t coord_x, int16_t coord_y);
uint8_t get_coord_driven(int16_t coord_x, int16_t coord_y);
uint8_t get_coord_obstacle(int16_t coord_x, int16_t coord_y);
int16_t CoordinatesToMap(int32_t Koordinate);
uint16_t MapToCoordinates(int16_t Koordinate);
uint8_t calc_reachability(int16_t* xCollision, int16_t* yCollision, int16_t ownx, int16_t owny, int16_t targetx, int16_t targety);


#endif /* MAP_H_ */
