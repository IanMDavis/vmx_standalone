#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "aesp8-ppc.h"
#include <openssl/aes.h>

#define BLOCK 16

char buffer[2048];

char *array2str(char *array, int size) {
	int i;
	char *buffer_ptr = buffer;

	for (i = 0; i < size; i++)
	{
		if (i != 0 && i % 4 == 0) {
			if (i % 16 == 0) sprintf(buffer_ptr, "\n");
			else sprintf(buffer_ptr, " ");
			buffer_ptr++;
		}
		sprintf(buffer_ptr, "%02x", *(array + i));
		buffer_ptr += 2;
	}
	buffer_ptr = 0;
	return buffer;
}

int main(int argc, char **argv) {

	unsigned char key[] = {
		0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
		0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
		
	unsigned char txt[] = {
		0x4d, 0x69, 0x64, 0x6e, 0x69, 0x67, 0x68, 0x74,
		0x5f, 0x4d, 0x61, 0x72, 0x6c, 0x69, 0x6e, 0x05,
		0x52, 0x69, 0x63, 0x68, 0x61, 0x72, 0x64, 0x52,
		0x69, 0x63, 0x68, 0x61, 0x72, 0x64, 0x06};

	unsigned char iv[] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xd0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	unsigned char* myKey = "Pampers baby-dry";

	int input_length = sizeof(txt);
	
	int padding_size;
	if(input_length >= BLOCK)
		padding_size = BLOCK - (input_length % BLOCK);
	else
		padding_size = BLOCK - input_length;

	int total_length = input_length + padding_size;

	
	unsigned char * padded_input = calloc(total_length, sizeof(char));
	memcpy(padded_input, txt, input_length);
	memset(padded_input + input_length, padding_size, padding_size);


	printf("\nInput length: %d Padding size: %d Total size: %d\n", input_length, padding_size, total_length);

	for(int i = 0; i < sizeof(iv); i++){
		padded_input[i] = iv[i] ^ padded_input[i];
	}

	struct aes_key enc_key;
	struct aes_key dec_key;

	unsigned char encoded[total_length];
	unsigned char decoded[total_length];

	int keylen = 16;

	//printf("key (%d):\n%s\n\n", keylen, array2str(key, keylen));

	aes_p8_set_encrypt_key(myKey, keylen * 8, &enc_key);
	//printf("key encode:\n%s\n\n", array2str(enc_key.key, 16*15));

	aes_p8_set_decrypt_key(myKey, keylen * 8, &dec_key);
	//printf("key decode:\n%s\n\n", array2str(dec_key.key, 16*15));

	printf("string:\n%s\n\n", array2str(padded_input, total_length)); 

	aes_p8_encrypt(txt, encoded, &enc_key);
	printf("aes_p8_encrypt encoded:\n%s\n\n", array2str(encoded, BLOCK));
	aes_p8_decrypt(encoded, decoded, &dec_key);
	printf("aes_p8_decrypt decoded (should be == string):\n%s\n\n", array2str(decoded, BLOCK));

	aes_p8_cbc_encrypt(padded_input, encoded, total_length, &enc_key, iv, AES_ENCRYPT);
	printf("aes_p8_cbc_encrypt encoded:\n%s\n\n", array2str(encoded, total_length));
	aes_p8_cbc_encrypt(encoded, decoded, total_length, &dec_key, iv, AES_DECRYPT);
	for(int i = 0; i < sizeof(iv); i++){
		decoded[i] = iv[i] ^ decoded[i];
	}
	printf("aes_p8_cbc_decrypt decoded (should be == string):\n%s\n\n", array2str(decoded, total_length));

	free(padded_input);
	return 0;
}
