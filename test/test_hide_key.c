#include "cdes/hex.h"
#include "cdes/encrypt.h"
#include "tools.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char** argv){
    const char * source_old = "ab";
    const char * key_old = "12345678";

    size_t len = strlen(source_old)+strlen(key_old);
    char * source = (char *)malloc(len*sizeof(char));
    memset(source,0,len);
    strncpy(source,source_old,strlen(source_old));
    print_hex(source, len);

    size_t len2 = strlen(key_old) + 1;
    char * key = (char *)malloc(len2*sizeof(char));
    memset(key,0,len2);
    strncpy(key,key_old,strlen(key_old));
    
    ////

    printf("*********************hide key*********************\n");

    hide_key((unsigned char*)source, len, (unsigned char*)key, len2-1);
    print_hex(source, len);

    printf("**********************restore key********************\n");

    unsigned char new_key [9];
    memset(new_key,0,9);
    restore_key((unsigned char*)source, len, new_key, 8);
    printf("new_key = %s\n",new_key);

    print_hex(source, len);

    free(source);
    free(key);

    return 0;
}

