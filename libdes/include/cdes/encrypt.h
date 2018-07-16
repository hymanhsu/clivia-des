#ifndef _ENCRYPT_H_
#define _ENCRYPT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

void hide_key(unsigned char* source_bytes, size_t source_len, unsigned char* des_key, size_t des_key_len);

void restore_key(unsigned char* source_bytes, size_t source_len, unsigned char* des_key, size_t des_key_len);

void des_encrypt_with_key(unsigned char * key,const unsigned char* input_plain_text, size_t input_len, char** output);

void des_encrypt(const unsigned char* input_plain_text, size_t input_len, char** output);

void des_decrypt(const char *cypher_text, char** output);


#ifdef __cplusplus
}
#endif

#endif
