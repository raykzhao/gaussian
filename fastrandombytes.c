/* Adapted from Intel® Advanced Encryption Standard (Intel® AES) Instructions Set - Rev 3.01
 * https://software.intel.com/sites/default/files/article/165683/aes-wp-2012-09-22-v01.pdf
 */

#include "fastrandombytes.h"
#include <string.h>
#include <x86intrin.h>

static __m128i round_key[11];
static __m128i iv;
static const __m128i ONE = {1, 0};

static inline __m128i AES_128_ASSIST (__m128i temp1, __m128i temp2)
{
	__m128i temp3;
	temp2 = _mm_shuffle_epi32 (temp2 ,0xff);
	temp3 = _mm_slli_si128 (temp1, 0x4);
	temp1 = _mm_xor_si128 (temp1, temp3);
	temp3 = _mm_slli_si128 (temp3, 0x4);
	temp1 = _mm_xor_si128 (temp1, temp3);
	temp3 = _mm_slli_si128 (temp3, 0x4);
	temp1 = _mm_xor_si128 (temp1, temp3);
	temp1 = _mm_xor_si128 (temp1, temp2);
	return temp1;
}

/* round_key <-- aes128_key_expansion(randomness), iv <-- 0 */
void fastrandombytes_setseed(const unsigned char *randomness)
{
	__m128i temp1, temp2;

	temp1 = _mm_loadu_si128((__m128i*)randomness);
	round_key[0] = temp1;
	temp2 = _mm_aeskeygenassist_si128 (temp1 ,0x1);
	temp1 = AES_128_ASSIST(temp1, temp2);
	round_key[1] = temp1;
	temp2 = _mm_aeskeygenassist_si128 (temp1,0x2);
	temp1 = AES_128_ASSIST(temp1, temp2);
	round_key[2] = temp1;
	temp2 = _mm_aeskeygenassist_si128 (temp1,0x4);
	temp1 = AES_128_ASSIST(temp1, temp2);
	round_key[3] = temp1;
	temp2 = _mm_aeskeygenassist_si128 (temp1,0x8);
	temp1 = AES_128_ASSIST(temp1, temp2);
	round_key[4] = temp1;
	temp2 = _mm_aeskeygenassist_si128 (temp1,0x10);
	temp1 = AES_128_ASSIST(temp1, temp2);
	round_key[5] = temp1;
	temp2 = _mm_aeskeygenassist_si128 (temp1,0x20);
	temp1 = AES_128_ASSIST(temp1, temp2);
	round_key[6] = temp1;
	temp2 = _mm_aeskeygenassist_si128 (temp1,0x40);
	temp1 = AES_128_ASSIST(temp1, temp2);
	round_key[7] = temp1;
	temp2 = _mm_aeskeygenassist_si128 (temp1,0x80);
	temp1 = AES_128_ASSIST(temp1, temp2);
	round_key[8] = temp1;
	temp2 = _mm_aeskeygenassist_si128 (temp1,0x1b);
	temp1 = AES_128_ASSIST(temp1, temp2);
	round_key[9] = temp1;
	temp2 = _mm_aeskeygenassist_si128 (temp1,0x36);
	temp1 = AES_128_ASSIST(temp1, temp2);
	round_key[10] = temp1;

	iv = _mm_setzero_si128();
}

static inline void AES_ctr_round(unsigned char *out)
{
	__m128i tmp;

	tmp = _mm_xor_si128(iv,round_key[0]);
	tmp = _mm_aesenc_si128(tmp,round_key[1]);
	tmp = _mm_aesenc_si128(tmp,round_key[2]);
	tmp = _mm_aesenc_si128(tmp,round_key[3]);
	tmp = _mm_aesenc_si128(tmp,round_key[4]);
	tmp = _mm_aesenc_si128(tmp,round_key[5]);
	tmp = _mm_aesenc_si128(tmp,round_key[6]);
	tmp = _mm_aesenc_si128(tmp,round_key[7]);
	tmp = _mm_aesenc_si128(tmp,round_key[8]);
	tmp = _mm_aesenc_si128(tmp,round_key[9]);
	tmp = _mm_aesenclast_si128(tmp,round_key[10]);
	_mm_storeu_si128((__m128i*)out,tmp);

	iv = _mm_add_epi32(iv, ONE);
}

/* r <-- aes128_ctr(round_key, iv, rlen) */
void fastrandombytes(unsigned char *r, unsigned long long rlen)
{
	unsigned char ct[16];
	unsigned long long num_of_blocks = rlen >> 4;
	unsigned long long i;
	
	for (i = 0; i < num_of_blocks; i++)
	{
		AES_ctr_round(r + (i << 4));
	}
	
	if (rlen & 0x0f)
	{
		AES_ctr_round(ct);
		
		memcpy(r + (i << 4), ct, rlen & 0x0f);
	}	
}