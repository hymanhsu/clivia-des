#ifndef _HEX_H_
#define _HEX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

void bin2hex(const unsigned char *bin, size_t len, char** out);

size_t hexs2bin(const char *hex, unsigned char **out);

#ifdef __cplusplus
}
#endif

#endif
