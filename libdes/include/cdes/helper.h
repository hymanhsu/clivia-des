#ifndef _HELPER_H_
#define _HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

void print_hexstr(const char* msg, unsigned char * str, size_t len);

void my_debug(const char *fmt, ...);


#ifdef __cplusplus
}
#endif

#endif
