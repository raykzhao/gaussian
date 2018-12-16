/* Adapted from Intel® Advanced Encryption Standard (Intel® AES) Instructions Set - Rev 3.01
 * https://software.intel.com/sites/default/files/article/165683/aes-wp-2012-09-22-v01.pdf
 */

#include "fastrandombytes.h"
#include <string.h>
#include <x86intrin.h>

static __m128i round_key[13];
static __m128i iv;
static const __m128i ONE = {1, 0};

static inline void KEY_192_ASSIST(__m128i* temp1, __m128i * temp2, __m128i * temp3)
{
	__m128i temp4;
	*temp2 = _mm_shuffle_epi32 (*temp2, 0x55);
	temp4 = _mm_slli_si128 (*temp1, 0x4);
	*temp1 = _mm_xor_si128 (*temp1, temp4);
	temp4 = _mm_slli_si128 (temp4, 0x4);
	*temp1 = _mm_xor_si128 (*temp1, temp4);
	temp4 = _mm_slli_si128 (temp4, 0x4);
	*temp1 = _mm_xor_si128 (*temp1, temp4);
	*temp1 = _mm_xor_si128 (*temp1, *temp2);
	*temp2 = _mm_shuffle_epi32(*temp1, 0xff);
	temp4 = _mm_slli_si128 (*temp3, 0x4);
	*temp3 = _mm_xor_si128 (*temp3, temp4);
	*temp3 = _mm_xor_si128 (*temp3, *temp2);
}

/* round_key <-- aes192_key_expansion(randomness), iv <-- 0 */
void fastrandombytes_setseed(const unsigned char *randomness, int nonce)
{
	__m128i temp1, temp2, temp3, temp4;

	temp1 = _mm_loadu_si128((__m128i*)randomness);
	temp3 = _mm_loadu_si128((__m128i*)(randomness+16));
	round_key[0]=temp1;
	round_key[1]=temp3;
	temp2=_mm_aeskeygenassist_si128 (temp3,0x1);
	KEY_192_ASSIST(&temp1, &temp2, &temp3);
	round_key[1] = (__m128i)_mm_shuffle_pd((__m128d)round_key[1], (__m128d)temp1,0);
	round_key[2] = (__m128i)_mm_shuffle_pd((__m128d)temp1,(__m128d)temp3,1);
	temp2=_mm_aeskeygenassist_si128 (temp3,0x2);
	KEY_192_ASSIST(&temp1, &temp2, &temp3);
	round_key[3]=temp1;
	round_key[4]=temp3;
	temp2=_mm_aeskeygenassist_si128 (temp3,0x4);
	KEY_192_ASSIST(&temp1, &temp2, &temp3);
	round_key[4] = (__m128i)_mm_shuffle_pd((__m128d)round_key[4], (__m128d)temp1,0);
	round_key[5] = (__m128i)_mm_shuffle_pd((__m128d)temp1,(__m128d)temp3,1);
	temp2=_mm_aeskeygenassist_si128 (temp3,0x8);
	KEY_192_ASSIST(&temp1, &temp2, &temp3);
	round_key[6]=temp1;
	round_key[7]=temp3;
	temp2=_mm_aeskeygenassist_si128 (temp3,0x10);
	KEY_192_ASSIST(&temp1, &temp2, &temp3);
	round_key[7] = (__m128i)_mm_shuffle_pd((__m128d)round_key[7], (__m128d)temp1,0);
	round_key[8] = (__m128i)_mm_shuffle_pd((__m128d)temp1,(__m128d)temp3,1);
	temp2=_mm_aeskeygenassist_si128 (temp3,0x20);
	KEY_192_ASSIST(&temp1, &temp2, &temp3);
	round_key[9]=temp1;
	round_key[10]=temp3;
	temp2=_mm_aeskeygenassist_si128 (temp3,0x40);
	KEY_192_ASSIST(&temp1, &temp2, &temp3);
	round_key[10] = (__m128i)_mm_shuffle_pd((__m128d)round_key[10], (__m128d)temp1,0);
	round_key[11] = (__m128i)_mm_shuffle_pd((__m128d)temp1,(__m128d)temp3,1);
	temp2=_mm_aeskeygenassist_si128 (temp3,0x80);
	KEY_192_ASSIST(&temp1, &temp2, &temp3);
	round_key[12]=temp1;

	iv = _mm_set_epi64x(nonce, 0);
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
	tmp = _mm_aesenc_si128(tmp,round_key[10]);
	tmp = _mm_aesenc_si128(tmp,round_key[11]);
	tmp = _mm_aesenclast_si128(tmp,round_key[12]);
	_mm_storeu_si128((__m128i*)out,tmp);

	iv = _mm_add_epi32(iv, ONE);
}

/* r <-- aes192_ctr(round_key, iv, rlen) */
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
