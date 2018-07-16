#ifdef USED_BY_PYTHON
#include <Python.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#endif

#include "cdes/helper.h"
#include "cdes/hex.h"


#ifdef DEBUG_FLAG

void print_hexstr(const char* msg, unsigned char * str, size_t len){
	printf("%s : len = [%u]\n",msg,len);
    char *hex = NULL;
    bin2hex(str, len, &hex);
    if(hex){
        size_t hexlen = strlen(hex);
        for(size_t n=0; n<hexlen; n+=2){
            printf("%c%c(%d) ",*(hex+n),*(hex+n+1),n/2);
        }
        printf("\n");
        free(hex);
    }
}

void my_debug(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
}

#else

void print_hexstr(const char* msg, unsigned char * str, size_t len)
{
}

void my_debug(const char *fmt, ...)
{
}

#endif

