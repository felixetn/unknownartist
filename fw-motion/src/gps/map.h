/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file map.h
 * @author mschnaubelt
 * @brief map provides the function to create a map represented as a graph and add
 *        new edges. It is also able to store specific information about edges and nodes.
 *        This map should then be stored on a SD-Card
 **/

#ifndef MAP_H_
#define MAP_H_

#include "../api/base_type.h"
#include "../cartography/mapping_controller.h"
#include "../api/defines.h"

typedef uint8_t nodeNumber;
typedef uint8_t edgeNumber;


// memory to store the map data; buffer[0] contains the max. size of the map (number of edges)
extern uint8_t *map_buffer;

// the maximum number of edges within a graph
#define MAP_MAX_EDGE_RETURN_NUMBER 15
extern edgeNumber map_edgeNumberBuffer[];

typedef struct {
	nodeNumber start;
	nodeNumber end;
	uint8_t length_in_cm;
} map_edge;

//value given when no edge found
extern const map_edge map_edge_null;

#define map_edge_size sizeof(map_edge)

//next free edge
extern edgeNumber map_first_free_edgeNumber;

extern uint8_t *map_info_buffer;

// the stored information about an edge is the side_start, side_end and the length of the edge
// 1: coming up, 2: left, 3: straight, 4: right
typedef struct {
	uint8_t crossing_side_start;
	uint8_t crossing_side_end;
	uint8_t length;
} map_edge_info;
#define EDGEINFOLENGTH sizeof(map_edge_info)

// the stored information about a node is the crossing type and the widths
typedef struct {
	CrossingType crossing_type;
	uint16_t w0;
	uint16_t w1;
	uint16_t w2;
	uint16_t w3;
} map_node_info;
#define NODEINFOLENGTH sizeof(map_node_info)

#define MAXNODEINFOS 10

extern uint8_t *nextMapEdgeInfo;

/**
 * Initializes map to work with a buffer of known length
 */
void initMap(uint8_t *b, uint16_t length);

/**
 * Returns the edge struct
 * @param edgeNumber the number whose edge struct shall be returned
 * @return the edge struct for a given edgeNumber, starting with 1, e. g. {0x00, 0x00, 0x00}
 */
map_edge *getEdge(edgeNumber);

/**
 * Return an array of edgeNumbers that are adjacent to the given nodeNumber.
 * The list ends with a zero edgeNumber. Before using this function again,
 * the returned list has to be saved!
 * @param node the node number
 * @return an array of edgeNumbers that are adjacent to the given nodeNumber.
 */
edgeNumber *getEdges(nodeNumber node);

/**
 * Adds an edge to the map
 * @param start start node
 * @param end end node
 * @param length_in_cm the length of the edge
 * @return the edge number if successful, otherwise 0.
 */
uint8_t addEdge(nodeNumber start, nodeNumber end, uint8_t length_in_cm);

/**
 * Print all Edges using printEdgeInfo
 */
void printEdgeList(void);

/*
 * Initialize a given buffer to hold the map edge/node info
 * @param b pointer to the buffer
 * @param length length of the buffer
 * TODO Currently implements one sector only!
 */
void initMapInfo(uint8_t *b, uint16_t length);

/*
 * Returns the information about a node
 * @param n the node number
 * @return a pointer to the struct containing the information about the node
 */
map_node_info *getNodeInfo(nodeNumber n);

/*
 * Returns the information about an edge
 * @param e the number of the edge
 * @return a pointer to a struct containing the edge info for a given edgeNumber
 */
map_edge_info *getEdgeInfo(edgeNumber e);

/*
 * Appends a new edge to map info
 * @param side_start start side of the edge (1, 2, 3, 4)
 * @param side_end end side of the edge
 * @param l the length of the edge
 * @param data the data array which contains the us sensor values
 */
void addEdgeInfo(uint8_t side_start, uint8_t side_end, uint8_t l, uint8_t *data);

/*
 * print edge info
 * @param e the number of the edge
 */
void printEdgeInfo(edgeNumber e);

/*
 * print node info
 * @param n the number of the node
 */
void printNodeInfo(nodeNumber n);

/*
 * wrapper method that adds information about a node, see also map_node_info
 * @param n the number of the node
 * @param node_info the information about the node
 */
void addNodeInfo(nodeNumber n, map_node_info node_info);

#endif /* MAP_H_ */
