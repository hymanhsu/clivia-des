/*
 * des.h provides the following functions and constants:
 *
 * generate_key, generate_sub_keys, process_message, ENCRYPTION_MODE, DECRYPTION_MODE
 *
 */

#ifndef _DES_H_
#define _DES_H_

#ifdef __cplusplus
extern "C" {
#endif

// DES key is 8 bytes long
#define DES_KEY_SIZE 8
#define DES_KEY_SET_SIZE 17

#define ENCRYPTION_MODE 1
#define DECRYPTION_MODE 0

typedef struct {
	unsigned char k[8];
	unsigned char c[4];
	unsigned char d[4];
} key_set;

void generate_key(unsigned char* key);

void generate_sub_keys(unsigned char* main_key, key_set* key_sets);

void process_message(unsigned char* message_piece, unsigned char* processed_piece, key_set* key_sets, int mode);

#ifdef __cplusplus
}
#endif

#endif
