#ifndef _HELPER_H_
#define _HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <time.h>

void print_hexstr(const char* msg, unsigned char * str, size_t len);

void my_debug(const char *fmt, ...);

#ifdef COST_FLAG
#define COST_RESULT(result,size) extern clock_t result []
#define COST_START(i) clock_t start##i = clock()
#define COST_END(i) clock_t end##i = clock()
#define COST_VALUE(i) (end##i - start##i)
#define COST_ACCUM(result,i)  *((result)+(i)) += (end##i - start##i)
#else
#define COST_RESULT(result,size)
#define COST_START(i) 
#define COST_END(i) 
#define COST_VALUE(i) 
#define COST_ACCUM(result,i) 
#endif

#ifdef __cplusplus
}
#endif

#endif
