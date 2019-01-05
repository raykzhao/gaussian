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

#define BERNOULLI_ENTRY_SIZE 6 /* 48bit exp expansion */

/* the closest integer k such that k*sigma_0=sigma */
#define BINARY_SAMPLER_K 254

/* -1/k^2 */
#define BINARY_SAMPLER_K_2_INV (-1.0/(BINARY_SAMPLER_K * BINARY_SAMPLER_K))

#define EXP_PRECISION 48
#define EXP_DOUBLE (1023 + EXP_PRECISION)

#define UNIFORM_SIZE 1
#define UNIFORM_REJ 20

#define BASE_TABLE_SIZE (4 * CDT_ENTRY_SIZE)
#define BERNOULLI_TABLE_SIZE (4 * BERNOULLI_ENTRY_SIZE)

/* CDT table */
static const __m256i V_CDT[][2] = {{{2200310400551559144, 2200310400551559144, 2200310400551559144, 2200310400551559144}, {3327841033070651387, 3327841033070651387, 3327841033070651387, 3327841033070651387}},
{{7912151619254726620, 7912151619254726620, 7912151619254726620, 7912151619254726620}, {380075531178589176, 380075531178589176, 380075531178589176, 380075531178589176}},
{{5167367257772081627, 5167367257772081627, 5167367257772081627, 5167367257772081627}, {11604843442081400, 11604843442081400, 11604843442081400, 11604843442081400}},
{{5081592746475748971, 5081592746475748971, 5081592746475748971, 5081592746475748971}, {90134450315532, 90134450315532, 90134450315532, 90134450315532}},
{{6522074513864805092, 6522074513864805092, 6522074513864805092, 6522074513864805092}, {175786317361, 175786317361, 175786317361, 175786317361}},
{{2579734681240182346, 2579734681240182346, 2579734681240182346, 2579734681240182346}, {85801740, 85801740, 85801740, 85801740}},
{{8175784047440310133, 8175784047440310133, 8175784047440310133, 8175784047440310133}, {10472, 10472, 10472, 10472}},
{{2947787991558061753, 2947787991558061753, 2947787991558061753, 2947787991558061753}, {0, 0, 0, 0}},
{{22489665999543, 22489665999543, 22489665999543, 22489665999543}, {0, 0, 0, 0}}};

static const __m256i V_CDT_LOW_MASK = {CDT_LOW_MASK, CDT_LOW_MASK, CDT_LOW_MASK, CDT_LOW_MASK};

static const __m256i V_K_K_K_K = {BINARY_SAMPLER_K, BINARY_SAMPLER_K, BINARY_SAMPLER_K, BINARY_SAMPLER_K};

/* coefficients of the exp evaluation polynomial */
static const __m256i EXP_COFF[] = {{0x3e833b70ffa2c5d4, 0x3e833b70ffa2c5d4, 0x3e833b70ffa2c5d4, 0x3e833b70ffa2c5d4},
								   {0x3eb4a480fda7e6e1, 0x3eb4a480fda7e6e1, 0x3eb4a480fda7e6e1, 0x3eb4a480fda7e6e1},
								   {0x3ef01b254493363f, 0x3ef01b254493363f, 0x3ef01b254493363f, 0x3ef01b254493363f},
								   {0x3f242e0e0aa273cc, 0x3f242e0e0aa273cc, 0x3f242e0e0aa273cc, 0x3f242e0e0aa273cc},
								   {0x3f55d8a2334ed31b, 0x3f55d8a2334ed31b, 0x3f55d8a2334ed31b, 0x3f55d8a2334ed31b},
								   {0x3f83b2aa56db0f1a, 0x3f83b2aa56db0f1a, 0x3f83b2aa56db0f1a, 0x3f83b2aa56db0f1a},
								   {0x3fac6b08e11fc57e, 0x3fac6b08e11fc57e, 0x3fac6b08e11fc57e, 0x3fac6b08e11fc57e},
								   {0x3fcebfbdff556072, 0x3fcebfbdff556072, 0x3fcebfbdff556072, 0x3fcebfbdff556072},
								   {0x3fe62e42fefa7fe6, 0x3fe62e42fefa7fe6, 0x3fe62e42fefa7fe6, 0x3fe62e42fefa7fe6},
								   {0x3ff0000000000000, 0x3ff0000000000000, 0x3ff0000000000000, 0x3ff0000000000000}};
								   
/* convert between double and int64 */
static const __m256i V_EXP_DOUBLE = {EXP_DOUBLE, EXP_DOUBLE, EXP_DOUBLE, EXP_DOUBLE};
static const __m256d V_INT64_DOUBLE = {0x0010000000000000, 0x0010000000000000, 0x0010000000000000, 0x0010000000000000};
static const __m256d V_DOUBLE_INT64 = {0x0018000000000000, 0x0018000000000000, 0x0018000000000000, 0x0018000000000000};

static const __m256d V_K_2_INV = {BINARY_SAMPLER_K_2_INV, BINARY_SAMPLER_K_2_INV, BINARY_SAMPLER_K_2_INV, BINARY_SAMPLER_K_2_INV};

#define BENCHMARK_ITERATION 1000

static inline uint64_t load_48(const unsigned char *x)
{
	return ((uint64_t)(*((uint16_t *)x))) | (((uint64_t)(*((uint32_t *)(x + 2)))) << 16);
}

/* constant time CDT sampler */
static inline __m256i cdt_sampler(unsigned char *r)
{
	__m256i x = _mm256_setzero_si256();
	__m256i r1, r2;
	__m256i r1_lt_cdt0, r2_lt_cdt1;
	__m256i r2_eq_cdt1;
	__m256i b;
	
	uint32_t i;
	
	r1 = _mm256_load_si256((__m256i *)r);
	r2 = _mm256_load_si256((__m256i *)(r + 32));
	
	r1 = _mm256_and_si256(r1, V_CDT_LOW_MASK);
	r2 = _mm256_and_si256(r2, V_CDT_LOW_MASK);

	for (i = 0; i < CDT_LENGTH; i++)
	{
		r1_lt_cdt0 = _mm256_sub_epi64(r1, V_CDT[i][0]);
		r1_lt_cdt0 = _mm256_srli_epi64(r1_lt_cdt0, 63);

		r2_lt_cdt1 = _mm256_sub_epi64(r2, V_CDT[i][1]);
		r2_lt_cdt1 = _mm256_srli_epi64(r2_lt_cdt1, 63);
		r2_eq_cdt1 = _mm256_cmpeq_epi64(r2, V_CDT[i][1]);
		r2_eq_cdt1 = _mm256_srli_epi64(r2_eq_cdt1, 63);

		b = _mm256_and_si256(r1_lt_cdt0, r2_eq_cdt1);
		b = _mm256_or_si256(b, r2_lt_cdt1);
		
		x = _mm256_add_epi64(x, b);
	}

	return x;
}

/* constant time Bernoulli sampler
 * we directly compute exp(-x/(2*sigma_0^2)), 
 * since sigma_0=sqrt(1/2ln2), exp(-x/(2*sigma_0^2))=2^(-x/k^2), 
 * we use a polynomial to directly evaluate 2^(-x/k^2) */
static inline void bernoulli_sampler(uint64_t *b, __m256i x, unsigned char *r)
{	
	__m256d vx, vx_1, vx_2, vsum, vres;
	__m256i vt, v_exp_x, k;

	/* 2^x=2^(floor(x)+a)=2^(floor(x))*2^a, where a is in [0,1]
	 * we only evaluate 2^a by using a polynomial */
	x = _mm256_or_si256(x, _mm256_castpd_si256(V_INT64_DOUBLE));
	vx = _mm256_sub_pd(_mm256_castsi256_pd(x), V_INT64_DOUBLE);
	vx = _mm256_mul_pd(vx, V_K_2_INV);
	
	vx_1 = _mm256_floor_pd(vx);
	vx_2 = _mm256_add_pd(vx_1, V_DOUBLE_INT64);
	vt = _mm256_sub_epi64(_mm256_castpd_si256(vx_2), _mm256_castpd_si256(V_DOUBLE_INT64));	

	/* evaluate 2^a */
	vx_2 = _mm256_sub_pd(vx, vx_1);
	vsum = _mm256_fmadd_pd(_mm256_castsi256_pd(EXP_COFF[0]), vx_2, _mm256_castsi256_pd(EXP_COFF[1]));
	vsum = _mm256_fmadd_pd(vsum, vx_2, _mm256_castsi256_pd(EXP_COFF[2]));
	vsum = _mm256_fmadd_pd(vsum, vx_2, _mm256_castsi256_pd(EXP_COFF[3]));
	vsum = _mm256_fmadd_pd(vsum, vx_2, _mm256_castsi256_pd(EXP_COFF[4]));
	vsum = _mm256_fmadd_pd(vsum, vx_2, _mm256_castsi256_pd(EXP_COFF[5]));
	vsum = _mm256_fmadd_pd(vsum, vx_2, _mm256_castsi256_pd(EXP_COFF[6]));
	vsum = _mm256_fmadd_pd(vsum, vx_2, _mm256_castsi256_pd(EXP_COFF[7]));
	vsum = _mm256_fmadd_pd(vsum, vx_2, _mm256_castsi256_pd(EXP_COFF[8]));
	vsum = _mm256_fmadd_pd(vsum, vx_2, _mm256_castsi256_pd(EXP_COFF[9]));
	
	/* combine to compute 2^x */
	vt = _mm256_add_epi64(vt, V_EXP_DOUBLE);
	vt = _mm256_slli_epi64(vt, 52);
	vres = _mm256_mul_pd(_mm256_castsi256_pd(vt), vsum);
	vres = _mm256_round_pd(vres, _MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC);

	vres = _mm256_add_pd(vres, V_DOUBLE_INT64);
	v_exp_x = _mm256_sub_epi64(_mm256_castpd_si256(vres), _mm256_castpd_si256(V_DOUBLE_INT64));		

	/* compute the Bernoulli value */
	k = _mm256_set_epi64x(load_48(r + 3 * BERNOULLI_ENTRY_SIZE), load_48(r + 2 * BERNOULLI_ENTRY_SIZE), load_48(r + BERNOULLI_ENTRY_SIZE), load_48(r));
	k = _mm256_sub_epi64(k, v_exp_x);
	k = _mm256_srli_epi64(k, 63);
	
	_mm256_store_si256((__m256i *)(b), k);
}

/* make sure that Pr(rerun the PRG)<=2^(-64) */
static inline void uniform_sampler(unsigned char *r, __m256i *y1, __m256i *y2)
{
	uint64_t sample[8] __attribute__ ((aligned (32)));
	uint32_t i = 0, j = 0;
	uint64_t x;
	
	while (j < 8)
	{
		do
		{	/* we ignore the low probability of rerunning the PRG */
			x = r[i++];
		} while (1 ^ ((x - BINARY_SAMPLER_K) >> 63));
		
		sample[j++] = x;
	}
	
	*y1 = _mm256_load_si256((__m256i *)(sample));
	*y2 = _mm256_load_si256((__m256i *)(sample + 4));
}

/* binary sampling algorithm 
 * we compute 8 samples every time by using the AVX2, 
 * then do the rejection */
void gaussian_sampler(int64_t *sample, uint32_t slen)
{
	__m256i v_x, v_y1, v_y2, v_z, v_b_in;
	uint64_t z[8] __attribute__ ((aligned (32)));
	uint64_t b[8] __attribute__ ((aligned (32)));
	
	unsigned char r[2 * (BASE_TABLE_SIZE + BERNOULLI_TABLE_SIZE) + UNIFORM_REJ * UNIFORM_SIZE + 1];
	unsigned char *r1;
	
	uint32_t i = 8, j = 0;
	uint64_t k;
	
	while (j < slen)
	{
		do
		{
			if (i == 8)
			{
				/* x<--D_sigma_0, y<--U([0,k-1]), z=kx+y */
				fastrandombytes(r, 2 * (BASE_TABLE_SIZE + BERNOULLI_TABLE_SIZE) + UNIFORM_REJ * UNIFORM_SIZE + 1);
				
				uniform_sampler(r + 2 * (BASE_TABLE_SIZE + BERNOULLI_TABLE_SIZE), &v_y1, &v_y2);
				
				r1 = r;
				v_x = cdt_sampler(r1);
				v_x = _mm256_mul_epu32(v_x, V_K_K_K_K);
				v_z = _mm256_add_epi64(v_x, v_y1);
				_mm256_store_si256((__m256i *)(z), v_z);
				/* b<--Bernoulli(exp(-y(y+2kx)/2sigma_0^2)) */
				v_b_in = _mm256_add_epi64(v_z, v_x);
				v_b_in = _mm256_mul_epu32(v_b_in, v_y1);
				bernoulli_sampler(b, v_b_in, r1 + BASE_TABLE_SIZE);
				
				r1 = r + (BASE_TABLE_SIZE + BERNOULLI_TABLE_SIZE);
				v_x = cdt_sampler(r1);
				v_x = _mm256_mul_epu32(v_x, V_K_K_K_K);
				v_z = _mm256_add_epi64(v_x, v_y2);
				_mm256_store_si256((__m256i *)(z + 4), v_z);
				/* b<--Bernoulli(exp(-y(y+2kx)/2sigma_0^2)) */
				v_b_in = _mm256_add_epi64(v_z, v_x);
				v_b_in = _mm256_mul_epu32(v_b_in, v_y2);
				bernoulli_sampler(b + 4, v_b_in, r1 + BASE_TABLE_SIZE);

				i = 0;
			}
			
			k = (r[2 * (BASE_TABLE_SIZE + BERNOULLI_TABLE_SIZE) + UNIFORM_REJ * UNIFORM_SIZE] >> i) & 0x1;
			i++;			
		} while (1 ^ (b[i - 1] & (((z[i - 1] | -z[i - 1]) | (k | -k)) >> 63))); /* rejection condition: b=0 or ((b=1) && (z=0) && (k=0)) */
		
		sample[j++] = z[i - 1] * (1 ^ ((-k) & 0xfffffffffffffffe)); /* sample=z*(-1)^k */
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
