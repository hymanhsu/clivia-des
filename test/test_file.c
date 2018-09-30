#include "cdes/hex.h"
#include "cdes/encrypt.h"
#include "cdes/buffer.h"
#include "tools.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

char global_mode = 'e';

int main(int argc, char** argv){

	unsigned int iseed = (unsigned int)time(NULL);
	srand (iseed);

    if(argc < 4){
        printf("Options: <-e|-d> <input file> <output file>\n");
        exit(1);
    }

    char * mode = argv[1];
    char * input_file = argv[2];
    char * output_file = argv[3];

    if(strcmp("-d",mode) == 0){
        global_mode = 'd';
    }
    if(strcmp("-e",mode) == 0){
        global_mode = 'e';
    }

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

    time_t t_start, t_end;
    t_start = time(NULL);
    char StrLine[10240] = {0};
    buffer * longLineBuf = NULL;
    int count = 0;
    while (!feof(input_fp)) 
    { 
        if(!fgets(StrLine,10240,input_fp)){
            break;
        }
        int i = strlen(StrLine);
        if(StrLine[i-1] == '\n'){
            StrLine[i-1] = 0;
        }else{
            //printf("too long , exit\n");
            //exit(0);
            //这行太长了，10k还没有读完一行，只能上buffer了
            longLineBuf = buffer_init();
            buffer_append_string(longLineBuf, StrLine);
            do{
                fgets(StrLine,10240,input_fp);
                i = strlen(StrLine); 
                if(StrLine[i-1] == '\n'){
                    StrLine[i-1] = 0;
                    buffer_append_string(longLineBuf, StrLine);
                    break;
                }else{
                    buffer_append_string(longLineBuf, StrLine);
                }
            }while(1);
        }
        count++;
        //printf(">> %d, %d, {{%s}}\n",count,i,StrLine);
        char * ptr = NULL;
        if(longLineBuf != NULL){
            ptr = longLineBuf->ptr;
        }else{
            ptr = StrLine;
        }
        if(global_mode == 'd'){
            char * plainText = NULL;
            des_decrypt(ptr,&plainText);
            if(plainText){
                fprintf(output_fp,"%s\n",plainText);
            }
            if(plainText){
                free(plainText);
            }
        }
        if(global_mode == 'e'){
            char * cypherText = NULL;
            des_encrypt(ptr,strlen(ptr),&cypherText);
            if(cypherText){
                fprintf(output_fp,"%s\n",cypherText);
            }
            if(cypherText){
                free(cypherText);
            }
        }
        if(longLineBuf != NULL){
            buffer_free(longLineBuf);
            longLineBuf = NULL;
        }
    } 

    fclose(input_fp);
    fclose(output_fp);

    t_end = time(NULL);
    printf("cost time: %.0f s\n", difftime(t_end,t_start)) ;

    return 0;
}


