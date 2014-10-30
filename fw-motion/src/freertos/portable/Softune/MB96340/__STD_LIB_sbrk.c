/* THIS SAMPLE CODE IS PROVIDED AS IS AND IS SUBJECT TO ALTERATIONS. FUJITSU */
/* MICROELECTRONICS ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR */
/* ELIGIBILITY FOR ANY PURPOSES.                                             */
/*                 (C) Fujitsu Microelectronics Europe GmbH                  */
/*---------------------------------------------------------------------------
  __STD_LIB_sbrk.C
  - Used by heap_3.c for memory accocation and deletion.

/*---------------------------------------------------------------------------*/

#include "FreeRTOSConfig.h"
#include <stdlib.h>

	static  long         brk_siz  =  0;
	typedef int          _heap_t;
	#define ROUNDUP(s)   (((s)+sizeof(_heap_t)-1)&~(sizeof(_heap_t)-1))
	static  _heap_t      _heap[ROUNDUP(configTOTAL_HEAP_SIZE)/sizeof(_heap_t)];
	#define              _heap_size      ROUNDUP(configTOTAL_HEAP_SIZE)

#if ANALYZE_MEMORY == 1
	long maxHeapUsage = 0;
#endif

	extern  char  *sbrk(int  size)
	{
	   if  (brk_siz  +  size  >  _heap_size  ||  brk_siz  +  size  <  0)
          return((char*)-1);
	   brk_siz  +=  size;

#if ANALYZE_MEMORY == 1
	   if (brk_siz > maxHeapUsage)
		   maxHeapUsage = brk_siz;
#endif
	   return(  (char*)_heap  +  brk_siz  -  size);
	}

