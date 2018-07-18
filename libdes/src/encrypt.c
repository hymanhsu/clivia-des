#ifdef USED_BY_PYTHON
#include <Python.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#endif

#include "cdes/des.h"
#include "cdes/hex.h"
#include "cdes/helper.h"


COST_RESULT(cost_time,10);

static void insert_byte(unsigned char* source_bytes, size_t source_len, size_t insert_pos, unsigned char byte)
{
	for(int pos = source_len - 1; pos > insert_pos; pos--){
		*(source_bytes + pos) = *(source_bytes + pos - 1);
	}
	*(source_bytes + insert_pos) = byte;
}


static unsigned char extract_byte(unsigned char* source_bytes, size_t source_len, size_t insert_pos)
{
	unsigned char temp = *(source_bytes + insert_pos);
	for(int pos = insert_pos; pos < source_len-1; pos++){
		*(source_bytes + pos) = *(source_bytes + pos + 1);
	}
	*(source_bytes + source_len - 1) = '\0';
	return temp;
}


void hide_key(unsigned char* source_bytes, size_t source_len, unsigned char* des_key, size_t des_key_len)
{
	if(source_len < 10){
		printf("source string length < 10! exit when hide_key()\n");
		exit(3);
	}
	print_hexstr("source_bytes",source_bytes, source_len);
	my_debug("source_len = %d\n",source_len);
	print_hexstr("des_key",des_key, des_key_len);
	my_debug("des_key_len = %d\n",des_key_len);
	size_t des_key_index = 0;
	memset(source_bytes+source_len-des_key_len, 0, des_key_len);
	size_t empty_count = des_key_len;
	do{
		my_debug("-------- %u\n",des_key_index);
		unsigned char value_byte = *(source_bytes + des_key_index);
		my_debug("value_byte = %x\n",value_byte);
		unsigned char key_byte = *(des_key + des_key_index);
		my_debug("key_byte = %x\n",key_byte);
		my_debug("len = %d\n", (source_len - empty_count - des_key_index - 1) );
		size_t insert_pos = (size_t)value_byte % (source_len - empty_count - des_key_index - 1) + des_key_index + 1;
		my_debug("insert_pos = %u\n",insert_pos);
		insert_byte(source_bytes, source_len, insert_pos, key_byte);
		print_hexstr("source_bytes",source_bytes, source_len);
		des_key_index ++;
		empty_count --;
	}while(des_key_index < des_key_len);
}


void restore_key(unsigned char* source_bytes, size_t source_len, unsigned char* des_key, size_t des_key_len)
{
	if(source_len < 10){
		printf("source string length < 10! exit when restore_key()\n");
		exit(3);
	}
	print_hexstr("source_bytes",source_bytes, source_len);
	my_debug("source_len = %d\n",source_len);
	print_hexstr("des_key",des_key, des_key_len);
	my_debug("des_key_len = %d\n",des_key_len);
	size_t des_key_index = 0, des_key_idx = 0;
	size_t restore_count = 0;
	do{
		des_key_idx = des_key_len - des_key_index - 1;
		my_debug("-------- %u\n",des_key_idx);
		unsigned char value_byte = *(source_bytes + des_key_idx);
		my_debug("value_byte = %x\n",value_byte);
		my_debug("len = %d\n", (source_len - des_key_idx - 2 - restore_count) );
		size_t insert_pos = (size_t)value_byte % (source_len - des_key_idx - 2 - restore_count) + des_key_idx + 1;
		my_debug("insert_pos = %u\n",insert_pos);
		unsigned char key_byte = extract_byte(source_bytes, source_len, insert_pos);
		my_debug("key_byte = %x\n",key_byte);
		*(des_key + des_key_idx) = key_byte;
		print_hexstr("source_bytes",source_bytes, source_len);
		des_key_index ++;
		restore_count ++;
	}while(des_key_index < des_key_len);
	//memset(source_bytes+source_len-des_key_len, 0, des_key_len);
}


void des_encrypt_with_key(unsigned char * des_key,const unsigned char* input_plain_text, size_t input_len, char** output)
{
	print_hexstr("input_plain_text",input_plain_text,input_len);
	print_hexstr("des_key",des_key,DES_KEY_SIZE);

	COST_START(1);
	// Generate DES key set
	key_set key_sets [ DES_KEY_SET_SIZE ];
	memset(key_sets,0,DES_KEY_SET_SIZE*sizeof(key_set));
	generate_sub_keys(des_key, key_sets);
	print_hexstr("key_sets",key_sets,DES_KEY_SET_SIZE * sizeof(key_set));
	COST_END(1);
    COST_ACCUM(cost_time,1);

    // DES encrypt
	my_debug("input_len = %d\n",input_len);
	short int process_mode = ENCRYPTION_MODE;
	unsigned long block_count = 0;
	unsigned long number_of_blocks = input_len/DES_KEY_SIZE + ((input_len % DES_KEY_SIZE)?1:0);
	my_debug("number_of_blocks = %u\n",number_of_blocks);
	unsigned short int padding = DES_KEY_SIZE - input_len % DES_KEY_SIZE;
	my_debug("padding = %d\n",padding);
	unsigned long output_len = DES_KEY_SIZE * number_of_blocks + ((padding==DES_KEY_SIZE)?DES_KEY_SIZE:0);
	my_debug("output_len = %d\n",output_len);

	COST_START(2);
	unsigned char data_block [DES_KEY_SIZE];
	unsigned char processed_block [DES_KEY_SIZE];
	//output_data长度 = blocks需要的字节数 + padding需要的字节数 + 插入main key需要的字节数
	unsigned char* output_data = (unsigned char*) malloc ( (output_len+DES_KEY_SIZE) * sizeof(unsigned char) );
	if(!output_data){
		perror("malloc failed ");
		exit(3);
	}
	memset(output_data, 0, (output_len+DES_KEY_SIZE) * sizeof(unsigned char) );
	unsigned char* output_data_ptr = output_data;
	size_t nread = 0;
	size_t bytes_written = 0;
	do{
		size_t needRead = (input_len-nread >= DES_KEY_SIZE) ? DES_KEY_SIZE : input_len-nread;
		memcpy(data_block, input_plain_text + nread, needRead );
		nread += needRead;
		block_count++;
		if (block_count == number_of_blocks) {
			if (padding < DES_KEY_SIZE) { // Fill empty data block bytes with padding
				memset((data_block + DES_KEY_SIZE - padding), (unsigned char)padding, padding);
				print_hexstr("data_block",data_block,DES_KEY_SIZE);
			}

			process_message(data_block, processed_block, key_sets, process_mode);
			print_hexstr("processed_block",processed_block,DES_KEY_SIZE);
			memcpy(output_data_ptr, processed_block, DES_KEY_SIZE);
			output_data_ptr += DES_KEY_SIZE;
			bytes_written += DES_KEY_SIZE;

			if (padding == DES_KEY_SIZE) { // Write an extra block for padding
				memset(data_block, (unsigned char)padding, DES_KEY_SIZE);
				process_message(data_block, processed_block, key_sets, process_mode);
				memcpy(output_data_ptr, processed_block, DES_KEY_SIZE);
				output_data_ptr += DES_KEY_SIZE;
				bytes_written += DES_KEY_SIZE;
			}
		} else {
			process_message(data_block, processed_block, key_sets, process_mode);				
			memcpy(output_data_ptr, processed_block, DES_KEY_SIZE);
			output_data_ptr += DES_KEY_SIZE;
			bytes_written += DES_KEY_SIZE;
		}
		memset(data_block, 0, DES_KEY_SIZE);
	}while(nread < input_len);
	my_debug("nread = %d\n",nread);
	my_debug("bytes_written = %d\n",bytes_written);
	print_hexstr("ouput_data",output_data,output_len+DES_KEY_SIZE);
	COST_END(2);
    COST_ACCUM(cost_time,2);

	COST_START(3);
	// Hide main key into cypher text
	hide_key(output_data, output_len+DES_KEY_SIZE, des_key, DES_KEY_SIZE);
	COST_END(3);
    COST_ACCUM(cost_time,3);

	COST_START(4);
	// Transfer to hex
	bin2hex(output_data, output_len+DES_KEY_SIZE, output);
	COST_END(4);
    COST_ACCUM(cost_time,4);

	// Free
	free(output_data);	
}


void des_encrypt(const unsigned char* input_plain_text, size_t input_len, char** output)
{

	if(input_plain_text == NULL || input_len == 0){
		return;
	}

	COST_START(0);
	// Generate main key
	unsigned char des_key [DES_KEY_SIZE];
	//memset(des_key,0,DES_KEY_SIZE);
	generate_key(des_key);
	COST_END(0);
    COST_ACCUM(cost_time,0);

	des_encrypt_with_key(des_key,input_plain_text,input_len,output);
}


void des_decrypt(const char *cypher_text, char** output)
{

	if(cypher_text == NULL){
		return;
	}

	// Transfer to binary
	unsigned char *bin;
	size_t binlen = hexs2bin(cypher_text, &bin);

	// Restore main key from cypher text
	unsigned char des_key [DES_KEY_SIZE];
	restore_key(bin, binlen, des_key, DES_KEY_SIZE);
	print_hexstr("des_key",des_key,DES_KEY_SIZE);

	// Generate DES key set
	key_set key_sets [ DES_KEY_SET_SIZE ];
	memset(key_sets,0,DES_KEY_SET_SIZE*sizeof(key_set));
	generate_sub_keys(des_key, key_sets);
	print_hexstr("key_sets",key_sets,DES_KEY_SET_SIZE * sizeof(key_set));

	// DES decrypt
	size_t input_len = binlen - DES_KEY_SIZE;
	print_hexstr("input_data",bin,binlen);
	my_debug("input_len = %d\n",input_len);
	short int process_mode = DECRYPTION_MODE;
	unsigned long block_count = 0;
	unsigned long number_of_blocks = input_len/DES_KEY_SIZE + ((input_len % DES_KEY_SIZE)?1:0);
	my_debug("number_of_blocks = %d\n",number_of_blocks);
	unsigned long output_len = DES_KEY_SIZE * number_of_blocks;
	my_debug("output_len = %d\n",output_len);

	unsigned char data_block [DES_KEY_SIZE];
	unsigned char processed_block [DES_KEY_SIZE];
	//output_data长度 = blocks需要的字节数 + padding需要的字节数
	*output = (char*) malloc ( (output_len+DES_KEY_SIZE) * sizeof(char) );
	if(NULL == *output){
		perror("malloc failed ");
		exit(3);
	}
	unsigned char* output_data = *output;
	unsigned char* output_data_ptr = output_data;
	size_t nread = 0;
	size_t bytes_written = 0;
	do{
		size_t needRead = (input_len-nread >= DES_KEY_SIZE) ? DES_KEY_SIZE : input_len-nread;
		memcpy(data_block, bin + nread, needRead );
		nread += needRead;
		block_count++;
		if (block_count == number_of_blocks) {
			print_hexstr("data_block",data_block,DES_KEY_SIZE);
			process_message(data_block, processed_block, key_sets, process_mode);
			print_hexstr("processed_block",processed_block,DES_KEY_SIZE);
			unsigned short int padding = processed_block[7];
			my_debug("padding = %d\n",padding);

			if (padding < DES_KEY_SIZE) {
				size_t count = DES_KEY_SIZE - padding;
				memcpy(output_data_ptr, processed_block, count);
				output_data_ptr += count;
				bytes_written += count;
			}
		} else {
			process_message(data_block, processed_block, key_sets, process_mode);
			memcpy(output_data_ptr, processed_block, DES_KEY_SIZE);
			output_data_ptr += DES_KEY_SIZE;
			bytes_written += DES_KEY_SIZE;
		}
		memset(data_block, 0, DES_KEY_SIZE);
	}while(nread < input_len);
	my_debug("nread = %d\n",nread);
	my_debug("bytes_written = %d\n",bytes_written);

	*(output_data + bytes_written) = '\0';
	print_hexstr("output_data",output_data,output_len+DES_KEY_SIZE);
	my_debug("output_data string = [%s]\n",output_data);

	// Free
	free(bin);
}

