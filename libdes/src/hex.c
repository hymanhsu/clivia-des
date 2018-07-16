#ifdef USED_BY_PYTHON
#include <Python.h>
#else
#include <string.h>
#include <stdio.h>
#include <errno.h>
#endif

#include "cdes/hex.h"


void bin2hex(const unsigned char *bin, size_t len, char** out)
{
    size_t  i;

    static char * chrs = "0123456789ABCDEF";
 
    if (bin == NULL || len == 0)
        return;
    
    char temp;
    *out = (char *) malloc (len*2+1);
    if(NULL == *out){
        perror("malloc failed ");
        exit(3);
    }
    for (i=0; i<len; i++) {
        //printf("one = [%c]\n",one);
        temp = chrs [ bin[i] >> 4 ];
        //printf("temp = [%c]\n",temp);
        (*out)[i*2] = temp;
        //printf("%x(%d)\n",(*out)[i*2],i*2);
        temp = chrs [ bin[i] & 0x0F ];
        //printf("temp = [%c]\n",temp);
        (*out)[i*2+1] = temp;
        //printf("%x(%d)\n",(*out)[i*2+1],i*2+1);
    }
    (*out)[len*2] = '\0';

}


int hexchr2bin(const char hex, char *out)
{
    if (out == NULL)
        return 0;
 
    if (hex >= '0' && hex <= '9') {
        *out = hex - '0';
    } else if (hex >= 'A' && hex <= 'F') {
        *out = hex - 'A' + 10;
    } else if (hex >= 'a' && hex <= 'f') {
        *out = hex - 'a' + 10;
    } else {
        return 0;
    }
 
    return 1;
}


size_t hexs2bin(const char *hex, unsigned char **out)
{
    size_t len;
    char   b1;
    char   b2;
    size_t i;
 
    if (hex == NULL || *hex == '\0' || out == NULL)
        return 0;
 
    len = strlen(hex);
    if (len % 2 != 0)
        return 0;
    len /= 2;
 
    *out = (unsigned char *)malloc(len);
    memset(*out, 'A', len);
    for (i=0; i<len; i++) {
        if (!hexchr2bin(hex[i*2], &b1) || !hexchr2bin(hex[i*2+1], &b2)) {
            return 0;
        }
        (*out)[i] = (b1 << 4) | b2;
    }
    return len;
}

