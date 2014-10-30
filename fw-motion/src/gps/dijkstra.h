/**
 * (c) Real-time systems project seminar, TU Darmstadt
 * 
 * @file dijkstra.h
 * @author Team GPS
 * @brief Wendet den Dijkstra Algortihmus auf eine kantenbasierte Karte an
 **/

#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

uint8_t way[50];

/**
 * Berechnet den kürzesten Weg
 * Input: startknoten, zielknoten
 * Return: gefundener Weg als array
 */
uint8_t *find_shortest_path(uint8_t start, uint8_t ziel);


#endif /* DIJKSTRA_H_ */
