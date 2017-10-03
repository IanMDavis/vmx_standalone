
#define AES_MAX_KEYLENGTH	(15 * 16)
# define AES_ENCRYPT     1
# define AES_DECRYPT     0

struct aes_key {
	uint8_t key[AES_MAX_KEYLENGTH];
	int rounds;
};

int aes_p8_set_encrypt_key(const uint8_t *userKey, const int bits,
			   struct aes_key *key);
int aes_p8_set_decrypt_key(const uint8_t *userKey, const int bits,
			   struct aes_key *key);

void aes_p8_encrypt(const uint8_t *in, uint8_t *out, const struct aes_key *key);
void aes_p8_decrypt(const uint8_t *in, uint8_t *out, const struct aes_key *key);
void aes_p8_cbc_encrypt(const uint8_t *in, uint8_t *out, uint16_t len,
			const struct aes_key *key, uint8_t *iv, const int enc);
