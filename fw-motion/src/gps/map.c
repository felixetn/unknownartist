/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file map.c
 * @author mschnaubelt
 * @brief map provides the function to create a map represented as a graph and add
 *        new edges. It is also able to store specific information about edges and nodes.
 *        This map should then be stored on a SD-Card
 */

#include "map.h"

uint8_t *map_buffer;
edgeNumber map_edgeNumberBuffer[MAP_MAX_EDGE_RETURN_NUMBER];
const map_edge map_edge_null = { 0x00, 0x00, 0x00 };
edgeNumber map_first_free_edgeNumber;
uint8_t *map_info_buffer;
uint8_t *nextMapEdgeInfo;

void initMap(uint8_t *b, uint16_t length) {
	uint16_t i;
	map_buffer = b;
	map_buffer[0] = length / map_edge_size;

#ifdef MAP_DEBUG
	printf("initializing map memory for %d edges at %d... \r\n", map_buffer[0],	map_buffer);
#endif
	map_first_free_edgeNumber = 1;

	// init buffer
	for (i = 1; i < map_buffer[0] * map_edge_size; i++)
		map_buffer[i] = 0;
	// mark map info buffer as uninitialized
	map_info_buffer = 0;
}

map_edge *getEdge(edgeNumber n) {
	if (n > map_buffer[0])
		return (map_edge*)&map_edge_null;
	else
		return (map_edge*) &map_buffer[n * map_edge_size + 1];
}

edgeNumber *getEdges(nodeNumber node) {
	uint8_t i;
	uint8_t j;
	map_edge *edge;

	// Clear edge list
	for (i = 0; i < MAP_MAX_EDGE_RETURN_NUMBER; i++)
		map_edgeNumberBuffer[i] = 0;

	i = 0;
	j = 0;
	edge = (map_edge*) &map_buffer[1];

	// Search edge list and add all edges to the buffer that are adjacent to the given node
	while ((edge++)->end != 0) {
#ifdef MAP_DEBUG
		printf("Checking edge %p", edge);
#endif
		if (edge->end == node || edge->start == node) {
			map_edgeNumberBuffer[i] = j;
			i++;
		}

		j++;
	}

	return map_edgeNumberBuffer;
}

uint8_t addEdge(nodeNumber start, nodeNumber end, uint8_t length_in_cm) {

	map_edge* edge;

	// is the map buffer full?
	if (map_first_free_edgeNumber > map_buffer[0])
		return 0;
	// are the nodes greater than zero?
	if (start < 0 || end < 0)
		return 0;

	// create edge, add it to the map_buffer and assign it its values
	edge = (map_edge*) &map_buffer[(map_first_free_edgeNumber - 1)
	                               * map_edge_size + 1];

	edge->start = start;
	edge->end = end;
	edge->length_in_cm = length_in_cm;

	map_first_free_edgeNumber++;

#ifdef MAP_DEBUG
	printf("Added edge #%d: (%d, %d), length %dcm\r\n",
			map_first_free_edgeNumber - 1, start, end, length_in_cm);
#endif

	return map_first_free_edgeNumber - 1;

}

void printEdgeList(void) {
	int i = 0;
	map_edge *edge;

	// iterate through the map buffer and print all edges
	while (i < map_first_free_edgeNumber - 1) {
		edge = (map_edge*) &map_buffer[i * map_edge_size + 1];
		printEdgeInfo(i);
		printf("%d %d %d\r\n", edge->start, edge->end, edge->length_in_cm);
		i++;
	}
}

void initMapInfo(uint8_t *b, uint16_t length) {
	uint16_t i;
	map_info_buffer = b;
	map_info_buffer[0] = length;
#ifdef MAP_DEBUG
	printf("initializing map info memory with %d bytes at %d... ",
			map_info_buffer[0], map_info_buffer);
#endif
	// init map_info_buffer with zeros
	for (i = 1; i < length; i++)
		map_info_buffer[i] = 0;
#ifdef MAP_DEBUG
	printf("done. \r\n");
#endif
	nextMapEdgeInfo = &map_info_buffer[1 + MAXNODEINFOS * EDGEINFOLENGTH];
}

map_node_info *getNodeInfo(nodeNumber n) {
	map_node_info *info;

	// out of bounds?
	if (n >= MAXNODEINFOS)
		return 0;

	info = (map_node_info*) &map_info_buffer[1 + n * NODEINFOLENGTH];
	return info;
}

map_edge_info *getEdgeInfo(edgeNumber e) {
	map_edge_info *info;
	int i;
	// get the map info
	info = (map_edge_info*) &map_info_buffer[1
	                                         + MAXNODEINFOS * NODEINFOLENGTH];

	for (i = 0; i < e - 1; i++) { // Seek forward to desired edge
		uint8_t *bytePointer = (uint8_t*) info;
		bytePointer += EDGEINFOLENGTH + 2 * info->length;
		info = (map_edge_info*) bytePointer;
	}
	return info;
}

void addEdgeInfo(uint8_t side_start, uint8_t side_end, uint8_t l, uint8_t *data) {

	map_edge_info *info;
	uint8_t i;

	// create a map_edge_info and assign it its values
	info = (map_edge_info*) nextMapEdgeInfo;
	info->crossing_side_start = side_start;
	info->crossing_side_end = side_end;
	info->length = l;
	nextMapEdgeInfo += EDGEINFOLENGTH;
	for (i = 0; i < 2 * l; i++) {
		nextMapEdgeInfo[i] = data[i];
	}

	nextMapEdgeInfo += 2 * l;
}

void printEdgeInfo(edgeNumber e) {
	map_edge_info *info = getEdgeInfo(e);
	if (info != 0)
		printf("Edge %d with data length %d: %d %d\r\n", e, info->length,
				info->crossing_side_start, info->crossing_side_end);
}

void printNodeInfo(nodeNumber n) {
	map_node_info *info = getNodeInfo(n);
	if (info != 0)
		printf("Node %d of type %d: %d %d %d %d\r\n", n, info->crossing_type,
				info->w0, info->w1, info->w2, info->w3);
}

void addNodeInfo(nodeNumber n, map_node_info node_info) {
	// get the node info and assign it directly its values
	map_node_info * helper = getNodeInfo(n);
	helper->crossing_type = node_info.crossing_type;
	helper->w0 = node_info.w0;
	helper->w1 = node_info.w1;
	helper->w2 = node_info.w2;
	helper->w3 = node_info.w3;
#ifdef MAP_DEBUG
	printf("NodeInfo added to node %d \r\n", n);
#endif

}

