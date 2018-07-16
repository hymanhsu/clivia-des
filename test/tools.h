#ifndef _TOOLS_H_
#define _TOOLS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

void genRandomString(int length, char** output);

void print_hex(unsigned char * str, size_t len);

#ifdef __cplusplus
}
#endif

#endif