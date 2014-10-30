/*
 * Config.h
 *
 *  Created on: 13.09.2012
 *      Author: Jan
 */

#ifndef CONFIG_H_
#define CONFIG_H_

// good compiler:
//#define TRACE(x) do { if (DEBUG) printf x; } while (0)

// bad compiler:
#if DEBUG
#define TRACE(x) printf x;
#else
#define TRACE(x)
#endif


// choose which port you want to use
#define MPNSync_send(x,y,z)		0

//#define MPNSync_send(x,y,z)		(0)



#endif /* CONFIG_H_ */
