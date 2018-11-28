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

#define BERNOULLI_ENTRY_SIZE 8 /* 64bit Bernoulli table entry */
#define BERNOULLI_ENTRY_NUM 21
#define BERNOULLI_MASK 0x7fffffffffffffff

/* the closest integer k such that k*sigma_0=sigma */
#define BINARY_SAMPLER_K 254

#define UNIFORM_SIZE 1
#define UNIFORM_REJ 20

#define BERNOULLI_TABLE_SIZE (BERNOULLI_ENTRY_NUM * BERNOULLI_ENTRY_SIZE)

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

/* Bernoulli table p_i=exp(-2^i/2sigma^2) */
static const uint64_t BER_TABLE[] = {9223272943296909082,
9223173850803678280,
9222975669011078694,
9222579318201007340,
9221786667678813911,
9220201571005729925,
9217032194980845944,
9210696710905216509,
9198038804169290314,
9172775152617009960,
9122455828981195376,
9022643781383367686,
8826283975149786757,
8446291497155242355,
7734680957230936012,
6486270885647983111,
4561423938435359062,
2255854828688696322,
551737584452187600,
33004671272150260,
118103045332015};

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
		x += (((r1 - CDT[i][0]) >> 63) & (1 ^ (((r2 - CDT[i][1]) | (CDT[i][1] - r2)) >> 63))) | ((r2 - CDT[i][1]) >> 63);
	}

	return x;
}

/* constant time table-based Bernoulli sampler */
static inline uint64_t bernoulli_sampler(uint64_t x, unsigned char *r)
{	
	uint32_t i;
	uint64_t res = 1;
	uint64_t y;
	
	for (i = 0; i < BERNOULLI_ENTRY_NUM; i++)
	{
		y = (*((uint64_t *)(r + i * BERNOULLI_ENTRY_SIZE))) & BERNOULLI_MASK;
		res &= ((y - BER_TABLE[i]) >> 63) | (1 ^ (x & 0x1));
		x >>= 1;
	}
	
	return res;
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
	
	unsigned char r[8 * (CDT_ENTRY_SIZE + BERNOULLI_TABLE_SIZE) + UNIFORM_REJ * UNIFORM_SIZE + 1];
	
	uint32_t i = 8, j = 0;
	uint64_t k;
	
	while (j < slen)
	{
		do
		{
			if (i == 8)
			{
				/* x<--D_sigma_0, y<--U([0,k-1]), z=kx+y */
				fastrandombytes(r, 8 * (CDT_ENTRY_SIZE + BERNOULLI_TABLE_SIZE) + UNIFORM_REJ * UNIFORM_SIZE + 1);
				
				uniform_sampler(r + 8 * (CDT_ENTRY_SIZE + BERNOULLI_TABLE_SIZE), y);
				i = 0;
			}

			x = cdt_sampler(r + i * CDT_ENTRY_SIZE) * BINARY_SAMPLER_K;
			z = x + y[i];
			b = bernoulli_sampler((z + x) * y[i], r + 8 * CDT_ENTRY_SIZE + i * BERNOULLI_TABLE_SIZE);
			
			k = (r[8 * (CDT_ENTRY_SIZE + BERNOULLI_TABLE_SIZE) + UNIFORM_REJ * UNIFORM_SIZE] >> i) & 0x1;
			i++;			
		} while (1 ^ (b & (((z | -z) | (k | -k)) >> 63))); /* rejection condition: b=0 or ((b=1) && (z=0) && (k=0)) */
		
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
