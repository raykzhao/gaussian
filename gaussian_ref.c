/* ****************************** *
 * Implemented by Raymond K. ZHAO *
 *                                *
 * Discrete Gaussian Sampler      *
 * ****************************** */

#include <stdint.h>
#include "fastrandombytes.h"
#include <x86intrin.h>
#include "cpucycles.h"
#include <stdio.h>
#include "randombytes.h"

#define CDT_ENTRY_SIZE 16
#define CDT_LOW_MASK 0x7fffffffffffffff
#define CDT_LENGTH 9 /* [0..tau*sigma]=[0..9] */

#define BERNOULLI_ENTRY_SIZE 9 /* 72bit randomness */

/* the closest integer k such that k*sigma_0=sigma */
#define BINARY_SAMPLER_K 254

/* -1/k^2 */
#define BINARY_SAMPLER_K_2_INV (-1.0/(BINARY_SAMPLER_K * BINARY_SAMPLER_K))

#define EXP_MANTISSA_PRECISION 52
#define EXP_MANTISSA_MASK ((1LL << EXP_MANTISSA_PRECISION) - 1)
#define R_MANTISSA_PRECISION (EXP_MANTISSA_PRECISION + 1)
#define R_MANTISSA_MASK ((1LL << R_MANTISSA_PRECISION) - 1)
#define R_EXPONENT_L (8 * BERNOULLI_ENTRY_SIZE - R_MANTISSA_PRECISION)

#define DOUBLE_ZERO (1023LL << 52)

#define UNIFORM_SIZE 1
#define UNIFORM_REJ 20

/* CDT table */
static const uint64_t CDT[][2] = {{2200310400551559144, 3327841033070651387},
{7912151619254726620, 380075531178589176},
{5167367257772081627, 11604843442081400},
{5081592746475748971, 90134450315532},
{6522074513864805092, 175786317361},
{2579734681240182346, 85801740},
{8175784047440310133, 10472},
{2947787991558061753, 0},
{22489665999543, 0}};

/* coefficients of the exp evaluation polynomial */
static const uint64_t EXP_COFF[] = {0x3e833b70ffa2c5d4,
									0x3eb4a480fda7e6e1,
									0x3ef01b254493363f,
									0x3f242e0e0aa273cc,
									0x3f55d8a2334ed31b,
									0x3f83b2aa56db0f1a,
									0x3fac6b08e11fc57e,
									0x3fcebfbdff556072,
									0x3fe62e42fefa7fe6,
									0x3ff0000000000000};
								   
/* convert between double and int64 */
static const __m128d V_K_2_INV = {BINARY_SAMPLER_K_2_INV, 0};

#define BENCHMARK_ITERATION 1000

/* constant time CDT sampler */
static inline uint64_t cdt_sampler(unsigned char *r)
{
	uint64_t x = 0;
	uint64_t r1, r2;
	uint32_t i;

	r1 = (*((uint64_t *)r)) & CDT_LOW_MASK;
	r2 = (*((uint64_t *)(r + 8))) & CDT_LOW_MASK;

	for (i = 0; i < CDT_LENGTH; i++)
	{
		x += (((r1 - CDT[i][0]) & ((1LL << 63) ^ ((r2 - CDT[i][1]) | (CDT[i][1] - r2)))) | (r2 - CDT[i][1])) >> 63;
	}

	return x;
}

/* constant time Bernoulli sampler
 * we directly compute exp(-x/(2*sigma_0^2)), 
 * since sigma_0=sqrt(1/2ln2), exp(-x/(2*sigma_0^2))=2^(-x/k^2), 
 * we use a polynomial to directly evaluate 2^(-x/k^2) */
static inline uint64_t bernoulli_sampler(uint64_t x, unsigned char *r)
{	
	__m128d vx, vx_1, vx_2, vsum;
	__m128i vt, vres;

	uint64_t res, res_mantissa, res_exponent;
	uint64_t r_mantissa, r_exponent;

	/* 2^x=2^(floor(x)+a)=2^(floor(x))*2^a, where a is in [0,1]
	 * we only evaluate 2^a by using a polynomial */	
	vx = _mm_cvtsi64_sd(_mm_setzero_pd(), x);
	vx = _mm_mul_sd(vx, V_K_2_INV);
	
	vx_1 = _mm_floor_pd(vx);
	vt = _mm_cvtpd_epi32(vx_1);
	vt = _mm_slli_epi64(vt, 52);
	
	/* evaluate 2^a */
	vx_2 = _mm_sub_sd(vx, vx_1);
	vsum = _mm_add_sd(_mm_mul_sd(_mm_castsi128_pd(_mm_cvtsi64x_si128(EXP_COFF[0])), vx_2), _mm_castsi128_pd(_mm_cvtsi64x_si128(EXP_COFF[1])));
	vsum = _mm_add_sd(_mm_mul_sd(vsum, vx_2), _mm_castsi128_pd(_mm_cvtsi64x_si128(EXP_COFF[2])));
	vsum = _mm_add_sd(_mm_mul_sd(vsum, vx_2), _mm_castsi128_pd(_mm_cvtsi64x_si128(EXP_COFF[3])));
	vsum = _mm_add_sd(_mm_mul_sd(vsum, vx_2), _mm_castsi128_pd(_mm_cvtsi64x_si128(EXP_COFF[4])));
	vsum = _mm_add_sd(_mm_mul_sd(vsum, vx_2), _mm_castsi128_pd(_mm_cvtsi64x_si128(EXP_COFF[5])));
	vsum = _mm_add_sd(_mm_mul_sd(vsum, vx_2), _mm_castsi128_pd(_mm_cvtsi64x_si128(EXP_COFF[6])));
	vsum = _mm_add_sd(_mm_mul_sd(vsum, vx_2), _mm_castsi128_pd(_mm_cvtsi64x_si128(EXP_COFF[7])));
	vsum = _mm_add_sd(_mm_mul_sd(vsum, vx_2), _mm_castsi128_pd(_mm_cvtsi64x_si128(EXP_COFF[8])));
	vsum = _mm_add_sd(_mm_mul_sd(vsum, vx_2), _mm_castsi128_pd(_mm_cvtsi64x_si128(EXP_COFF[9])));
	
	/* combine to compute 2^x */
	vres = _mm_add_epi64(vt, _mm_castpd_si128(vsum));
	_mm_storel_epi64((__m128i *)(&res), vres);
	
	res_mantissa = (res & EXP_MANTISSA_MASK) | (1LL << EXP_MANTISSA_PRECISION);
	res_exponent = R_EXPONENT_L - 1023 + 1 + (res >> EXP_MANTISSA_PRECISION); 
	
	r_mantissa = (*((uint64_t *)r)) & R_MANTISSA_MASK;
	r_exponent = ((*((uint64_t *)r)) >> R_MANTISSA_PRECISION) |  (((uint64_t)(r[8])) << (64 - R_MANTISSA_PRECISION));
	
	/* (res == 1.0) || ((r_mantissa < res_mantissa) && (r_exponent < (1 << res_exponent))) */
	return ((1LL << 63) ^ ((res - DOUBLE_ZERO) | (DOUBLE_ZERO - res))) | ((r_mantissa - res_mantissa) & (r_exponent - (1LL << res_exponent)));
}

/* make sure that Pr(rerun the PRG)<=2^(-64) */
static inline void uniform_sampler(unsigned char *r, uint64_t *y)
{
	uint32_t i = 0, j = 0;
	uint64_t x;
	
	while (j < 8)
	{
		do
		{	/* we ignore the low probability of rerunning the PRG */
			x = r[i++];
		} while (1 ^ ((x - BINARY_SAMPLER_K) >> 63));
		
		y[j++] = x;
	}	
}

/* binary sampling algorithm 
 * we compute 8 samples every time by using the AVX2, 
 * then do the rejection */
void gaussian_sampler(int64_t *sample, uint32_t slen)
{
	uint64_t x, y[8], z, b_in, b;
	
	unsigned char r[8 * (CDT_ENTRY_SIZE + BERNOULLI_ENTRY_SIZE) + UNIFORM_REJ * UNIFORM_SIZE + 1];
	
	uint32_t i = 8, j = 0;
	uint64_t k;
	
	while (j < slen)
	{
		do
		{
			if (i == 8)
			{
				/* x<--D_sigma_0, y<--U([0,k-1]), z=kx+y */
				fastrandombytes(r, 8 * (CDT_ENTRY_SIZE + BERNOULLI_ENTRY_SIZE) + UNIFORM_REJ * UNIFORM_SIZE + 1);
				
				uniform_sampler(r + 8 * (CDT_ENTRY_SIZE + BERNOULLI_ENTRY_SIZE), y);
				i = 0;
			}

			x = cdt_sampler(r + i * CDT_ENTRY_SIZE) * BINARY_SAMPLER_K;
			z = x + y[i];
			b = bernoulli_sampler((z + x) * y[i], r + 8 * CDT_ENTRY_SIZE + i * BERNOULLI_ENTRY_SIZE);
			
			k = (r[8 * (CDT_ENTRY_SIZE + BERNOULLI_ENTRY_SIZE) + UNIFORM_REJ * UNIFORM_SIZE] >> i) & 0x1;
			i++;			
		} while (1 ^ ((b & ((z | -z) | (k | -k))) >> 63)); /* rejection condition: b=0 or ((b=1) && (z=0) && (k=0)) */
		
		sample[j++] = z * (1 ^ ((-k) & 0xfffffffffffffffe)); /* sample=z*(-1)^k */
	}
}

int main()
{
	int64_t sample[1024];
	unsigned char seed[32];
	long long cycle1[BENCHMARK_ITERATION], cycle2[BENCHMARK_ITERATION];
	uint32_t t, i;
	
	for (t = 0; t < BENCHMARK_ITERATION; t++)
	{
		printf("Iteration: %u\n", t);
		randombytes(seed, 32);	
		fastrandombytes_setseed(seed);
		cycle1[t] = cpucycles();
		gaussian_sampler(sample, 1024);
		cycle2[t] = cpucycles();
	
		for (i = 0; i < 1024; i++)
		{
			printf("%lld ", sample[i]);
		}	
		printf("\n");
	}
	
	printf("Cycles:\n");
	for (t = 0; t < BENCHMARK_ITERATION; t++)
	{
		printf("%lld\n", cycle2[t] - cycle1[t]);
	}
	
	return 0;
}
