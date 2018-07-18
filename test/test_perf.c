#include "cdes/hex.h"
#include "cdes/encrypt.h"
#include "tools.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>


extern clock_t cost_time [];

void test_perf(int argc, char** argv){
    struct timeval start;
    struct timeval end;

    int string_len = 20;
    int counter = 1000000;
    if(argc > 1){
        string_len = atoi(argv[1]);
    }
    if(argc > 2){
        counter = atoi(argv[2]);
    }
    int counter2 = counter;

    int totalCount = 0;
    gettimeofday(&start,NULL);
    while( counter-- > 0 ){
        printf("%d\n",counter);

        int length = string_len;

        while(length-- > 1){
            char * plain_text;
            genRandomString(length,&plain_text);

            char * cypher_text;
            des_encrypt((const unsigned char *)plain_text,strlen(plain_text),&cypher_text);
            
            char * plainText;
            des_decrypt(cypher_text,&plainText);
            if(strlen(plainText) == strlen(plain_text) && memcmp(plain_text,plainText,strlen(plain_text)) == 0 ){
                //printf("success!\n");
            }else{
                printf("error ! \nold=[%s]\nnew=[%s]\n",plain_text,plainText);
            }
            free(plainText);
            
            free(cypher_text);
            free(plain_text);
            totalCount++;
        }

    }
    gettimeofday(&end,NULL);

    long time_use = (end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);
    printf("Batch size is %d\n",totalCount);
    printf("time_use is %ld micro seconds\n",time_use);
    printf("COST PER ENCRYPT&DECRYPT is %ld micro seconds\n",time_use/totalCount);

    for(int i=0; i<10; ++i){
        printf("%d elapsed time (clocks) : %u \n", i, cost_time[i] );
    }

}

void test_stability(){

    int length = 1000;

    while(length-- > 1){
        printf("length = %d, ",length);
        char * plain_text;
        genRandomString(length,&plain_text);

        char * cypher_text;
        des_encrypt((const unsigned char *)plain_text,strlen(plain_text),&cypher_text);
        
        char * plainText;
        des_decrypt(cypher_text,&plainText);
        if(strlen(plainText) == strlen(plain_text) && memcmp(plain_text,plainText,strlen(plain_text)) == 0 ){
            printf("success!\n");
        }else{
            printf("error ! \nold=[%s]\nnew=[%s]\n",plain_text,plainText);
        }
        free(plainText);
        
        free(cypher_text);
        free(plain_text);
    }

}

int main(int argc, char** argv){

	unsigned int iseed = (unsigned int)time(NULL);
	srand (iseed);

    test_perf(argc, argv);
    //test_stability();

    return 0;
}
