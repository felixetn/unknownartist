
/*	dijkstra.c

	Compute shortest paths in weighted graphs using Dijkstra's algorithm

	by: Steven Skiena
	date: March 6, 2002
*/

/*
Copyright 2003 by Steven S. Skiena; all rights reserved.

Permission is granted for use in non-commerical applications
provided this copyright notice remains intact and unchanged.

This program appears in my book:

"Programming Challenges: The Programming Contest Training Manual"
by Steven Skiena and Miguel Revilla, Springer-Verlag, New York 2003.

See our website www.programming-challenges.com for additional information.

This book can be ordered from Amazon.com at

http://www.amazon.com/exec/obidos/ASIN/0387001638/thealgorithmrepo/

Modifikationen und Anpassungen durch Team GPS

*/


#include <stdlib.h>
#include "map.h"
//#include "math.h"
#include "dijkstra.h"

#define MAXINT  100007
#define MAX8 255

int16_t parent[50];               /* discovery relation */

/**
 * Berechnet alle Pfade von start aus
 */
void dijkstra(uint8_t start){
	uint8_t i;				/* counter */
	uint8_t intree[MAX8+1];		/* is the vertex in the tree yet? */
	uint8_t distance[MAX8+1];		/* distance vertex is from start */
	uint8_t v;				/* current vertex to process */
	uint8_t w;				/* candidate next vertex */
	uint8_t weight;			/* edge weight */
	uint8_t dist;			/* best current distance from start */
	uint8_t* edges;
	map_edge* g;

	//Initialisierung aller werte
	for(i = 1; i <= map_first_free_edgeNumber; i++){
		intree[i] = 0;
		distance[i] = MAX8;
		parent[i] = -1;
	}

	distance[start] = 0;
	v = start;

	i = 0;
	while (intree[v] == 0){
		intree[v] = 1;
		edges = getEdges(v);
		i = 0;
		//Laufe die komplette Adjazenzliste durch und berechne die Gewichte
		while((g = getEdge(edges[i])) != 0){
		//	weight = sqrt(g.start*g.start + g.end*g.end);
			//addEdge(g->start,g->end,g->length_in_dm);// zurück in Liste schreiben, Frage: wird an die selbe Stelle geschrieben wie vorher?
			//ist zurückschreiben notwendig?
			weight = g->length_in_cm;
			w = g->end;
			if (distance[w] > (distance[v]+weight)){
				distance[w] = distance[v]+weight;
				parent[w] = v;
			}
			i++;
		}

		v = 1;
		dist = MAX8;
		//Finde Knoten zu dem momentan der kürzeste Weg besteht
		for (i =1; i <= map_first_free_edgeNumber; i++){
			if ((intree[i] == 0) && (dist > distance[i])){
				dist = distance[i];
				v = i;
			}
		}
	}
}

uint8_t *find_shortest_path(uint8_t start, uint8_t ziel){
	int8_t done = 0, help = 0, length;

	dijkstra(start);

	length = 0;
	way[0] = ziel;
	while (!done){
		help = parent[ziel];
		if (help != -1){
			length = length + 1;
			way[length] = help;
			ziel = help;
		}
		else done = 1;
	}
	return way;
}


