#ifndef GPS_HANDLER_H_
#define GPS_HANDLER_H_


#include "GPSmessagetypes.h"

//extern static uint8_t coordsValidFlags; //last bit for own Coords, previous one for target Coords
extern uint8_t coordsValidFlags; //last bit for own Coords, previous one for target Coords
extern uint8_t taskSet;

gps_reducedData_t* get_ownCoords(void);
void set_ownCoords(int16_t x, int16_t y, uint16_t angle);

void Init_GPS_Handler(void);


#endif /* GPS_HANDLER_H_ */
