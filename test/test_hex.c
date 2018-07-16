#include "cdes/hex.h"
#include <stdio.h>

int main(int argc, char** argv){
    const char    *a = "Test 123! - jklmn";
    char          *hex;
    unsigned char *bin;
    size_t         binlen;
 
    bin2hex((unsigned char *)a, strlen(a), &hex);
    printf("%s\n", hex);
 
    binlen = hexs2bin(hex, &bin);
    printf("%.*s\n", (int)binlen, (char *)bin);
 
    free(bin);
    free(hex);
    return 0;
}
