#include "cdes/hex.h"
#include "cdes/encrypt.h"
#include "tools.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


int main(int argc, char** argv){

	unsigned int iseed = (unsigned int)time(NULL);
	srand (iseed);

    if(argc < 4){
        printf("test_file <-e|-d> <encrypted file> <decrypted file>\n");
        exit(1);
    }

    char * mode = argv[1];
    char * input_file = argv[2];
    char * output_file = argv[3];

    FILE *input_fp; 
    if((input_fp = fopen(input_file,"r")) == NULL){
        perror("open input file failed");
        exit(1);
    }
    printf("input file : %s\n",input_file);

    FILE *output_fp; 
    if((output_fp = fopen(output_file,"w")) == NULL){
        perror("open output file failed");
        exit(1);
    }
    printf("output file : %s\n",output_file);

    char StrLine[4096] = {0};
    while (!feof(input_fp)) 
    { 
        fgets(StrLine,4095,input_fp);
        //printf("[%s]\n",StrLine);
        int i = strlen(StrLine); //去掉回车
        if(StrLine[i-1]=='\n'){
            StrLine[i-1]=0;
        }
        if(strcmp("-d",mode) == 0){
            char * plainText = NULL;
            des_decrypt(StrLine,&plainText);
            if(plainText){
                fprintf(output_fp,"%s\n",plainText);
            }
            if(plainText){
                free(plainText);
            }
        }
        if(strcmp("-e",mode) == 0){
            char * cypherText = NULL;
            des_encrypt(StrLine,strlen(StrLine),&cypherText);
            if(cypherText){
                fprintf(output_fp,"%s\n",cypherText);
            }
            if(cypherText){
                free(cypherText);
            }
        }
    } 

    fclose(input_fp);
    fclose(output_fp);

    return 0;
}


