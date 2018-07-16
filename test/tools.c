#include "tools.h"
#include "cdes/hex.h"
#include "cdes/encrypt.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


void genRandomString(int length, char** output)
{  
    int flag, i;  
    srand((unsigned) time(NULL ));  
    if ((*output = (char*) malloc(length+1)) == NULL )  
    {  
        printf("Malloc failed!flag:14\n");  
        *output = NULL;
    }  
  
    for (i = 0; i < length; i++)  
    {  
        flag = rand() % 3;  
        switch (flag)  
        {  
            case 0:  
                (*output)[i] = 'A' + rand() % 26;  
                break;  
            case 1:  
                (*output)[i] = 'a' + rand() % 26;  
                break;  
            case 2:  
                (*output)[i] = '0' + rand() % 10;  
                break;  
            default:  
                (*output)[i] = 'x';  
                break;  
        }  
    }  
    (*output)[length] = '\0';
}


void print_hex(unsigned char * str, size_t len){
    char *hex = NULL;
    bin2hex(str, len, &hex);
    if(hex){
        printf("%s\n",hex);
        free(hex);
    }
}


