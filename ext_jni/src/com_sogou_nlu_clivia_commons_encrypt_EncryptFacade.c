#include "com_sogou_nlu_clivia_commons_encrypt_EncryptFacade.h"
#include "cdes/encrypt.h"
#include "cdes/helper.h"

#include <string.h>
#include <stdlib.h>


/*
 * Class:     com_sogou_nlu_clivia_commons_encrypt_EncryptFacade
 * Method:    encrypt
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_sogou_nlu_clivia_commons_encrypt_EncryptFacade_encrypt
  (JNIEnv * env, jclass cls, jstring plainString_)
{
    //unsigned char des_key [8] = {'a','b','c','d','e','f','g','h'};
    const char * plainString = (*env)->GetStringUTFChars(env, plainString_, 0);

    char * cypher_text = NULL;
    //printf("plainString = [%s]\n",plainString);
    des_encrypt((unsigned char *)plainString,strlen(plainString),&cypher_text);
    //printf("cypher_text = [%s]\n",cypher_text);

    jstring result;
    if(cypher_text){
        result = (*env)->NewStringUTF(env,cypher_text); 
        free(cypher_text);
    }

    (*env)->ReleaseStringUTFChars(env, plainString_, plainString);
    return result;
}

/*
 * Class:     com_sogou_nlu_clivia_commons_encrypt_EncryptFacade
 * Method:    decrypt
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_sogou_nlu_clivia_commons_encrypt_EncryptFacade_decrypt
  (JNIEnv * env, jclass cls, jstring cypherString_)
{
    const char * cypherString = (*env)->GetStringUTFChars(env, cypherString_, 0);

    char * plain_text = NULL;
    des_decrypt(cypherString,&plain_text);

    jstring result;
    if(plain_text){
        result = (*env)->NewStringUTF(env,plain_text); 
        free(plain_text);
    }

    (*env)->ReleaseStringUTFChars(env, cypherString_, cypherString);
    return result;
}
