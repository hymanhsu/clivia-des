#include "cdes/hex.h"
#include "cdes/encrypt.h"
#include "tools.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


void decypt_jni(){
    const char * cypher_text = "3AA615A2A3552BBEF1FC53246897F507";
    char * plainText;
    des_decrypt(cypher_text,&plainText);
    print_hex(plainText, strlen(plainText));
    printf("plainText = %s\n",plainText);
}

void encrypt_and_decrypt(){
    unsigned char des_key [8] = {'a','b','c','d','e','f','g','h'};
    const char * plain_text = "111";
    print_hex(plain_text, strlen(plain_text));
    
    printf("***********************encrypt*******************\n");

    char * cypher_text;
    des_encrypt_with_key(des_key,(unsigned char *)plain_text,strlen(plain_text),&cypher_text);
    printf("cypher_text = %s\n",cypher_text);

    printf("***********************decrypt*******************\n");

    char * plainText;
    des_decrypt(cypher_text,&plainText);
    print_hex(plainText, strlen(plainText));

    free(cypher_text);
    free(plainText);
}

void encrypt_and_decrypt_short_string(int argc, char** argv){
    char * plain_text = "播放qq音乐	ILMH9056	VIDEO	{\"domain\":\"video\",\"action\":\"query\"}";
    printf("plain_text = %s\n",plain_text);
    print_hex(plain_text, strlen(plain_text));

    printf("***********************encrypt*******************\n");

    char * cypher_text;
    des_encrypt((unsigned char *)plain_text,strlen(plain_text),&cypher_text);
    if(cypher_text){
        printf("cypher_text = %s\n",cypher_text);
    } 

    printf("***********************decrypt*******************\n");

    char * plainText;
    des_decrypt(cypher_text,&plainText);
    if(plainText){
        print_hex(plainText, strlen(plainText));
        printf("plainText = %s\n",plainText);
    }
    
    if(cypher_text){
        free(cypher_text);
    }
    if(plainText){
        free(plainText);
    }
    
}


void decypt_crash_test(){

    char * plain_text;
    genRandomString(56,&plain_text); //"I0Kmg7Y3Sg7727e4GW5777K1o0AeM43hdnoD69Gbm5j4G8x8za915CnU";

    printf("plain_text = [%s]\n",plain_text);
    printf("plain_text.len = [%d]\n",strlen(plain_text));
    
    printf("***********************encrypt*******************\n");

    char * cypher_text;
    des_encrypt((unsigned char *)plain_text,strlen(plain_text),&cypher_text);
    printf("cypher_text = %s\n",cypher_text);

    printf("***********************decrypt*******************\n");

    char * plainText;
    des_decrypt(cypher_text,&plainText);
    printf("plainText = [%s]\n",plainText);
    printf("decrypt_size = %d\n",strlen(plainText)); 
    if(strlen(plainText) == strlen(plain_text) && memcmp(plain_text,plainText,strlen(plain_text)) == 0 ){
        printf("success!\n");
    }else{
        printf("error ! \nold=[%s]\nnew=[%s]\n",plain_text,plainText);
    }

    /*unsigned char * plainText;
    size_t decrypt_size = des_decrypt(cypher_text, &plainText);
    if(decrypt_size == strlen(plain_text) && memcmp(plain_text,plainText,decrypt_size) == 0 ){
        printf("success!\n");
    }else{
        printf("error ! \nold=[%s]\nnew=[%s]\n",plain_text,plainText);
    }*/

    free(cypher_text);
    free(plainText);

}

int main(int argc, char** argv){

	unsigned int iseed = (unsigned int)time(NULL);
	srand (iseed);

    //decypt_jni();
    encrypt_and_decrypt();
    //decypt_crash_test();
    //encrypt_and_decrypt_short_string(argc,argv);

    return 0;
}
