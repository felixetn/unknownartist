/**
 * (c) Real-time systems project seminar, TU Darmstadt
 * 
 * @file tree.h
 * @author Team GPS
 * @brief Dieses Modul stellt einen Baum zur Verfügung wo in jedem Knoten ein Struct gespeichert ist.
 * Basierend auf dem Baum können mehrere Wege zum Ziel rekursiv ausgelesen werden
 **/

#ifndef TREE_H_
#define TREE_H_

#include "commonLibraries.h"
#include "stdlib.h"

void tree_init(uint8_t x, uint8_t y);
void tree_insert(uint8_t x_next, uint8_t y_next, uint8_t x_last, uint8_t y_last, uint8_t direction);
void tree_delete(void);
uint8_t tree_get_shortest_path(uint8_t xWay[], uint8_t yWay[],  uint8_t* j,uint8_t xZiel, uint8_t yZiel);
uint8_t tree_isEmpty(void);


#endif /* TREE_H_ */
