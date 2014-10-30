#ifndef PRINTF_STDARG_H_
#define PRINTF_STDARG_H_

#include <stdarg.h>

extern int print( char **out, const char *format, va_list args );

extern int printf(const char *format, ...);

extern int sprintf(char *out, const char *format, ...);

extern int snprintf( char *buf, unsigned int count, const char *format, ... );

extern int write( int i, char* c, int n);

#endif /* PRINTF_STDARG_H_ */
