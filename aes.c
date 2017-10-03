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

	unsigned char key[2][32] = {{
		0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
		0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c},{
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
		0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
		0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f}};
		;
	unsigned char txt[2][16] = {{
		0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d,
		0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34},{
		0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
		0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff}};

	unsigned char iv[16] = {
		0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x45,};

	struct aes_key enc_key;
	struct aes_key dec_key;
	char encoded[BLOCK];
	char decoded[BLOCK];
	
	int ret = 0;

	for (int i = 0; i < 2; i++) {
		int keylen = (i+1)*16;

		printf("key (%d):\n%s\n\n", keylen, array2str(key[i], keylen));

		ret += aes_p8_set_encrypt_key(key[i], keylen * 8, &enc_key);
		printf("key encode:\n%s\n\n", array2str(enc_key.key, 16*15));

		ret += aes_p8_set_decrypt_key(key[i], keylen * 8, &dec_key);
		printf("key decode:\n%s\n\n", array2str(dec_key.key, 16*15));

		printf("string:\n%s\n\n", array2str(txt[i], BLOCK)); 

		aes_p8_encrypt(txt[i], encoded, &enc_key);
		printf("aes_p8_encrypt encoded:\n%s\n\n", array2str(encoded, BLOCK));
		aes_p8_decrypt(encoded, decoded, &dec_key);
		printf("aes_p8_decrypt decoded (should be == string):\n%s\n\n", array2str(decoded, BLOCK));
	
		aes_p8_cbc_encrypt(txt[i], encoded, keylen, &enc_key, iv, AES_ENCRYPT);
		printf("aes_p8_cbc_encrypt encoded:\n%s\n\n", array2str(encoded, BLOCK));
		aes_p8_cbc_encrypt(encoded, decoded, keylen, &dec_key, iv, AES_DECRYPT);
		printf("aes_p8_cbc_decrypt decoded (should be == string):\n%s\n\n", array2str(decoded, BLOCK));
	}

	return ret;
}
