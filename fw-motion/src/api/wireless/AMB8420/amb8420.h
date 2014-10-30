#ifndef AMB8420_H_
#define AMB8420_H_

#include "../../base_type.h"
#include "../wireless.h"

extern boolean_t AMB8420_Init(void);
extern void AMB8420_Send(wirelessMessage_t* msg);

#endif /* AMB8420_H_ */
