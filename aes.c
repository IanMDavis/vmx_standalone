#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "aesp8-ppc.h"

#define BLOCK 16

char buffer[512];

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

	unsigned char key[16] = {
		0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
		0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

	unsigned char txt[16] = {
		0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
		0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

	unsigned char iv[16] = {
		0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x45,};

	struct aes_key enc_key;
	struct aes_key dec_key;
	char encoded[BLOCK];
	char decoded[BLOCK];
	
	int ret = 0;

	int keylen = 16;

	printf("key (%d):\n%s\n\n", keylen, array2str(key, keylen));

	ret += aes_p8_set_encrypt_key(key, keylen * 8, &enc_key);
	printf("key encode:\n%s\n\n", array2str(enc_key.key, 16*15));
	
	ret += aes_p8_set_decrypt_key(key, keylen * 8, &dec_key);
	printf("key decode:\n%s\n\n", array2str(dec_key.key, 16*15));

	printf("IV:\n%s\n\n", array2str(iv, BLOCK));

	printf("string:\n%s\n\n", array2str(txt, BLOCK));

	aes_p8_encrypt(txt, encoded, &enc_key);
	printf("aes_p8_encrypt encoded:\n%s\n\n", array2str(encoded, BLOCK));
	aes_p8_decrypt(encoded, decoded, &dec_key);
	printf("aes_p8_decrypt decoded (should be == string):\n%s\n\n", array2str(decoded, BLOCK));

	aes_p8_cbc_encrypt(txt, encoded, keylen, &enc_key, iv, AES_ENCRYPT);
	printf("aes_p8_cbc_encrypt encoded:\n%s\n\n", array2str(encoded, BLOCK));
	aes_p8_cbc_encrypt(encoded, decoded, keylen, &dec_key, iv, AES_DECRYPT);
	printf("aes_p8_cbc_decrypt decoded (should be == string):\n%s\n\n", array2str(decoded, BLOCK));

	return ret;
}
