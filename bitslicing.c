// Adapted from KRR+18, https://github.com/Angshumank/const_gauss
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include "randombytes.h"
#include "cpucycles.h"
#include "fastrandombytes.h"

uint64_t hist[84];

//-------shake------
void initRandom(uint8_t *x, uint32_t len){

	randombytes(x,len);
}
//-------shake ends-

int main(){

	int64_t i,j,k;
	uint64_t bit[128];// to hold the bits
	uint64_t sample[64];
	volatile uint64_t out[7];
	
	volatile uint64_t clock1, clock2,clock3,clock4, clock11;
	clock1=0;clock2=0;clock3=0;clock4=0;clock11=0;
	
	//-------------------prg--------------------

	uint8_t seed[32];
	uint32_t SEEDBYTES=32;
 	unsigned char* byte_bank = (unsigned char*) ((void*) bit);

	initRandom(seed,SEEDBYTES);
	fastrandombytes_setseed(seed);
	

	//-------------------prg ends---------------

	uint64_t disable_update,control;
	
	for(i=0;i<84;i++)
		hist[i]=0;


	clock3=0;
	clock4=0;
	int repeat=1000000;
	for(i=0;i<repeat;i++){	

		for(k=0;k<7;k++)
		out[k]=out[k]^out[k];

		clock1=0;
		clock2=0;
		clock11=0;

	clock11 = cpucycles();

//-------------------prg----------------------
	fastrandombytes(byte_bank, sizeof(bit));
//-------------------prg ends-----------------


		clock1 = cpucycles();

//-----------------------------------------------------------------------------------------------------------

		disable_update=0; //0-> to update every time. 1-> don't update anymore. Once switched to 1 stays at 1
		control=0xffffffffffffffff; //used to control the disable_update properly

		out[2] = (~disable_update & ((~bit[2]&bit[1]) | (bit[2]&~bit[1]))) | (disable_update & out[2]);

		out[1] = (~disable_update & (bit[2])) | (disable_update & out[1]);

		out[0] = (~disable_update & ((~bit[2]&~bit[1]) | (~bit[3]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[0]);
		control=control & bit[0];
		//-------------1--------------

		out[3] = (~disable_update & ((~bit[4]&~bit[3]&bit[2]) | (bit[4]&bit[3]&~bit[2]) | (~bit[4]&bit[3]&~bit[2]))) | (disable_update & out[3]);

		out[2] = (~disable_update & (bit[4]&~bit[3]&bit[2])) | (disable_update & out[2]);

		out[1] = (~disable_update & ((~bit[4]&bit[3]&~bit[2]) | (bit[3]&bit[2]))) | (disable_update & out[1]);

		out[0] = (~disable_update & ((~bit[4]&bit[3]&bit[2]) | (bit[4]&bit[3]&~bit[2]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[1]);
		control=control & bit[1];

		//-------------2--------------

		out[3] = (~disable_update & ((~bit[4]&~bit[3]) | (bit[6]&bit[5]&bit[4]&bit[3]) | (~bit[6]&bit[5]&bit[4]&bit[3]) | (~bit[5]&~bit[3]))) | (disable_update & out[3]);

		out[2] = (~disable_update & ((bit[6]&bit[5]&bit[4]&bit[3]) | (~bit[6]&bit[5]&bit[4]&bit[3]) | (bit[5]&bit[4]&~bit[3]) | (~bit[5]&~bit[4]))) | (disable_update & out[2]);

		out[1] = (~disable_update & ((~bit[5]&bit[4]&bit[3]) | (~bit[6]&bit[5]&bit[4]&bit[3]) | (bit[5]&bit[4]&~bit[3]) | (bit[5]&~bit[4]))) | (disable_update & out[1]);

		out[0] = (~disable_update & ((~bit[4]&bit[3]) | (bit[6]&bit[5]&bit[4]&bit[3]) | (bit[5]&~bit[4]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[2]);
		control=control & bit[2];

		//-------------3--------------

		out[4] = (~disable_update & ((~bit[7]&~bit[6]&bit[5]&bit[4]))) | (disable_update & out[4]);

		out[3] = (~disable_update & ((bit[7]&~bit[6]&bit[5]&bit[4]) | (~bit[6]&~bit[4]) | (bit[6]&bit[5]&bit[4]) | (~bit[5]&~bit[4]))) | (disable_update & out[3]);

		out[2] = (~disable_update & ((~bit[7]&bit[6]&bit[5]) | (~bit[6]&~bit[5]&bit[4]) | (bit[6]&bit[5]&~bit[4]) | (bit[7]&~bit[6]&bit[5]&bit[4]))) | (disable_update & out[2]);

		out[1] = (~disable_update & ((~bit[6]&~bit[5]&~bit[4]) | (bit[6]&~bit[5]&bit[4]) | (bit[7]&~bit[6]&bit[5]&bit[4]))) | (disable_update & out[1]);

		out[0] = (~disable_update & ((bit[6]&bit[5]&~bit[4]) | (bit[7]&~bit[6]&bit[5]&bit[4]) | (bit[6]&bit[5]&bit[4]) | (~bit[5]&~bit[4]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[3]);
		control=control & bit[3];

		//-------------4--------------

		out[4] = (~disable_update & ((~bit[8]&~bit[7]&~bit[6]&bit[5]))) | (disable_update & out[4]);

		out[3] = (~disable_update & ((bit[7]&~bit[6]&bit[5]) | (~bit[7]&bit[6]&bit[5]) | (bit[8]&~bit[7]&~bit[6]&bit[5]))) | (disable_update & out[3]);

		out[2] = (~disable_update & ((bit[8]&bit[7]&bit[6]&bit[5]) | (~bit[8]&bit[7]&bit[6]&bit[5]) | (bit[7]&~bit[6]&bit[5]) | (bit[8]&~bit[7]&~bit[6]&bit[5]) | (~bit[6]&~bit[5]))) | (disable_update & out[2]);

		out[1] = (~disable_update & ((~bit[8]&bit[7]&bit[6]&bit[5]) | (bit[6]&~bit[5]) | (~bit[7]&bit[6]&bit[5]) | (bit[8]&~bit[7]&~bit[6]&bit[5]) | (~bit[7]&~bit[5]))) | (disable_update & out[1]);

		out[0] = (~disable_update & ((bit[8]&bit[7]&bit[6]&bit[5]) | (~bit[8]&~bit[6]) | (~bit[8]&~bit[7]) | (bit[8]&~bit[7]&~bit[6]&bit[5]) | (~bit[6]&~bit[5]) | (~bit[7]&~bit[5]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[4]);
		control=control & bit[4];

		//-------------5--------------

		out[4] = (~disable_update & ((bit[8]&bit[7]&~bit[6]))) | (disable_update & out[4]);

		out[3] = (~disable_update & ((~bit[9]&~bit[8]&bit[6]) | (~bit[7]&bit[6]))) | (disable_update & out[3]);

		out[2] = (~disable_update & ((bit[9]&~bit[8]&bit[7]&bit[6]) | (bit[8]&bit[7]&bit[6]) | (~bit[8]&~bit[7]))) | (disable_update & out[2]);

		out[1] = (~disable_update & ((bit[8]&bit[7]&~bit[6]) | (bit[9]&~bit[8]&bit[7]&bit[6]) | (~bit[9]&~bit[7]&bit[6]))) | (disable_update & out[1]);

		out[0] = (~disable_update & ((bit[9]&bit[8]&~bit[7]) | (bit[9]&~bit[8]&bit[7]&bit[6]) | (bit[8]&~bit[7]&~bit[6]) | (~bit[9]&bit[8]&bit[7]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[5]);
		control=control & bit[5];

		//-------------6--------------

		out[4] = (~disable_update & ((bit[8]&~bit[7]))) | (disable_update & out[4]);

		out[3] = (~disable_update & ((~bit[10]&~bit[9]&bit[7]) | (~bit[8]&bit[7]))) | (disable_update & out[3]);

		out[2] = (~disable_update & ((~bit[10]&~bit[9]&bit[8]&~bit[7]) | (bit[10]&~bit[9]&bit[8]&bit[7]) | (~bit[10]&bit[9]&bit[7]) | (~bit[9]&~bit[8]&bit[7]))) | (disable_update & out[2]);

		out[1] = (~disable_update & ((bit[9]&bit[8]&bit[7]) | (bit[10]&bit[9]&bit[7]) | (~bit[9]&~bit[8]&bit[7]) | (~bit[8]&~bit[7]) | (bit[10]&~bit[9]))) | (disable_update & out[1]);

		out[0] = (~disable_update & ((~bit[10]&bit[9]&bit[8]&~bit[7]) | (bit[10]&~bit[9]&bit[8]&bit[7]) | (~bit[9]&~bit[8]&~bit[7]) | (~bit[10]&~bit[9]&bit[7]) | (bit[10]&bit[9]&bit[7]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[6]);
		control=control & bit[6];

		//-------------7--------------

		out[4] = (~disable_update & ((bit[11]&~bit[10]&bit[9]&~bit[8]) | (~bit[11]&bit[9]&~bit[8]))) | (disable_update & out[4]);

		out[3] = (~disable_update & ((bit[11]&bit[10]&bit[9]&~bit[8]) | (~bit[9]&bit[8]) | (~bit[11]&~bit[10]&bit[8]))) | (disable_update & out[3]);

		out[2] = (~disable_update & ((~bit[11]&~bit[10]&bit[9]&~bit[8]) | (~bit[11]&~bit[9]&bit[8]) | (bit[11]&bit[10]&bit[9]&~bit[8]) | (~bit[11]&bit[10]&bit[8]) | (bit[11]&~bit[10]&bit[8]))) | (disable_update & out[2]);

		out[1] = (~disable_update & ((bit[11]&~bit[10]&bit[9]&~bit[8]) | (~bit[10]&~bit[9]&~bit[8]) | (bit[11]&bit[10]&bit[9]&~bit[8]) | (bit[11]&bit[10]&bit[8]) | (~bit[11]&~bit[10]&bit[8]))) | (disable_update & out[1]);

		out[0] = (~disable_update & ((~bit[11]&~bit[10]&bit[9]&~bit[8]) | (bit[11]&bit[10]&bit[8]) | (bit[11]&~bit[10]&bit[8]) | (bit[10]&~bit[8]))) | (disable_update & out[0]);


		disable_update= disable_update | (control & ~bit[7]);
		control=control & bit[7];


		//-------------8--------------

		out[4] = (~disable_update & ((~bit[12]&~bit[11]&~bit[10]&bit[9]) | (bit[12]&bit[11]&bit[10]&~bit[9]) | (~bit[12]&bit[11]&bit[10]&~bit[9]) | (~bit[12]&bit[11]&~bit[10]&bit[9]) | (bit[12]&~bit[11]&~bit[10]&bit[9]))) | (disable_update & out[4]);

		out[3] = (~disable_update & ((bit[12]&bit[11]&~bit[10]&bit[9]) | (~bit[11]&bit[10]&bit[9])))  | (disable_update & out[3]);

		out[2] = (~disable_update & ((~bit[12]&~bit[11]&~bit[10]&bit[9]) | (bit[12]&bit[11]&~bit[10]&bit[9]) | (bit[11]&bit[10])))  | (disable_update & out[2]);

		out[1] = (~disable_update & ((~bit[12]&bit[11]&bit[10]&~bit[9]) | (~bit[11]&~bit[10]&~bit[9]) | (bit[12]&bit[11]&~bit[10]&bit[9]) | (~bit[11]&bit[10]&bit[9]) | (bit[12]&~bit[11]&~bit[10]&bit[9])))  | (disable_update & out[1]);

		out[0] = (~disable_update & ((bit[12]&bit[11]&bit[10]&~bit[9]) | (~bit[12]&bit[10]&bit[9]) | (bit[11]&~bit[10]&~bit[9]) | (~bit[12]&bit[11]&~bit[10]&bit[9]) | (bit[12]&~bit[11]&~bit[10]&bit[9])))  | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[8]);
		control=control & bit[8];

		//-------------9--------------

		out[4] = (~disable_update & ((~bit[13]&~bit[12]&bit[11]&~bit[10]) | (bit[13]&~bit[12]&bit[11]&~bit[10]) | (bit[12]&bit[11]&~bit[10]))) | (disable_update & out[4]);

		out[3] = (~disable_update & ((~bit[13]&~bit[12]&bit[11]&~bit[10]) | (bit[13]&~bit[11]&bit[10]) | (~bit[13]&~bit[11]&bit[10]) | (~bit[13]&~bit[12]&bit[10]))) | (disable_update & out[3]);

		out[2] = (~disable_update & ((~bit[13]&bit[12]&bit[11]&~bit[10]) | (bit[13]&~bit[12]&bit[10]) | (~bit[13]&~bit[11]&bit[10]) | (bit[13]&~bit[12]&bit[11]&~bit[10]))) | (disable_update & out[2]);

		out[1] = (~disable_update & ((~bit[13]&bit[12]&bit[11]) | (bit[13]&~bit[11]&bit[10]) | (bit[13]&~bit[12]&bit[11]&~bit[10]) | (bit[12]&bit[11]&~bit[10]) | (~bit[12]&~bit[11]))) | (disable_update & out[1]);

		out[0] = (~disable_update & ((bit[12]&~bit[11]&~bit[10]) | (bit[12]&bit[11]&bit[10]) | (~bit[13]&~bit[12]&bit[10]) | (bit[13]&~bit[12]&bit[11]&~bit[10]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[9]);
		control=control & bit[9];

		//-------------10--------------

		out[4] = (~disable_update & ((~bit[14]&bit[13]&~bit[12]&~bit[11]) | (bit[14]&bit[13]&~bit[11]) | (~bit[13]&bit[12]&~bit[11]) | (~bit[14]&bit[12]&~bit[11]))) | (disable_update & out[4]);

		out[3] = (~disable_update & ((~bit[14]&bit[13]&~bit[12]&~bit[11]) | (~bit[12]&bit[11]))) | (disable_update & out[3]);

		out[2] = (~disable_update & ((bit[14]&bit[13]&~bit[12]&~bit[11]) | (~bit[14]&bit[12]&bit[11]) | (~bit[13]&bit[12]&~bit[11]) | (~bit[13]&bit[11]))) | (disable_update & out[2]);

		out[1] = (~disable_update & ((bit[14]&bit[13]&bit[12]&bit[11]) | (bit[14]&bit[13]&~bit[12]&~bit[11]) | (~bit[14]&~bit[13]&bit[11]) | (~bit[14]&~bit[12]&bit[11]) | (~bit[14]&bit[12]&~bit[11]))) | (disable_update & out[1]);

		out[0] = (~disable_update & ((~bit[14]&bit[13]&~bit[12]&~bit[11]) | (bit[14]&bit[12]&bit[11]) | (bit[14]&bit[13]&~bit[11]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[10]);
		control=control & bit[10];

		//-------------11--------------

		out[4] = (~disable_update & ((bit[14]&bit[13]&~bit[12]) | (~bit[13]&bit[12]))) | (disable_update & out[4]);

		out[3] = (~disable_update & ((~bit[15]&bit[14]&bit[13]&~bit[12]) | (bit[15]&~bit[14]&bit[13]&bit[12]) | (~bit[15]&~bit[14]&bit[13]&bit[12]) | (~bit[15]&bit[14]&bit[13]&bit[12]))) | (disable_update & out[3]);

		out[2] = (~disable_update & ((~bit[14]&bit[12]) | (bit[15]&bit[14]&bit[13]))) | (disable_update & out[2]);

		out[1] = (~disable_update & ((~bit[14]&~bit[13]&~bit[12]) | (~bit[15]&bit[14]&bit[13]&~bit[12]) | (~bit[15]&~bit[14]&bit[13]&bit[12]) | (bit[15]&bit[14]&bit[13]))) | (disable_update & out[1]);

		out[0] = (~disable_update & ((bit[15]&~bit[14]&bit[13]&bit[12]) | (bit[14]&~bit[13]&~bit[12]) | (~bit[15]&~bit[13]&bit[12]) | (~bit[15]&bit[14]&bit[13]&bit[12]) | (bit[15]&bit[14]&bit[13]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[11]);
		control=control & bit[11];

		//-------------12--------------

		out[4] = (~disable_update & ((~bit[16]&~bit[14]&bit[13]) | (bit[16]&~bit[15]&~bit[14]&bit[13]))) | (disable_update & out[4]);

		out[3] = (~disable_update & ((~bit[15]&bit[14]&bit[13]) | (bit[16]&bit[15]&~bit[14]&bit[13]) | (~bit[16]&~bit[15]&bit[13]))) | (disable_update & out[3]);

		out[2] = (~disable_update & ((~bit[15]&~bit[14]&~bit[13]) | (bit[16]&bit[15]&~bit[14]&bit[13]) | (bit[15]&bit[14]&bit[13]) | (bit[16]&~bit[15]&~bit[14]&bit[13]))) | (disable_update & out[2]);

		out[1] = (~disable_update & ((bit[16]&bit[15]&~bit[14]&bit[13]) | (bit[15]&~bit[14]&~bit[13]) | (bit[15]&bit[14]&bit[13]) | (bit[16]&~bit[15]&~bit[14]&bit[13]) | (~bit[16]&~bit[15]&bit[13]))) | (disable_update & out[1]);

		out[0] = (~disable_update & ((~bit[16]&bit[14]&bit[13]) | (bit[15]&~bit[14]&~bit[13]) | (~bit[15]&~bit[13]) | (bit[16]&~bit[15]&~bit[14]&bit[13]) | (~bit[16]&~bit[15]&bit[13]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[12]);
		control=control & bit[12];

		//-------------13--------------

		out[4] = (~disable_update & ((~bit[17]&bit[16]&bit[15]&~bit[14]) | (bit[17]&bit[16]&bit[15]&~bit[14]) | (~bit[15]&bit[14]))) | (disable_update & out[4]);

		out[3] = (~disable_update & ((~bit[17]&~bit[16]&bit[15]&bit[14]) | (~bit[17]&bit[16]&bit[15]&~bit[14]))) | (disable_update & out[3]);

		out[2] = (~disable_update & ((~bit[17]&bit[16]&bit[15]&~bit[14]) | (bit[17]&~bit[16]&bit[15]&bit[14]) | (bit[17]&bit[16]&bit[15]&~bit[14]) | (~bit[17]&bit[15]&bit[14]) | (~bit[16]&~bit[15]))) | (disable_update & out[2]);

		out[1] = (~disable_update & ((bit[16]&~bit[15]&~bit[14]) | (~bit[17]&~bit[15]&bit[14]) | (bit[17]&~bit[16]&bit[15]&bit[14]) | (bit[17]&bit[16]&bit[15]&~bit[14]))) | (disable_update & out[1]);

		out[0] = (~disable_update & ((bit[17]&~bit[16]&bit[15]&bit[14]) | (bit[17]&bit[16]&bit[15]&~bit[14]) | (~bit[16]&~bit[14]) | (~bit[17]&bit[15]&bit[14]) | (bit[16]&~bit[15]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[13]);
		control=control & bit[13];

		//-------------14--------------

		out[4] = (~disable_update & ((~bit[18]&~bit[17]&~bit[16]) | (bit[16]&~bit[15]) | (~bit[16]&~bit[15]))) | (disable_update & out[4]);

		out[3] = (~disable_update & ((~bit[18]&bit[17]&~bit[16]&bit[15]) | (~bit[17]&bit[16]&bit[15]) | (bit[18]&~bit[16]&bit[15]) | (~bit[16]&~bit[15]))) | (disable_update & out[3]);

		out[2] = (~disable_update & ((bit[18]&~bit[17]&~bit[16]&bit[15]) | (~bit[18]&bit[17]&~bit[16]&bit[15]) | (~bit[17]&bit[16]&~bit[15]) | (~bit[18]&~bit[17]&~bit[15]))) | (disable_update & out[2]);

		out[1] = (~disable_update & ((bit[17]&bit[16]&bit[15]) | (bit[18]&~bit[17]&~bit[16]) | (bit[18]&~bit[16]&bit[15]) | (~bit[18]&bit[16]))) | (disable_update & out[1]);

		out[0] = (~disable_update & ((~bit[18]&bit[17]&bit[16]) | (~bit[18]&~bit[15]) | (bit[18]&~bit[16]&bit[15]) | (bit[16]&~bit[15]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[14]);
		control=control & bit[14];

		//-------------15--------------

		out[4] = (~disable_update & ((bit[20]&bit[19]&bit[18]&bit[17]&bit[16]) | (bit[19]&~bit[18]&~bit[17]&bit[16]) | (~bit[19]&~bit[18]&~bit[17]&bit[16]) | (~bit[19]&~bit[18]&bit[17]&~bit[16]) | (~bit[20]&bit[18]&bit[17]&bit[16]) | (bit[19]&~bit[18]&bit[17]&~bit[16]) | (bit[19]&bit[18]&bit[17]&~bit[16]) | (bit[18]&~bit[17]&~bit[16]) | (~bit[19]&bit[18]))) | (disable_update & out[4]);

		out[3] = (~disable_update & ((bit[20]&~bit[19]&bit[18]&bit[17]&bit[16]) | (bit[19]&bit[18]&~bit[17]&bit[16]) | (~bit[19]&~bit[18]&bit[17]&~bit[16]) | (~bit[20]&bit[18]&bit[17]&bit[16]) | (bit[18]&~bit[17]&~bit[16]))) | (disable_update & out[3]);

		out[2] = (~disable_update & ((~bit[20]&~bit[19]&bit[18]&bit[17]&bit[16]) | (~bit[19]&bit[18]&bit[17]&~bit[16]) | (bit[19]&bit[18]&~bit[17]&bit[16]) | (bit[20]&bit[19]&bit[18]&bit[17]&bit[16]) | (bit[19]&~bit[18]&bit[17]&~bit[16]))) | (disable_update & out[2]);

		out[1] = (~disable_update & ((~bit[20]&~bit[19]&bit[18]&bit[17]&bit[16]) | (~bit[19]&bit[18]&~bit[17]&~bit[16]) | (bit[20]&bit[19]&bit[18]&bit[17]&bit[16]) | (~bit[19]&~bit[18]&~bit[17]&bit[16]) | (bit[19]&bit[18]&bit[17]&~bit[16]))) | (disable_update & out[1]);

		out[0] = (~disable_update & ((bit[20]&~bit[19]&bit[18]&bit[17]&bit[16]) | (~bit[19]&~bit[18]&bit[17]&bit[16]) | (bit[19]&~bit[18]&~bit[17]&bit[16]) | (bit[19]&~bit[18]&bit[17]&~bit[16]) | (bit[19]&bit[18]&bit[17]&~bit[16]) | (bit[18]&~bit[17]&~bit[16]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[15]);
		control=control & bit[15];

		//-------------16--------------

		out[4] = (~disable_update & ((~bit[18]&~bit[17]))) | (disable_update & out[4]);

		out[3] = (~disable_update & ((~bit[20]&~bit[18]&bit[17]) | (~bit[19]&~bit[18]&bit[17]) | (bit[18]&~bit[17]))) | (disable_update & out[3]);

		out[2] = (~disable_update & ((bit[20]&bit[19]&~bit[18]&bit[17]) | (~bit[20]&~bit[19]&bit[18]) | (~bit[20]&~bit[19]&~bit[17]) | (bit[18]&~bit[17]))) | (disable_update & out[2]);

		out[1] = (~disable_update & ((bit[20]&bit[19]&~bit[18]&bit[17]) | (~bit[20]&bit[19]&~bit[18]&~bit[17]) | (~bit[20]&~bit[19]&~bit[18]&bit[17]) | (bit[20]&~bit[19]&~bit[18]&~bit[17]) | (bit[20]&~bit[19]&bit[18]) | (~bit[19]&bit[18]&~bit[17]))) | (disable_update & out[1]);

		out[0] = (~disable_update & ((bit[20]&~bit[19]&~bit[18]&~bit[17]) | (~bit[19]&~bit[18]&bit[17]) | (~bit[20]&~bit[19]&~bit[17]) | (~bit[20]&bit[18]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[16]);
		control=control & bit[16];

		//-------------17--------------

		out[5] = (~disable_update & ((~bit[22]&bit[21]&bit[20]&bit[19]&bit[18]))) | (disable_update & out[5]);

		out[4] = (~disable_update & ((bit[22]&bit[21]&bit[20]&bit[19]&bit[18]) | (~bit[18]))) | (disable_update & out[4]);

		out[3] = (~disable_update & ((bit[22]&bit[21]&bit[20]&bit[19]&bit[18]) | (~bit[19]&~bit[18]) | (~bit[21]&~bit[19]&bit[18]) | (~bit[20]&~bit[19]))) | (disable_update & out[3]);

		out[2] = (~disable_update & ((bit[22]&bit[21]&bit[20]&bit[19]&bit[18]) | (~bit[21]&~bit[19]&~bit[18]) | (bit[21]&bit[20]&~bit[19]&bit[18]) | (~bit[21]&~bit[20]) | (~bit[20]&~bit[19]) | (~bit[20]&~bit[18]))) | (disable_update & out[2]);

		out[1] = (~disable_update & ((bit[22]&bit[21]&bit[20]&bit[19]&bit[18]) | (bit[21]&~bit[20]&bit[19]&bit[18]) | (~bit[21]&bit[20]&bit[19]) | (bit[21]&bit[20]&~bit[19]&bit[18]) | (~bit[21]&~bit[19]&bit[18]) | (~bit[20]&~bit[18]))) | (disable_update & out[1]);

		out[0] = (~disable_update & ((bit[22]&bit[21]&bit[20]&bit[19]&bit[18]) | (bit[21]&~bit[20]&bit[19]&bit[18]) | (~bit[21]&~bit[20]&~bit[18]) | (bit[21]&bit[20]&~bit[19]&bit[18]) | (~bit[21]&~bit[19]&bit[18]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[17]);
		control=control & bit[17];

		//-------------18--------------

		out[4] = (~disable_update & ((bit[22]&bit[21]&~bit[20]&~bit[19]) | (~bit[21]&bit[20]&~bit[19]) | (~bit[22]&~bit[20]&~bit[19]) | (~bit[21]&~bit[20]&~bit[19]))) | (disable_update & out[4]);

		out[3] = (~disable_update & ((bit[22]&~bit[21]&~bit[20]&bit[19]) | (~bit[22]&~bit[20]&~bit[19]) | (~bit[22]&~bit[21]&~bit[20]) | (bit[21]&bit[20]&~bit[19]) | (~bit[21]&~bit[20]&~bit[19]))) | (disable_update & out[3]);

		out[2] = (~disable_update & ((~bit[22]&~bit[21]&bit[20]&bit[19]) | (bit[22]&bit[21]&~bit[20]&~bit[19]) | (~bit[22]&~bit[21]&~bit[20]&~bit[19]) | (bit[21]&~bit[20]&bit[19]) | (bit[21]&bit[20]&~bit[19]))) | (disable_update & out[2]);

		out[1] = (~disable_update & ((bit[22]&~bit[21]&bit[20]&bit[19]) | (~bit[22]&bit[21]&bit[19]) | (~bit[22]&bit[20]&~bit[19]) | (~bit[22]&~bit[21]&~bit[20]) | (bit[21]&bit[20]&~bit[19]) | (~bit[21]&~bit[20]&~bit[19]))) | (disable_update & out[1]);

		out[0] = (~disable_update & ((bit[22]&~bit[21]&bit[20]&bit[19]) | (bit[22]&~bit[21]&~bit[20]&bit[19]) | (bit[21]&~bit[20]&bit[19]) | (~bit[21]&bit[20]&~bit[19]) | (~bit[22]&bit[20]&~bit[19]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[18]);
		control=control & bit[18];

		//-------------19--------------

		out[5] = (~disable_update & ((~bit[23]&bit[22]&bit[21]&bit[20]) | (~bit[23]&~bit[22]&~bit[21]&~bit[20]))) | (disable_update & out[5]);

		out[4] = (~disable_update & ((bit[23]&bit[22]&bit[21]&bit[20]) | (bit[23]&bit[22]&~bit[21]&~bit[20]) | (~bit[23]&~bit[22]&bit[21]&~bit[20]) | (~bit[23]&bit[22]&~bit[21]&~bit[20]) | (~bit[23]&bit[22]&bit[21]&~bit[20]) | (bit[23]&~bit[22]&~bit[20]))) | (disable_update & out[4]);

		out[3] = (~disable_update & ((~bit[23]&~bit[22]&bit[21]&~bit[20]) | (~bit[23]&bit[22]&~bit[21]&~bit[20]) | (~bit[23]&~bit[21]&bit[20]) | (bit[23]&~bit[21]) | (bit[23]&bit[22]&bit[21]))) | (disable_update & out[3]);

		out[2] = (~disable_update & ((~bit[23]&~bit[22]&bit[21]&bit[20]) | (~bit[23]&bit[22]&~bit[21]&~bit[20]) | (bit[23]&~bit[22]&~bit[20]) | (bit[23]&bit[22]&bit[21]))) | (disable_update & out[2]);

		out[1] = (~disable_update & ((~bit[24]&bit[22]&bit[21]&bit[20]) | (~bit[22]&~bit[21]&bit[20]) | (~bit[23]&bit[22]&bit[21]&~bit[20]) | (bit[23]&~bit[22]&~bit[20]))) | (disable_update & out[1]);

		out[0] = (~disable_update & ((bit[23]&bit[21]&~bit[20]) | (~bit[23]&~bit[22]&~bit[21]&~bit[20]) | (bit[23]&bit[22]&~bit[21]&~bit[20]) | (~bit[23]&~bit[21]&bit[20]) | (~bit[23]&bit[22]&bit[21]&~bit[20]) | (bit[23]&bit[22]&bit[21]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[19]);
		control=control & bit[19];

		//-------------20--------------

		out[5] = (~disable_update & ((~bit[25]&bit[24]&~bit[23]&bit[22]&bit[21]))) | (disable_update & out[5]);

		out[4] = (~disable_update & ((~bit[24]&~bit[23]&bit[22]&~bit[21]) | (bit[25]&bit[24]&~bit[23]&bit[22]&bit[21]) | (~bit[24]&bit[23]&bit[22]&bit[21]) | (bit[24]&bit[23]&bit[22]&bit[21]) | (~bit[22]&~bit[21]))) | (disable_update & out[4]);

		out[3] = (~disable_update & ((bit[25]&bit[24]&~bit[23]&bit[22]&bit[21]) | (~bit[24]&~bit[23]&~bit[22]) | (bit[24]&~bit[23]&~bit[21]) | (bit[23]&bit[22]))) | (disable_update & out[3]);

		out[2] = (~disable_update & ((bit[25]&bit[24]&~bit[23]&bit[22]&bit[21]) | (bit[24]&~bit[23]&~bit[22]&bit[21]) | (~bit[24]&bit[23]&bit[22]&bit[21]) | (~bit[24]&~bit[22]&~bit[21]) | (bit[23]&~bit[22]&~bit[21]))) | (disable_update & out[2]);

		out[1] = (~disable_update & ((~bit[25]&bit[24]&bit[23]&bit[21]) | (~bit[24]&~bit[23]&bit[22]&~bit[21]) | (bit[25]&bit[24]&~bit[23]&bit[22]&bit[21]) | (~bit[24]&bit[23]&~bit[21]) | (bit[23]&~bit[22]&bit[21]) | (bit[24]&~bit[23]&~bit[21]))) | (disable_update & out[1]);

		out[0] = (~disable_update & ((~bit[25]&bit[23]&bit[22]&bit[21]) | (~bit[24]&bit[23]&~bit[22]&bit[21]) | (bit[25]&bit[24]&~bit[23]&bit[22]&bit[21]) | (bit[24]&~bit[23]&~bit[22]&bit[21]) | (bit[24]&bit[23]&bit[22]&bit[21]) | (bit[24]&bit[22]&~bit[21]) | (~bit[24]&~bit[23]&bit[22]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[20]);
		control=control & bit[20];

		//-------------21--------------

		out[5] = (~disable_update & ((~bit[26]&~bit[24]&bit[23]&bit[22]) | (~bit[25]&~bit[24]&bit[23]&bit[22]))) | (disable_update & out[5]);

		out[4] = (~disable_update & ((bit[26]&bit[25]&~bit[24]&bit[23]&bit[22]) | (~bit[25]&bit[24]&bit[23]&bit[22]) | (~bit[25]&~bit[24]&~bit[22]) | (bit[25]&bit[24]&bit[23]&bit[22]) | (~bit[23]&~bit[22]))) | (disable_update & out[4]);

		out[3] = (~disable_update & ((bit[26]&bit[25]&~bit[24]&bit[23]&bit[22]) | (~bit[25]&bit[24]&bit[23]&bit[22]) | (bit[25]&~bit[24]&bit[23]&~bit[22]) | (bit[24]&bit[23]&~bit[22]))) | (disable_update & out[3]);

		out[2] = (~disable_update & ((bit[26]&bit[25]&~bit[24]&bit[23]&bit[22]) | (bit[25]&bit[24]&bit[23]&bit[22]) | (bit[25]&~bit[24]&bit[23]&~bit[22]) | (~bit[24]&~bit[23]&~bit[22]) | (bit[24]&bit[23]&~bit[22]) | (~bit[24]&~bit[23]&bit[22]) | (~bit[25]&~bit[23]))) | (disable_update & out[2]);

		out[1] = (~disable_update & ((~bit[26]&bit[25]&bit[24]&bit[22]) | (~bit[26]&~bit[25]&~bit[24]&bit[22]) | (bit[25]&bit[24]&~bit[23]) | (bit[25]&~bit[24]&bit[23]&~bit[22]) | (~bit[25]&~bit[24]&~bit[23]) | (~bit[24]&~bit[23]&bit[22]))) | (disable_update & out[1]);

		out[0] = (~disable_update & ((~bit[26]&bit[24]&bit[23]&bit[22]) | (~bit[25]&bit[24]&bit[23]&~bit[22]) | (bit[26]&bit[25]&~bit[24]&bit[23]&bit[22]) | (~bit[25]&~bit[24]&bit[23]&bit[22]) | (bit[25]&bit[24]&bit[22]) | (~bit[24]&~bit[23]&~bit[22]) | (~bit[25]&~bit[24]&~bit[23]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[21]);
		control=control & bit[21];

		//-------------22--------------

		out[5] = (~disable_update & ((~bit[25]&bit[24]&bit[23]))) | (disable_update & out[5]);

		out[4] = (~disable_update & ((~bit[26]&~bit[24]&~bit[23]) | (bit[26]&bit[25]&bit[24]&bit[23]) | (~bit[26]&bit[25]&bit[24]&bit[23]) | (~bit[25]&~bit[24]&~bit[23]))) | (disable_update & out[4]);

		out[3] = (~disable_update & ((bit[26]&bit[25]&bit[24]&bit[23]) | (bit[26]&bit[25]&~bit[24]&~bit[23]) | (~bit[26]&bit[25]&bit[24]&bit[23]) | (~bit[25]&bit[24]&~bit[23]))) | (disable_update & out[3]);

		out[2] = (~disable_update & ((~bit[27]&~bit[26]&bit[24]&bit[23]) | (bit[26]&bit[25]&~bit[24]&~bit[23]) | (~bit[26]&bit[25]&bit[24]&bit[23]) | (bit[24]&~bit[23]))) | (disable_update & out[2]);

		out[1] = (~disable_update & ((bit[27]&~bit[26]&~bit[25]&bit[23]) | (~bit[27]&bit[25]&bit[24]&bit[23]) | (~bit[26]&~bit[25]&~bit[24]) | (~bit[26]&bit[25]&bit[24]&~bit[23]) | (bit[26]&bit[25]&~bit[24]&~bit[23]) | (~bit[25]&~bit[24]&bit[23]))) | (disable_update & out[1]);

		out[0] = (~disable_update & ((bit[27]&~bit[26]&~bit[25]&bit[23]) | (~bit[27]&bit[26]&bit[24]&bit[23]) | (~bit[26]&~bit[25]&~bit[23]) | (bit[26]&bit[25]&bit[24]&bit[23]) | (bit[26]&bit[25]&~bit[24]&~bit[23]) | (~bit[26]&~bit[24]&bit[23]) | (~bit[25]&~bit[24]&~bit[23]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[22]);
		control=control & bit[22];

		//-------------23--------------

		out[5] = (~disable_update & ((~bit[28]&bit[27]&~bit[26]&bit[25]&bit[24]))) | (disable_update & out[5]);

		out[4] = (~disable_update & ((bit[28]&bit[27]&~bit[26]&bit[25]&bit[24]) | (~bit[26]&~bit[25]&~bit[24]) | (bit[26]&~bit[25]&~bit[24]) | (bit[26]&bit[25]&bit[24]))) | (disable_update & out[4]);

		out[3] = (~disable_update & ((bit[28]&bit[27]&~bit[26]&bit[25]&bit[24]) | (~bit[26]&~bit[25]&bit[24]) | (bit[26]&bit[25]&bit[24]) | (bit[25]&~bit[24]))) | (disable_update & out[3]);

		out[2] = (~disable_update & ((bit[28]&bit[27]&~bit[26]&bit[25]&bit[24]) | (bit[26]&~bit[25]&bit[24]) | (~bit[26]&~bit[25]&~bit[24]) | (~bit[26]&bit[25]&~bit[24]) | (~bit[27]&bit[25]))) | (disable_update & out[2]);

		out[1] = (~disable_update & ((~bit[28]&bit[27]&bit[26]&bit[25]) | (bit[28]&bit[27]&~bit[26]&bit[25]&bit[24]) | (bit[27]&bit[26]&bit[25]&~bit[24]) | (~bit[27]&~bit[26]&~bit[24]) | (~bit[27]&bit[26]&~bit[25]))) | (disable_update & out[1]);

		out[0] = (~disable_update & ((~bit[28]&bit[27]&~bit[26]&bit[25]&bit[24]) | (~bit[27]&~bit[25]&bit[24]) | (~bit[28]&bit[26]&bit[24]) | (bit[27]&bit[26]&bit[24]) | (bit[26]&~bit[25]&~bit[24]) | (~bit[26]&bit[25]&~bit[24]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[23]);
		control=control & bit[23];

		//-------------24--------------

		out[5] = (~disable_update & ((bit[28]&~bit[27]&bit[26]&bit[25]) | (~bit[28]&~bit[27]&bit[26]&bit[25]))) | (disable_update & out[5]);

		out[4] = (~disable_update & ((bit[29]&bit[28]&bit[27]&bit[26]&bit[25]) | (bit[29]&~bit[28]&bit[27]&bit[26]&bit[25]) | (~bit[29]&bit[27]&bit[26]&bit[25]) | (bit[27]&~bit[26]&~bit[25]) | (~bit[27]&~bit[25]))) | (disable_update & out[4]);

		out[3] = (~disable_update & ((~bit[28]&~bit[27]&~bit[26]&~bit[25]) | (bit[27]&bit[26]))) | (disable_update & out[3]);

		out[2] = (~disable_update & ((~bit[29]&bit[27]&bit[26]&bit[25]) | (~bit[28]&~bit[27]&bit[26]&bit[25]) | (bit[28]&~bit[27]&~bit[26]&~bit[25]) | (bit[27]&~bit[26]&~bit[25]) | (~bit[28]&bit[27]) | (~bit[27]&~bit[26]&bit[25]))) | (disable_update & out[2]);

		out[1] = (~disable_update & ((~bit[29]&~bit[28]&bit[27]&bit[26]&bit[25]) | (bit[29]&bit[28]&bit[27]&bit[26]&bit[25]) | (~bit[28]&bit[27]&~bit[26]&~bit[25]) | (~bit[29]&bit[28]&~bit[27]&bit[25]) | (~bit[28]&~bit[27]&bit[26]&~bit[25]) | (bit[28]&~bit[27]&~bit[26]&~bit[25]) | (bit[28]&~bit[26]&bit[25]) | (~bit[27]&~bit[26]&bit[25]))) | (disable_update & out[1]);

		out[0] = (~disable_update & ((bit[29]&~bit[28]&bit[27]&bit[26]&bit[25]) | (~bit[29]&~bit[27]&bit[26]&bit[25]) | (bit[28]&~bit[27]&bit[26]&bit[25]) | (~bit[28]&~bit[27]&bit[26]&~bit[25]) | (bit[28]&~bit[27]&~bit[26]&~bit[25]) | (~bit[28]&~bit[26]&bit[25]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[24]);
		control=control & bit[24];

		//-------------25--------------

		out[5] = (~disable_update & ((~bit[29]&bit[28]&bit[27]&bit[26]) | (bit[29]&~bit[28]&bit[27]&bit[26]))) | (disable_update & out[5]);

		out[4] = (~disable_update & ((bit[29]&bit[28]&bit[27]&bit[26]) | (bit[29]&bit[28]&~bit[27]&~bit[26]) | (~bit[29]&~bit[28]&bit[27]&~bit[26]) | (bit[29]&~bit[28]&~bit[27]&~bit[26]) | (~bit[29]&~bit[27]&~bit[26]))) | (disable_update & out[4]);

		out[3] = (~disable_update & ((~bit[28]&~bit[27]&bit[26]) | (bit[29]&~bit[28]&bit[27]&~bit[26]) | (~bit[29]&bit[28]&bit[27]&~bit[26]) | (bit[29]&bit[28]&bit[27]))) | (disable_update & out[3]);

		out[2] = (~disable_update & ((bit[29]&~bit[28]&bit[27]&bit[26]) | (~bit[30]&bit[29]&bit[27]&bit[26]) | (~bit[29]&bit[28]&~bit[27]&bit[26]) | (bit[29]&~bit[28]&~bit[27]&~bit[26]) | (~bit[29]&~bit[27]&~bit[26]) | (bit[29]&~bit[28]&bit[27]&~bit[26]) | (~bit[29]&bit[28]&bit[27]&~bit[26]))) | (disable_update & out[2]);

		out[1] = (~disable_update & ((~bit[30]&bit[28]&bit[27]&bit[26]) | (bit[29]&bit[28]&~bit[27]&~bit[26]) | (~bit[30]&bit[29]&bit[27]&bit[26]) | (~bit[29]&~bit[28]&~bit[27]) | (~bit[29]&bit[28]&~bit[27]&bit[26]) | (bit[29]&~bit[28]&bit[27]&~bit[26]) | (bit[29]&bit[28]&bit[27]))) | (disable_update & out[1]);

		out[0] = (~disable_update & ((bit[30]&bit[29]&bit[27]&bit[26]) | (~bit[29]&~bit[28]&bit[27]&~bit[26]) | (bit[30]&bit[28]&bit[27]) | (bit[29]&~bit[28]&~bit[27]&~bit[26]) | (bit[28]&~bit[27]&bit[26]) | (~bit[29]&bit[28]&bit[27]&~bit[26]) | (bit[29]&bit[28]&bit[27]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[25]);
		control=control & bit[25];

		//-------------26--------------

		out[5] = (~disable_update & ((bit[30]&bit[29]&~bit[28]&bit[27]) | (~bit[29]&bit[28]&bit[27]))) | (disable_update & out[5]);

		out[4] = (~disable_update & ((~bit[30]&~bit[29]&~bit[28]&~bit[27]) | (bit[30]&~bit[29]&~bit[28]&~bit[27]) | (bit[29]&bit[28]&bit[27]))) | (disable_update & out[4]);

		out[3] = (~disable_update & ((~bit[30]&~bit[29]&~bit[28]&~bit[27]) | (~bit[30]&~bit[29]&bit[28]&~bit[27]) | (bit[29]&~bit[28]&~bit[27]) | (bit[29]&bit[28]&bit[27]))) | (disable_update & out[3]);

		out[2] = (~disable_update & ((bit[31]&~bit[30]&bit[29]&bit[28]) | (~bit[31]&~bit[30]&bit[29]&bit[28]&bit[27]) | (bit[30]&bit[29]&~bit[28]&bit[27]) | (bit[30]&~bit[29]&bit[28]&~bit[27]) | (bit[30]&~bit[29]&~bit[28]&~bit[27]) | (bit[29]&~bit[27]))) | (disable_update & out[2]);

		out[1] = (~disable_update & ((~bit[31]&~bit[30]&bit[29]&bit[28]&bit[27]) | (~bit[30]&~bit[29]&~bit[28]&~bit[27]) | (bit[30]&~bit[29]&bit[28]&~bit[27]) | (~bit[31]&bit[30]&bit[29]&bit[27]) | (~bit[30]&~bit[29]&bit[27]) | (bit[29]&~bit[28]&~bit[27]))) | (disable_update & out[1]);

		out[0] = (~disable_update & ((bit[31]&~bit[30]&bit[29]&bit[28]) | (~bit[31]&~bit[29]&bit[28]&bit[27]) | (~bit[30]&~bit[29]&bit[28]&~bit[27]) | (~bit[31]&bit[30]&bit[29]&bit[27]) | (bit[30]&~bit[29]&~bit[28]&~bit[27]) | (bit[30]&~bit[28]&bit[27]) | (~bit[30]&bit[29]&~bit[27]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[26]);
		control=control & bit[26];

		//-------------27--------------
/*
		out[4] = (~disable_update & ((~bit[29]&~bit[28]) | (~bit[30]&~bit[28]))) | (disable_update & out[4]);

		out[3] = (~disable_update & ((~bit[31]&~bit[30]&~bit[29]&bit[28]) | (bit[31]&bit[30]&bit[29]&~bit[28]) | (~bit[31]&bit[30]&bit[29]&~bit[28]))) | (disable_update & out[3]);

		out[2] = (~disable_update & ((~bit[31]&bit[30]&bit[29]&~bit[28]) | (~bit[31]&bit[30]&~bit[29]) | (bit[31]&~bit[30]&~bit[29]) | (~bit[29]&~bit[28]))) | (disable_update & out[2]);

		out[1] = (~disable_update & ((~bit[31]&~bit[30]&~bit[29]&bit[28]) | (bit[31]&bit[30]&bit[29]&~bit[28]) | (~bit[31]&bit[30]&bit[29]&~bit[28]) | (bit[31]&~bit[30]&~bit[29]) | (~bit[30]&~bit[28]))) | (disable_update & out[1]);

		out[0] = (~disable_update & ((bit[31]&~bit[29]&bit[28]) | (bit[31]&bit[30]&bit[29]&~bit[28]) | (~bit[31]&~bit[30]&~bit[28]) | (~bit[31]&bit[30]&~bit[29]))) | (disable_update & out[0]);

*/
		//-------------28--------------

		



		//out[5] = (~disable_update & (---put espresso output---)) | (disable_update & out[5]);

		out[5] = (~disable_update & ((~bit[32]&bit[31]&bit[30]&bit[29]&bit[28]) | (~bit[32]&~bit[31] &bit[30]&bit[29]&bit[28]) | (bit[32]&~bit[31]&bit[30]&bit[29]&bit[28]) | (bit[32]&bit[31]&~bit[30]&bit[29]&bit[28]) | (~bit[32]&bit[31]&~bit[30]&bit[29]&bit[28]))) | (disable_update & out[5]);

		out[4] = (~disable_update & ((bit[32]&bit[31]&bit[30]&bit[29]&bit[28]) | (~bit[29]&~bit[28]) | (~bit[30]&~bit[28]))) | (disable_update & out[4]);

		out[3] = (~disable_update & ((~bit[31]&~bit[30]&~bit[29]&bit[28]) | (bit[32]&bit[31]&bit[30]&bit[29]&bit[28]) | (bit[31]&bit[30]&bit[29]&~bit[28]) | (~bit[31]&bit[30]&bit[29]&~bit[28]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[32]&~bit[31]&bit[30]&bit[29]&bit[28]) | (bit[32]&bit[31]&~bit[30]&bit[29]&bit[28]) | (~bit[32]&bit[31]&~bit[30]&bit[29]&bit[28]) | (bit[32]&bit[31]&bit[30]&bit[29]&bit[28]) | (~bit[31]&bit[30]&bit[29]&~bit[28]) | (~bit[31]&bit[30]&~bit[29]) | (bit[31]&~bit[30]&~bit[29]&bit[28]) | (~bit[29]&~bit[28]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[32]&~bit[31]&bit[30]&bit[29]&bit[28]) | (~bit[31]&~bit[30]&~bit[29]&bit[28]) | (~bit[32]&bit[31]&~bit[30]&bit[29]&bit[28]) | (bit[32]&bit[31]&bit[30]&bit[29]&bit[28]) | (bit[31]&bit[30]&bit[29]&~bit[28]) | (~bit[31]&bit[30]&bit[29]&~bit[28]) | (bit[31]&~bit[30]&~bit[29]&bit[28]) | (~bit[30]&~bit[28]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[32]&bit[31]&bit[30]&bit[29]&bit[28]) | (bit[32]&bit[31]&~bit[30]&bit[29]&bit[28]) | (~bit[31]&~bit[30]&~bit[28]) | (bit[30]&~bit[29]&bit[28]) | (bit[31]&bit[30]&bit[29]&~bit[28]) | (~bit[31]&bit[30]&~bit[29]) | (bit[31]&~bit[30]&~bit[29]&bit[28]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[27]);
		control=control & bit[27];

		//-------------28-1--------------

		out[5] = (~disable_update & ((~bit[33]&~bit[32]&bit[31]&bit[30]&bit[29]) | (bit[33]&~bit[32]&bit[31]&bit[30]&bit[29]) | (bit[32]&bit[31]&bit[30]&bit[29]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[31]&~bit[29]) | (bit[31]&~bit[30]&~bit[29]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[33]&~bit[32]&bit[31]&bit[30]&bit[29]) | (bit[32]&~bit[31]&~bit[30]&bit[29]) | (~bit[32]&bit[31]&bit[30]&~bit[29]) | (~bit[32]&~bit[31]&~bit[30]) | (bit[32]&bit[31]&bit[30]&~bit[29]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[33]&~bit[32]&bit[31]&bit[30]&bit[29]) | (bit[32]&~bit[31]&~bit[30]&~bit[29]) | (bit[32]&bit[31]&bit[30]&bit[29]) | (~bit[32]&bit[31]&bit[30]&~bit[29]) | (bit[31]&~bit[30]&bit[29]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[33]&~bit[32]&bit[31]&bit[30]&bit[29]) | (~bit[31]&bit[30]&bit[29]) | (bit[31]&~bit[30]&~bit[29]) | (~bit[32]&~bit[31]&~bit[30])| (bit[32]&bit[31]&bit[30]&~bit[29]) | (bit[31]&~bit[30]&bit[29]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[33]&bit[32]&bit[31]&bit[30]) | (bit[32]&~bit[31]&~bit[30]&~bit[29]) | (bit[32]&~bit[31]&~bit[30]&bit[29]) | (~bit[32]&~bit[31]&bit[30]) | (~bit[32]&bit[31]&~bit[30]) | (bit[32]&bit[31]&bit[30]&~bit[29]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[28]);
		control=control & bit[28];

		//-------------29--------------

		out[5] = (~disable_update & ((~bit[34]&~bit[33]&bit[32]&bit[31]&bit[30]) | (bit[34]&bit[33]&~bit[32]&bit[31]&bit[30]) | (~bit[34]&bit[33]&~bit[32]&bit[31]&bit[30]) | (~bit[32]&~bit[31]&~bit[30]) | (~bit[33]&~bit[32]&bit[31]&bit[30]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[34]&bit[33]&bit[32]&bit[31]&bit[30]) | (~bit[33]&bit[32]&~bit[31]&~bit[30]) | (bit[34]&bit[32]&bit[31]&bit[30]) | (bit[33]&bit[32]&~bit[31]&~bit[30]) | (~bit[33]&bit[31]&~bit[30]) | (~bit[32]&bit[31]&~bit[30]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[33]&~bit[32]&~bit[31]&bit[30]) | (~bit[33]&~bit[32]&bit[31]&bit[30]) | (~bit[33]&bit[32]&~bit[31]&~bit[30]) | (bit[33]&bit[32]&~bit[31]&~bit[30]) | (bit[33]&bit[32]&bit[31]&~bit[30]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[34]&~bit[33]&bit[32]&bit[31]&bit[30]) | (~bit[34]&bit[33]&bit[32]&bit[31]&bit[30]) | (bit[34]&bit[33]&~bit[32]&bit[31]&bit[30]) | (~bit[34]&bit[33]&~bit[32]&bit[31]&bit[30]) | (~bit[33]&~bit[32]&~bit[31]&bit[30]) | (~bit[33]&bit[32]&~bit[31]&~bit[30]) | (bit[33]&~bit[32]&~bit[31]&bit[30]) | (~bit[32]&bit[31]&~bit[30]) | (bit[33]&bit[32]&bit[31]&~bit[30]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[34]&~bit[33]&bit[32]&bit[31]&bit[30]) | (~bit[33]&~bit[32]&~bit[30]) | (~bit[34]&bit[33]&~bit[32]&bit[31]&bit[30]) | (bit[34]&bit[32]&bit[31]&bit[30]) | (bit[33]&bit[32]&~bit[31]&~bit[30]) | (bit[33]&~bit[32]&~bit[31]&bit[30]) | (~bit[33]&bit[31]&~bit[30]) | (bit[33]&bit[32]&bit[31]&~bit[30]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[34]&~bit[33]&~bit[32]&bit[31]) | (bit[34]&bit[33]&~bit[32]&bit[31]&bit[30]) | (~bit[33]&~bit[32]&bit[31]&~bit[30]) | (~bit[33]&bit[32]&~bit[31]) | (bit[34]&bit[32]&bit[31]&bit[30]) | (bit[33]&bit[32]&~bit[31]&~bit[30]) | (bit[33]&~bit[32]&~bit[31]&bit[30]) | (bit[33]&bit[32]&bit[31]&~bit[30]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[29]);
		control=control & bit[29];

		//-------------30--------------

		out[5] = (~disable_update & ((~bit[33]&bit[32]&bit[31]) | (bit[34]&bit[33]&~bit[32]&bit[31]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[35]&bit[34]&bit[33]&bit[32]&bit[31]) | (bit[34]&~bit[33]&~bit[32]&~bit[31]) | (~bit[34]&~bit[33]&~bit[32]&~bit[31]) | (~bit[35]&bit[33]&bit[32]&bit[31]) | (~bit[34]&bit[33]&bit[32]&bit[31]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[35]&bit[34]&bit[33]&bit[32]&bit[31]) | (~bit[34]&~bit[33]&~bit[32]&bit[31]) | (~bit[35]&bit[33]&bit[32]&bit[31]) | (~bit[34]&~bit[33]&bit[32]&~bit[31]) | (~bit[34]&bit[33]&~bit[31]) | (bit[34]&bit[32]&~bit[31]) | (bit[33]&~bit[32]&~bit[31]) | (~bit[34]&bit[33]&bit[32]&bit[31]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[35]&~bit[34]&bit[32]&bit[31]) | (bit[34]&bit[33]&~bit[32]&bit[31]) | (~bit[35]&bit[33]&bit[32]&bit[31]) | (~bit[34]&~bit[33]&bit[32]&~bit[31]) | (bit[33]&~bit[32]&~bit[31]) | (~bit[34]&bit[33]&bit[32]&bit[31]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[35]&~bit[34]&bit[33]&bit[32]) | (bit[35]&~bit[34]&~bit[33]&bit[32]&bit[31]) | (~bit[35]&bit[34]&~bit[33]&bit[32]) | (bit[35]&bit[34]&bit[33]&bit[32]&bit[31]) | (bit[34]&~bit[33]&bit[32]&~bit[31]) | (~bit[34]&~bit[33]&~bit[32]&~bit[31]) | (bit[34]&bit[33]&~bit[32]&bit[31]) | (bit[34]&~bit[33]&~bit[32]&bit[31]) | (~bit[34]&bit[33]&~bit[31]) | (bit[33]&~bit[32]&~bit[31]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[35]&bit[33]&~bit[32]&bit[31]) | (bit[35]&~bit[34]&~bit[33]&bit[32]&bit[31]) | (bit[34]&~bit[33]&~bit[32]&~bit[31]) | (~bit[35]&~bit[34]&bit[32]&bit[31]) | (~bit[34]&bit[33]&~bit[32]) | (bit[34]&~bit[33]&~bit[32]&bit[31]) | (~bit[34]&~bit[33]&bit[32]&~bit[31]) | (bit[34]&bit[32]&~bit[31]) | (~bit[34]&bit[33]&bit[32]&bit[31]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[30]);
		control=control & bit[30];

		//-------------31--------------

		out[5] = (~disable_update & ((bit[36]&bit[35]&~bit[34]&bit[33]&bit[32]) | (~bit[36]&bit[34]&bit[33]&bit[32]) | (~bit[36]&bit[35]&~bit[34]&bit[33]&bit[32]) | (~bit[35]&bit[34]&bit[33]&bit[32]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[36]&bit[35]&bit[34]&bit[33]&bit[32]) | (~bit[35]&~bit[34]&bit[33]&~bit[32]) | (~bit[33]&~bit[32]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[36]&bit[35]&~bit[34]&bit[33]&bit[32]) | (bit[36]&bit[35]&bit[34]&bit[33]&bit[32]) | (~bit[35]&~bit[34]&~bit[33]&bit[32]) | (~bit[35]&~bit[34]&~bit[33]&~bit[32]) | (bit[35]&~bit[34]&~bit[33]&bit[32]) | (~bit[35]&bit[34]&bit[33]&~bit[32]) | (bit[35]&bit[33]&~bit[32]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[36]&bit[35]&~bit[34]&bit[33]&bit[32]) | (bit[36]&bit[35]&bit[34]&bit[33]&bit[32]) | (bit[35]&~bit[34]&~bit[32]) | (~bit[35]&bit[34]&bit[33]&~bit[32]) | (bit[34]&~bit[33]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[36]&bit[35]&~bit[34]&bit[33]&bit[32]) | (bit[36]&bit[35]&bit[34]&bit[33]&bit[32]) | (~bit[35]&bit[34]&bit[33]&bit[32]) | (~bit[35]&~bit[34]&bit[33]&~bit[32]) | (~bit[35]&~bit[34]&~bit[33]&bit[32]) | (bit[35]&~bit[34]&~bit[32]) | (bit[35]&bit[33]&~bit[32]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[36]&~bit[35]&bit[34]) | (~bit[35]&~bit[34]&~bit[33]&~bit[32]) | (bit[35]&~bit[34]&~bit[33]&bit[32]) | (bit[35]&~bit[34]&~bit[32]) | (~bit[35]&bit[34]&~bit[33]) | (~bit[35]&bit[34]&bit[33]&~bit[32]) | (bit[35]&bit[33]&~bit[32]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[31]);
		control=control & bit[31];

		//-------------32--------------

		out[5] = (~disable_update & ((bit[36]&bit[35]&bit[34]&bit[33]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[36]&bit[34]&~bit[33]) | (~bit[35]&bit[34]&~bit[33]) | (~bit[34]&~bit[33]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[36]&bit[35]&bit[34]&~bit[33]) | (~bit[35]&~bit[34]) | (~bit[34]&~bit[33]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[37]&bit[36]&bit[35]&bit[33]) | (~bit[35]&bit[34]&~bit[33]) | (bit[36]&bit[35]&bit[34]&~bit[33]) | (bit[35]&~bit[34]&bit[33]) | (~bit[36]&~bit[35]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[36]&~bit[35]&bit[34]&bit[33]) | (~bit[36]&bit[35]&~bit[34]&bit[33]) | (bit[36]&~bit[35]&~bit[34]&~bit[33]) | (~bit[36]&bit[34]&~bit[33]) | (bit[36]&bit[35]&bit[34]&~bit[33]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[36]&~bit[34]&~bit[33]) | (bit[36]&~bit[35]&~bit[34]) | (bit[36]&bit[35]&bit[34]&~bit[33]) | (bit[35]&~bit[34]&bit[33]) | (~bit[36]&bit[35]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[32]);
		control=control & bit[32];

		//-------------33--------------

		out[5] = (~disable_update & ((~bit[38]&~bit[37]&bit[35]&bit[34]) | (~bit[36]&bit[35]&bit[34]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[37]&bit[36]&bit[35]&bit[34]) | (~bit[37]&bit[36]&bit[35]&~bit[34]) | (bit[38]&bit[36]&bit[35]&bit[34]) | (~bit[36]&bit[35]&~bit[34]) | (~bit[35]&~bit[34]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[37]&bit[36]&bit[35]&bit[34]) | (bit[37]&bit[36]&bit[35]&~bit[34]) | (bit[38]&bit[36]&bit[35]&bit[34]) | (~bit[37]&~bit[36]) | (~bit[35]&~bit[34]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[38]&bit[37]&bit[35]&bit[34]) | (bit[38]&~bit[37]&bit[36]&bit[34]) | (bit[37]&~bit[36]&~bit[35]&bit[34]) | (~bit[36]&~bit[35]&~bit[34]) | (~bit[37]&bit[36]&~bit[35]) | (bit[37]&bit[36]&bit[35] &~bit[34]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[37]&~bit[36]&~bit[35]&bit[34]) | (~bit[37]&~bit[36]&~bit[34]) | (~bit[36]&bit[35]&bit[34]) | (bit[38]&bit[36]&bit[35]&bit[34]) | (~bit[36]&bit[35]&~bit[34]) | (bit[37]&bit[36]&~bit[35]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[38]&~bit[37]&bit[35]&bit[34]) | (~bit[37]&bit[36]&bit[35]&~bit[34]) | (bit[37]&~bit[35]&~bit[34]) | (bit[37]&bit[36]&bit[35]&~bit[34]) | (~bit[37]&~bit[35]&bit[34]) | (bit[38]&bit[36]&bit[35]&bit[34]) | (bit[37]&bit[36]&~bit[35]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[33]);
		control=control & bit[33];

		//-------------34--------------

		out[5] = (~disable_update & ((~bit[39]&bit[38]&bit[37]&~bit[36]&bit[35]) | (bit[39]&bit[38]&bit[37]&~bit[36]&bit[35]) | (bit[38]&~bit[37]&bit[36]&bit[35]) | (~bit[38]&bit[36]&bit[35]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[38]&~bit[36]&~bit[35]) | (bit[38]&bit[37]&bit[36]&bit[35]) | (bit[38]&~bit[37]&~bit[36]&~bit[35]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[39]&~bit[38]&~bit[37]&bit[36]&bit[35]) | (~bit[39]&bit[38]&bit[37]&bit[35]) | (bit[39]&bit[38]&bit[37]&~bit[36]&bit[35]) | (~bit[39]&~bit[38]&~bit[37]&bit[36]) | (~bit[38]&~bit[37]&~bit[35]) | (bit[38]&bit[37]&~bit[36]&~bit[35]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[39]&bit[38]&bit[37]&~bit[36]&bit[35]) | (~bit[39]&~bit[38]&bit[37]&bit[36]) | (~bit[38]&~bit[37]&~bit[36]&bit[35]) | (~bit[38]&bit[37]&bit[36]&~bit[35]) | (bit[38]&~bit[37]&bit[36]&bit[35]) | (bit[38]&~bit[37]&~bit[36]&~bit[35]) | (bit[38]&bit[36]&~bit[35]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[39]&bit[38]&bit[37]&~bit[36]&bit[35]) | (~bit[39]&~bit[38]&~bit[37]&bit[36]) | (bit[38]&bit[37]&bit[36]&bit[35]) | (~bit[38]&bit[37]&bit[36]&~bit[35]) | (bit[38]&bit[37]&~bit[36]&~bit[35]) | (~bit[37]&bit[36]&~bit[35]) | (bit[38]&~bit[37]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[39]&~bit[38]&~bit[37]&bit[36]&bit[35]) | (~bit[38]&bit[37]&~bit[36]&bit[35]) | (bit[39]&bit[38]&bit[37]&~bit[36]&bit[35]) | (~bit[39]&bit[38]&~bit[37]&bit[36]) | (bit[38]&~bit[37]&~bit[36]&~bit[35]) | (bit[38]&bit[37]&~bit[36]&~bit[35]) | (bit[38]&bit[36]&~bit[35]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[34]);
		control=control & bit[34];

		//-------------35--------------
/*
		//out[5] = (~disable_update & ((~bit[39]&~bit[38]&~bit[37]&~bit[36]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[39]&~bit[38]&~bit[37]&~bit[36]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[39]&bit[38]&~bit[37]&~bit[36]) | (bit[39]&bit[38]&~bit[37]&~bit[36]) | (bit[39]&~bit[38]&~bit[37]&~bit[36]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[39]&~bit[38]&bit[37]&~bit[36]) | (bit[39]&~bit[38]&~bit[37]&~bit[36]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[39]&~bit[38]&bit[37]&~bit[36]) | (~bit[39]&bit[38]&~bit[37]&~bit[36]) | (bit[39]&~bit[38]&~bit[37]&~bit[36]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[39]&bit[38]&bit[37]&~bit[36]) | (bit[39]&~bit[38]&bit[37]&~bit[36]) | (~bit[39]&~bit[38]&~bit[37]&~bit[36]) | (bit[39]&bit[38]&~bit[37]&~bit[36]))) | (disable_update & out[0]);
*/
		//-------------36--------------

		out[5] = (~disable_update & ((bit[40]&~bit[39]&~bit[38]&~bit[37]&bit[36]) | (~bit[40]&~bit[38]&~bit[37]&bit[36]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[40]&bit[39]&~bit[38]&bit[37]&bit[36]) | (~bit[40]&~bit[39]&~bit[38]&bit[37]&bit[36]) | (~bit[39]&~bit[38]&~bit[37]&~bit[36]) | (bit[40]&~bit[38]&bit[37]&bit[36]) | (bit[40]&bit[39]&~bit[38]&~bit[37]&bit[36]) | (bit[38]&~bit[37]&bit[36]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[40]&bit[39]&bit[38]&~bit[37]&bit[36]) | (bit[39]&bit[38]&~bit[37]&~bit[36]) | (~bit[40]&~bit[39]&~bit[38]&~bit[37]&bit[36]) | (bit[40]&bit[39]&bit[38]&bit[37]&bit[36]) | (~bit[39]&bit[38]&~bit[37]&~bit[36]) | (~bit[40]&bit[38]&bit[37]&bit[36]) | (bit[40]&bit[39]&~bit[38]&~bit[37]&bit[36]) | (bit[39]&~bit[38]&~bit[37]&~bit[36]) | (~bit[39]&bit[38]&bit[36]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[40]&~bit[39]&~bit[38]&~bit[37]&bit[36]) | (bit[40]&bit[39]&bit[38]&~bit[37]&bit[36]) | (~bit[39]&~bit[38]&bit[37]&~bit[36]) | (~bit[40]&~bit[39]&~bit[38]&bit[37]&bit[36]) | (~bit[40]&bit[38]&bit[37]&bit[36]) | (bit[40]&bit[39]&~bit[38]&~bit[37]&bit[36]) | (bit[39]&~bit[38]&~bit[37]&~bit[36]) | (~bit[39]&bit[38]&bit[36]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[40]&~bit[39]&~bit[38]&bit[37]&bit[36]) | (~bit[40]&~bit[39]&bit[38]&bit[37]&bit[36]) | (~bit[40]&bit[39]&~bit[38]&bit[37]&bit[36]) | (~bit[40]&bit[39]&bit[38]&~bit[37]&bit[36]) | (~bit[40]&~bit[39]&~bit[38]&~bit[37]&bit[36]) | (bit[39]&~bit[38]&bit[37]&~bit[36]) | (bit[40]&bit[39]&bit[38]&bit[37]&bit[36]) | (~bit[39]&bit[38]&~bit[37]&~bit[36]) | (bit[40]&bit[39]&~bit[38]&~bit[37]&bit[36]) | (bit[39]&~bit[38]&~bit[37]&~bit[36]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[40]&bit[39]&bit[38]&~bit[37]&bit[36]) | (bit[39]&bit[38]&~bit[37]&~bit[36]) | (bit[39]&~bit[38]&bit[37]&~bit[36]) | (bit[40]&bit[39]&bit[38]&bit[37]&bit[36]) | (~bit[40]&~bit[39]&~bit[37]&bit[36]) | (~bit[39]&~bit[38]&~bit[37]&~bit[36]) | (~bit[40]&~bit[38]&~bit[37]&bit[36]) | (bit[40]&~bit[38]&bit[37]&bit[36]) | (~bit[39]&bit[38]&bit[37]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[35]);
		control=control & bit[35];

		//-------------36-1--------------

		out[5] = (~disable_update & ((bit[40]&bit[39]&bit[38]&~bit[37]) | (~bit[40]&~bit[38]&bit[37]) | (~bit[41]&~bit[38]&bit[37]) | (~bit[39]&~bit[38]&bit[37]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[41]&bit[40]&bit[39]&~bit[38]&bit[37]) | (bit[40]&~bit[39]&bit[38]&bit[37]) | (~bit[40]&bit[38]&bit[37]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[41]&bit[40]&bit[39]&~bit[38]&bit[37]) | (bit[41]&bit[40]&bit[39]&bit[38]&bit[37]) | (bit[40]&bit[39]&bit[38]&~bit[37]) | (~bit[41]&bit[40]&bit[39]&bit[38]) | (~bit[40]&~bit[39]&bit[38]&bit[37]) | (~bit[39]&~bit[38]&~bit[37]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[41]&bit[40]&bit[39]&~bit[38]&bit[37]) | (~bit[40]&~bit[39]&bit[38]&~bit[37]) | (~bit[41]&bit[40]&bit[39]&bit[38]) | (~bit[41]&bit[39]&bit[38]&bit[37]) | (bit[40]&~bit[39]&bit[38]&bit[37]) | (bit[39]&~bit[38]&~bit[37]) | (~bit[39]&~bit[38]&bit[37]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[41]&bit[40]&bit[39]&bit[38]&bit[37]) | (bit[40]&~bit[39]&bit[38]&~bit[37]) | (~bit[41]&~bit[39]&bit[38]&bit[37]) | (bit[39]&~bit[38]&~bit[37]) | (~bit[40]&~bit[38]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[41]&bit[40]&bit[39]&bit[38]&bit[37]) | (~bit[41]&bit[39]&bit[38]&~bit[37]) | (~bit[41]&~bit[40]&~bit[38]&bit[37]) | (bit[41]&~bit[39]&bit[38]&bit[37]) | (~bit[41]&~bit[39]&~bit[38]&bit[37]) | (~bit[40]&bit[39]&~bit[37]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[36]);
		control=control & bit[36];
		//-------------37--------------

		out[5] = (~disable_update & ((~bit[41]&~bit[40]&~bit[39]&bit[38]) | (bit[40]&bit[39]&~bit[38]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[41]&~bit[40]&bit[39]&bit[38]) | (~bit[41]&bit[40]&~bit[39]&bit[38]) | (bit[41]&~bit[39]&bit[38]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[42]&bit[40]&bit[39]&bit[38]) | (~bit[41]&~bit[40]&~bit[39]&~bit[38]) | (~bit[41]&bit[40]&bit[39]) | (~bit[41]&bit[40]&~bit[39]&bit[38]) | (bit[41]&~bit[40]&bit[38]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[42]&bit[41]&bit[40]&bit[39]) | (~bit[41]&bit[40]&~bit[39]&~bit[38]) | (~bit[42]&bit[40]&~bit[38]) | (~bit[41]&bit[40]&~bit[39]&bit[38]) | (~bit[42]&~bit[40]&bit[38]) | (bit[41]&~bit[39]&~bit[38])| ( bit[41]&~bit[40]&bit[38]) | (bit[41]&~bit[39]&bit[38]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[42]&bit[41]&bit[40]&bit[39]&~bit[38]) | (~bit[42]&~bit[41]&bit[40]&bit[39]&bit[38]) | (bit[42]&~bit[41]&~bit[40]&~bit[39]&bit[38]) | (~bit[42]&bit[41]&~bit[40]&bit[38]) | (bit[42]&bit[41]&bit[40]&bit[39]) | (~bit[41]&~bit[40]&bit[39]&~bit[38]) | (~bit[41]&bit[40]&~bit[39]&~bit[38]) | (bit[41]&~bit[40]&~bit[39]) | (bit[41]&~bit[39]&bit[38]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[42]&~bit[41]&bit[40]&bit[39]&bit[38]) | (~bit[42]&bit[41]&bit[40]&bit[39]&~bit[38]) | (bit[42]&~bit[41]&~bit[40]&~bit[39]&bit[38]) | (~bit[41]&~bit[40]&~bit[39]&~bit[38]) | (~bit[41]&~bit[40]&bit[39]&~bit[38]) | (~bit[42]&~bit[39]&bit[38]) | (bit[41]&~bit[39]&~bit[38]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[37]);
		control=control & bit[37];

		//-------------38--------------

		out[5] = (~disable_update & ((~bit[42]&~bit[41]&~bit[40]&bit[39]) | (bit[43]&~bit[42]&~bit[41]&bit[40]&~bit[39]) | (bit[42]&bit[41]&bit[40]&~bit[39]) | (bit[43]&~bit[42]&bit[41]&bit[40]&~bit[39]) | (~bit[43]&bit[41]&bit[40]&~bit[39]) | (bit[42]&~bit[41]&bit[40]&~bit[39]) | (~bit[43]&~bit[41]&bit[40]&~bit[39]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[43]&bit[42]&bit[41]&~bit[40]&bit[39]) | (bit[43]&~bit[42]&bit[41]&~bit[40]&bit[39]) | (~bit[43]&bit[41]&~bit[40]&bit[39]) | (bit[42]&~bit[41]&~bit[40]&bit[39]) | (~bit[41]&bit[40]&bit[39]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[43]&~bit[42]&bit[41]&~bit[40]&bit[39]) | (bit[43]&~bit[42]&~bit[41]&bit[40]&~bit[39]) | (bit[43]&bit[42]&bit[41]&bit[40]&bit[39]) | (~bit[43]&bit[41]&~bit[40]&bit[39]) | (bit[42]&~bit[41]&~bit[40]&bit[39]) | (~bit[43]&bit[41]&bit[40]&bit[39]) | (~bit[43]&~bit[42]&bit[41]&bit[40]) | (bit[42]&~bit[41]&bit[40]&~bit[39]) | (~bit[43]&~bit[41]&bit[40]&~bit[39]) | (~bit[42]&bit[41]&bit[40]&bit[39]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[43]&bit[42]&bit[41]&~bit[40]&bit[39]) | (bit[42]&bit[41]&bit[40]&~bit[39]) | (bit[43]&~bit[42]&bit[41]&bit[40]&~bit[39]) | (bit[42]&~bit[41]&~bit[40]&bit[39]) | (~bit[43]&bit[41]&bit[40]&bit[39]) | (~bit[42]&~bit[41]&bit[40]) | (~bit[42]&bit[41]&bit[40]&bit[39]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[43]&bit[42]&bit[41]&~bit[40]&bit[39]) | (~bit[43]&~bit[42]&~bit[40]&bit[39]) | (bit[43]&bit[42]&bit[41]&bit[40]&bit[39]) | (bit[43]&~bit[42]&bit[41]&bit[40]&~bit[39]) | (~bit[41]&~bit[40]&~bit[39]) | (~bit[43]&bit[42]&~bit[41]&bit[40]) | (bit[42]&~bit[41]&bit[40]&~bit[39]) | (~bit[43]&~bit[41]&bit[40]&~bit[39]) | (~bit[42]&bit[41]&bit[40]&bit[39]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[43]&bit[42]&~bit[41]&bit[39]) | (~bit[43]&~bit[41]&bit[40]&bit[39]) | (bit[43]&~bit[42]&bit[41]&~bit[40]&bit[39]) | (bit[43]&~bit[42]&~bit[41]&bit[40]&~bit[39]) | (bit[43]&bit[42]&bit[41]&bit[40]&bit[39]) | (bit[43]&~bit[42]&bit[41]&bit[40]&~bit[39]) | (~bit[43]&bit[41]&bit[40]&~bit[39]) | (~bit[42]&~bit[40]&~bit[39]) | (~bit[43]&bit[42]&~bit[41]&bit[40]) | (~bit[43]&~bit[42]&bit[41]&bit[40]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[38]);
		control=control & bit[38];

		//-------------39--------------

		out[5] = (~disable_update & ((~bit[43]&~bit[41]&bit[40]) | (~bit[42]&~bit[41]&bit[40]) | (bit[42]&bit[41]&~bit[40]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[44]&~bit[42]&bit[41]&bit[40]) | (~bit[44]&~bit[42]&bit[41]&bit[40]) | (bit[43]&bit[42]&~bit[41]&bit[40]) | (~bit[43]&bit[41]&bit[40]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[44]&~bit[43]&~bit[42]&bit[40]) | (~bit[43]&~bit[42]&bit[41]&bit[40]) | (~bit[42]&~bit[41]&~bit[40]) | (bit[42]&bit[41]&~bit[40]) | (bit[43]&bit[42]&bit[40]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[44]&~bit[43]&~bit[42]&~bit[41]&bit[40]) | (bit[43]&bit[42]&~bit[41]&~bit[40]) | (~bit[44]&~bit[42]&bit[41]&bit[40]) | (~bit[44]&bit[43]&bit[40]) | (~bit[43]&bit[42]&~bit[40]) | (bit[43]&bit[42]&bit[40]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[44]&~bit[43]&~bit[42]&~bit[41]&bit[40]) | (bit[44]&bit[43]&~bit[42]&bit[40]) | (~bit[43]&bit[42]&~bit[41]&~bit[40]) | (~bit[43]&~bit[42]&bit[41]&~bit[40]) | (bit[43]&bit[42]&~bit[41]&bit[40]) | (~bit[44]&bit[42]&bit[41]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[44]&bit[42]&bit[41]&~bit[40]) | (~bit[43]&~bit[42]&~bit[41]&~bit[40]) | (~bit[44]&bit[42]&~bit[41]&bit[40]) | (bit[43]&bit[42]&~bit[41]&~bit[40]) | (bit[44]&~bit[42]&bit[41]&bit[40]) | (bit[43]&bit[42]&bit[41]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[39]);
		control=control & bit[39];

		//-------------40--------------

		out[5] = (~disable_update & ((~bit[44]&bit[43]&~bit[42]&bit[41]) | (~bit[45]&bit[44]&~bit[42]&bit[41]) | (bit[44]&~bit[43]&~bit[42]&bit[41]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[45]&~bit[44]&bit[42]&bit[41]) | (bit[45]&bit[44]&bit[43]&~bit[42]&bit[41]) | (~bit[43]&bit[42]&bit[41]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[45]&~bit[43]&bit[42]&bit[41]) | (~bit[45]&bit[44]&bit[43]&bit[42]&bit[41]) | (bit[45]&bit[43]&bit[42]&bit[41]) | (bit[45]&bit[44]&bit[43]&~bit[42]&bit[41]) | (~bit[44]&~bit[43]&bit[42]&bit[41]) | (bit[44]&~bit[43]&~bit[42]&~bit[41]) | (~bit[44]&~bit[42]&~bit[41]) | (bit[44]&~bit[43]&~bit[42]&bit[41]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[45]&~bit[44]&bit[43]&bit[42]&bit[41]) | (~bit[45]&~bit[44]&bit[43]&~bit[42]&bit[41]) | (~bit[44]&~bit[43]&~bit[41]) | (~bit[45]&bit[44]&bit[43]&bit[42]&bit[41]) | (bit[44]&bit[43]&~bit[42]&~bit[41]) | (bit[45]&bit[44]&bit[43]&~bit[42]&bit[41]) | (~bit[43]&bit[42]&~bit[41]) | (bit[44]&~bit[43]&~bit[42]&bit[41]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[45]&bit[44]&~bit[43]&~bit[42]) | (bit[45]&bit[43]&bit[42]&bit[41]) | (~bit[44]&bit[43]&~bit[42]&bit[41]) | (~bit[44]&~bit[43]&bit[42]&bit[41]) | (bit[44]&~bit[43]&~bit[42]&~bit[41]) | (bit[43]&~bit[41]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[45]&bit[44]&bit[42]&bit[41]) | (~bit[45]&~bit[44]&~bit[43]&bit[41]) | (~bit[45]&bit[44]&bit[43]&bit[42]&bit[41]) | (bit[44]&bit[43]&~bit[42]&~bit[41]) | (bit[45]&bit[44]&bit[43]&~bit[42]&bit[41]) | (~bit[45]&bit[44]&~bit[42]&bit[41]) | (bit[44]&~bit[43]&~bit[42]&~bit[41]) | (~bit[44]&bit[42]&~bit[41]) | (~bit[44]&~bit[42]&bit[41]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[40]);
		control=control & bit[40];

		//-------------41--------------

		out[5] = (~disable_update & ((~bit[46]&bit[45]&~bit[44]&bit[43]&~bit[42]) | (bit[46]&~bit[45]&bit[44]&bit[43]&~bit[42]) | (bit[46]&bit[45]&~bit[44]&~bit[43]&bit[42]) | (bit[46]&bit[45]&bit[43]&~bit[42]) | (~bit[46]&~bit[44]&~bit[43]&bit[42]) | (~bit[46]&bit[44]&bit[43]&~bit[42]) | (~bit[45]&~bit[43]&bit[42]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[46]&bit[45]&~bit[44]&bit[43]&~bit[42]) | (bit[45]&bit[44]&~bit[43]&bit[42]) | (bit[43]&bit[42]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[46]&~bit[45]&~bit[44]&bit[42]) | (bit[46]&~bit[45]&bit[44]&bit[43]&~bit[42]) | (~bit[45]&~bit[44]&~bit[43]&~bit[42]) | (~bit[46]&~bit[44]&bit[43]&bit[42]) | (bit[46]&bit[45]&bit[43]&~bit[42]) | (~bit[45]&~bit[44]&bit[43]&bit[42]) | (~bit[46]&bit[44]&bit[43]&~bit[42]) | (bit[45]&bit[44]&~bit[43]&bit[42]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[46]&~bit[45]&~bit[44]&~bit[43]&bit[42]) | (bit[45]&~bit[44]&~bit[43]&~bit[42]) | (~bit[46]&bit[45]&~bit[43]&bit[42]) | (~bit[45]&bit[44]&bit[43]) | (bit[46]&bit[45]&~bit[44]&bit[43]) | (bit[45]&bit[44]&~bit[43]&bit[42]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[46]&~bit[45]&~bit[44]&bit[43]&bit[42]) | (bit[46]&~bit[45]&~bit[44]&~bit[43]&bit[42]) | (~bit[45]&bit[44]&~bit[43]&~bit[42]) | (~bit[46]&~bit[45]&bit[44]&~bit[43]) | (bit[46]&bit[45]&~bit[44]&~bit[43]&bit[42]) | (~bit[46]&bit[45]&bit[44]&bit[42]) | (bit[45]&~bit[44]&~bit[43]&~bit[42]) | (bit[46]&bit[45]&bit[43]&~bit[42]) | (bit[46]&bit[45]&~bit[44]&bit[43]) | (~bit[46]&bit[44]&bit[43]&~bit[42]) | (bit[45]&bit[44]&~bit[43]&bit[42]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[46]&bit[45]&bit[44]&~bit[42]) | (~bit[46]&~bit[45]&bit[43]&bit[42]) | (bit[46]&~bit[45]&bit[44]&bit[43]&~bit[42]) | (bit[46]&bit[45]&bit[43]&bit[42]) | (bit[46]&bit[45]&~bit[44]&~bit[43]&bit[42]) | (~bit[46]&bit[45]&~bit[43]&bit[42]) | (~bit[46]&~bit[44]&~bit[43]&bit[42]) | (bit[44]&~bit[43]&~bit[42]) | (~bit[45]&~bit[44]&bit[43]&bit[42]) | (bit[46]&bit[45]&~bit[44]&bit[43]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[41]);
		control=control & bit[41];

		//-------------42--------------
		out[5] = (~disable_update & ((bit[46]&bit[45]&~bit[44]&bit[43]) | (~bit[46]&bit[45]&~bit[44]&bit[43]) | (~bit[45]&bit[44]&bit[43]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[46]&bit[45]&bit[44]&bit[43]) | (~bit[46]&~bit[45]&~bit[44]&~bit[43]) | (~bit[47]&bit[45]&bit[44]&bit[43]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[46]&~bit[45]&bit[44]&~bit[43]) | (~bit[47]&~bit[46]&bit[44]&bit[43]) | (bit[47]&bit[46]&bit[45]&bit[44]&bit[43]) | (bit[46]&~bit[45]&~bit[44]&~bit[43]) | (bit[45]&~bit[44]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[47]&bit[46]&bit[45]&bit[44]&bit[43]) | (bit[46]&~bit[45]&~bit[44]&~bit[43]) | (~bit[46]&bit[45]&~bit[44]&bit[43]) | (bit[46]&~bit[45]&bit[44]) | (~bit[46]&bit[45]&~bit[43]) | (bit[47]&~bit[46]&bit[44]&bit[43]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[47]&bit[46]&bit[45]&bit[44]&bit[43]) | (~bit[47]&bit[45]&~bit[44]&bit[43]) | (~bit[47]&bit[46]&bit[44]) | (bit[47]&~bit[46]&bit[44]&bit[43]) | (~bit[46]&~bit[44]&bit[43]) | (bit[46]&~bit[43]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[46]&bit[45]&bit[44]&~bit[43]) | (bit[46]&bit[45]&~bit[44]&bit[43]) | (bit[46]&~bit[45]&~bit[44]&~bit[43]) | (~bit[47]&bit[45]&~bit[44]&bit[43]) | (bit[47]&~bit[46]&bit[44]&bit[43]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[42]);
		control=control & bit[42];

		//-------------43--------------

		out[5] = (~disable_update & ((~bit[47]&~bit[46]&bit[44]) | (~bit[45]&bit[44]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[48]&~bit[47]&~bit[46]&~bit[45]&bit[44]) | (bit[46]&bit[45]&bit[44]) | (bit[47]&~bit[46]&bit[45]&bit[44]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[47]&~bit[46]&~bit[45]&~bit[44]) | (bit[48]&~bit[47]&~bit[46]&~bit[45]&bit[44]) | (bit[47]&~bit[46]&~bit[45]&bit[44]) | (bit[47]&~bit[46]&bit[45]&bit[44]) | (~bit[47]&bit[46]&~bit[45]) | (bit[47]&~bit[46]&~bit[45]&~bit[44]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[48]&~bit[47]&bit[46]&bit[45]&bit[44]) | (~bit[47]&~bit[46]&~bit[45]&~bit[44]) | (bit[47]&bit[46]&~bit[45]&~bit[44]) | (~bit[46]&bit[45]&~bit[44]) | (bit[48]&~bit[47]&~bit[46]&~bit[45]&bit[44]) | (bit[47]&~bit[46]&~bit[45]&bit[44]) | (~bit[48]&bit[47]&~bit[45]&bit[44]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[48]&bit[46]&~bit[45]&bit[44]) | (bit[48]&~bit[47]&bit[46]&bit[45]) | (~bit[48]&bit[47]&bit[45]&bit[44]) | (bit[46]&bit[45]&~bit[44]) | (bit[48]&~bit[47]&~bit[46]&~bit[45]&bit[44]) | (bit[47]&bit[46]&~bit[45]) | (bit[47]&~bit[46]&~bit[45]&~bit[44]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[48]&~bit[47]&bit[46]&bit[45]) | (~bit[48]&~bit[47]&bit[46]&bit[45]&bit[44]) | (bit[47]&bit[46]&~bit[45]&~bit[44]) | (bit[48]&~bit[47]&~bit[46]&~bit[45]&bit[44]) | (~bit[48]&bit[47]&~bit[45]&bit[44]) | (bit[47]&~bit[46]&bit[45]&bit[44]) | (bit[47]&~bit[46]&~bit[45]&~bit[44]) | (~bit[48]&~bit[46]&bit[44]) | (~bit[47]&~bit[44]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[43]);
		control=control & bit[43];

		//-------------44--------------

		out[5] = (~disable_update & ((~bit[49]&bit[48]&bit[47]&bit[46]&~bit[45]) | (bit[49]&~bit[48]&~bit[46]&bit[45]) | (~bit[49]&~bit[48]&~bit[46]&bit[45]) | (bit[49]&bit[48]&bit[47]&bit[46]&~bit[45]) | (~bit[47]&~bit[46]&bit[45]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[49]&bit[48]&bit[47]&bit[46]&~bit[45]) | (~bit[49]&bit[48]&~bit[47]&bit[46]&bit[45]) | (~bit[48]&bit[46]&bit[45]) | (bit[49]&~bit[47]&bit[46]&bit[45]) | (bit[48]&bit[47]&~bit[46]&bit[45]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[49]&~bit[48]&~bit[47]&bit[45]) | (~bit[48]&~bit[47]&bit[46]&~bit[45]) | (bit[49]&bit[48]&bit[47]&bit[46]&~bit[45]) | (bit[48]&bit[47]&~bit[46]&~bit[45]) | (bit[48]&bit[47]&~bit[46]&bit[45]) | (bit[48]&bit[47]&bit[46]&bit[45]) | (bit[48]&~bit[47]&~bit[46]&~bit[45]) | (~bit[48]&~bit[46]&~bit[45]) | (~bit[47]&~bit[46]&bit[45]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[49]&~bit[48]&~bit[47]&bit[46]&bit[45]) | (~bit[49]&bit[47]&~bit[46]&bit[45]) | (~bit[49]&bit[48]&~bit[47]&bit[46]&bit[45]) | (~bit[49]&~bit[48]&~bit[46]&bit[45]) | (bit[48]&~bit[47]&bit[46]&~bit[45]) | (bit[49]&bit[48]&bit[47]&bit[46]&~bit[45]) | (bit[48]&bit[47]&bit[46]&bit[45]) | (bit[48]&~bit[47]&~bit[46]&~bit[45]) | (~bit[48]&~bit[46]&~bit[45]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[48]&~bit[47]&~bit[46]&~bit[45]) | (bit[49]&~bit[48]&~bit[46]&bit[45]) | (~bit[48]&bit[47]&bit[46]&~bit[45]) | (bit[48]&~bit[47]&bit[46]&~bit[45]) | (bit[49]&bit[48]&bit[47]&bit[46]&~bit[45]) | (bit[49]&~bit[47]&bit[46]&bit[45]) | (bit[48]&bit[47]&~bit[46]&~bit[45]) | (bit[48]&bit[47]&bit[46]&bit[45]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[49]&~bit[48]&~bit[47]&bit[45]) | (~bit[49]&bit[48]&~bit[46]) | (~bit[48]&bit[47]&bit[46]&~bit[45]) | (~bit[48]&~bit[47]&bit[46]&~bit[45]) | (bit[49]&bit[48]&bit[47]&bit[46]&~bit[45]) | (~bit[49]&bit[47]&bit[45]) | (bit[49]&~bit[47]&bit[46]&bit[45]) | (bit[48]&bit[47]&~bit[46]&~bit[45]) | (bit[48]&bit[47]&~bit[46]&bit[45]) | (bit[48]&~bit[47]&~bit[46]&~bit[45]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[44]);
		control=control & bit[44];

		//-------------45--------------

		out[5] = (~disable_update & ((bit[49]&~bit[48]&bit[47]&~bit[46]) | (~bit[48]&~bit[47]&bit[46]) | (bit[48]&bit[47]&~bit[46]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[49]&~bit[48]&bit[47]&~bit[46]) | (~bit[48]&bit[47]&bit[46]) | (bit[48]&~bit[47]&bit[46]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[50]&bit[49]&bit[48]&bit[47]) | (~bit[49]&~bit[48]&~bit[47]&~bit[46]) | (bit[49]&~bit[48]&~bit[47]&~bit[46]) | (~bit[49]&bit[48]&~bit[47]&~bit[46]) | (bit[48]&bit[47]&~bit[46]) | (bit[48]&~bit[47]&bit[46]) | (~bit[50]&bit[48]&bit[47]) | (~bit[49]&bit[48]&bit[46]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[49]&~bit[48]&~bit[47]&~bit[46]) | (~bit[50]&~bit[49]&~bit[48]&bit[46]) | (bit[49]&bit[48]&~bit[47]&~bit[46]) | (~bit[49]&bit[47]&bit[46]) | (~bit[50]&bit[48]&bit[47]) | (~bit[49]&bit[48]&bit[47]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[50]&bit[49]&bit[47]&bit[46]) | (~bit[50]&bit[49]&~bit[48]&bit[47]) | (bit[49]&~bit[48]&~bit[47]&~bit[46]) | (~bit[50]&~bit[49]&~bit[48]&bit[46]) | (~bit[49]&~bit[48]&~bit[47]&bit[46]) | (bit[49]&bit[48]&~bit[47]&~bit[46]) | (~bit[49]&bit[48]&bit[47]) | (~bit[49]&bit[48]&bit[46]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[50]&bit[49]&bit[48]&bit[47]) | (bit[50]&bit[49]&bit[47]&~bit[46]) | (~bit[50]&~bit[49]&bit[48]) | (~bit[50]&~bit[48]&bit[46]) | (~bit[49]&bit[48]&~bit[47]&~bit[46]) | (~bit[49]&~bit[48]&~bit[47]&bit[46]) | (bit[49]&bit[48]&~bit[47]&~bit[46]) | (~bit[50]&~bit[47]&bit[46]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[45]);
		control=control & bit[45];

		//-------------46--------------

		out[5] = (~disable_update & ((~bit[51]&~bit[50]&~bit[49]&bit[48]&bit[47]) | (~bit[51]&~bit[50]&bit[49]&~bit[48]&bit[47]) | (bit[51]&bit[49]&~bit[48]&bit[47]) | (~bit[51]&bit[50]&~bit[48]&bit[47]) | (bit[50]&~bit[49]&~bit[48]&bit[47]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[51]&bit[50]&~bit[49]&bit[48]&bit[47]) | (bit[51]&~bit[49]&bit[48]&bit[47]) | (bit[49]&bit[48]&bit[47]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[51]&~bit[50]&bit[49]&~bit[48]&bit[47]) | (~bit[50]&~bit[49]&~bit[48]&~bit[47]) | (~bit[51]&bit[50]&~bit[49]&bit[48]&bit[47]) | (bit[50]&~bit[49]&~bit[48]) | (bit[51]&~bit[49]&bit[48]&bit[47]) | (bit[49]&bit[48]&bit[47]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[51]&~bit[50]&bit[49]&~bit[48]) | (~bit[51]&~bit[50]&bit[49]&bit[48]&bit[47]) | (~bit[50]&~bit[49]&bit[48]&~bit[47]) | (~bit[51]&bit[50]&~bit[49]&bit[48]&bit[47]) | (bit[49]&~bit[48]&~bit[47]) | (~bit[51]&bit[50]&~bit[48]&bit[47]) | (bit[50]&~bit[49]&~bit[48]&bit[47]) | (bit[51]&~bit[49]&bit[48]&bit[47]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[50]&~bit[49]&bit[48]&~bit[47]) | (~bit[50]&~bit[49]&~bit[48]&~bit[47]) | (bit[51]&~bit[50]&bit[48]&bit[47]) | (~bit[51]&bit[50]&~bit[49]&bit[48]&bit[47]) | (~bit[50]&bit[49]&~bit[47]) | (bit[51]&bit[49]&~bit[48]&bit[47]) | (bit[50]&~bit[49]&~bit[48]&bit[47]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[51]&bit[50]&bit[49]) | (bit[51]&~bit[50]&bit[48]&bit[47]) | (bit[51]&bit[49]&~bit[48]&bit[47]) | (~bit[51]&bit[50]&~bit[48]&bit[47]) | (bit[51]&~bit[49]&bit[48]&bit[47]) | (bit[50]&~bit[47]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[46]);
		control=control & bit[46];

		//-------------47--------------

		out[5] = (~disable_update & ((~bit[52]&~bit[51]&bit[50]&bit[49]&bit[48]) | (bit[52]&bit[50]&bit[49]&bit[48]) | (~bit[51]&~bit[50]&bit[49]&bit[48]) | (bit[51]&bit[49]&bit[48]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[51]&~bit[50]&bit[49]&~bit[48]) | (~bit[50]&~bit[49]&~bit[48]) | (bit[50]&~bit[49]&~bit[48]) | (~bit[51]&~bit[50]&bit[49]&bit[48]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[52]&~bit[51]&bit[50]&bit[49]&bit[48]) | (~bit[52]&~bit[51]&bit[50]&bit[49]&bit[48]) | (~bit[51]&~bit[50]&~bit[49]&bit[48]) | (~bit[51]&bit[50]&bit[49]&~bit[48]) | (bit[51]&~bit[50]&bit[49]) | (bit[51]&bit[49]&~bit[48]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[52]&~bit[51]&bit[50]&bit[49]&bit[48]) | (~bit[51]&bit[50]&bit[49]&~bit[48]) | (~bit[50]&~bit[49]&~bit[48]) | (bit[51]&bit[49]&bit[48]) | (bit[51]&~bit[50]&~bit[48]) | (bit[51]&~bit[50]&bit[48]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[52]&~bit[51]&bit[50]&bit[49]&bit[48]) | (~bit[52]&bit[51]&bit[49]) | (~bit[51]&bit[50]&~bit[49]) | (~bit[51]&~bit[49]&~bit[48]) | (bit[50]&~bit[49]&~bit[48]) | (~bit[51]&~bit[50]&bit[49]&bit[48]) | (bit[51]&~bit[50]&bit[48]) | (bit[51]&bit[49]&~bit[48]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[52]&bit[50]&bit[49]&bit[48]) | (~bit[51]&~bit[50]&~bit[49]&bit[48]) | (~bit[51]&~bit[50]&bit[49]&~bit[48]) | (~bit[52]&~bit[50]&bit[49]) | (~bit[51]&bit[50]&~bit[48]) | (bit[51]&~bit[50]&~bit[48]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[47]);
		control=control & bit[47];

		//-------------48--------------

		out[5] = (~disable_update & ((~bit[53]&bit[52]&bit[51]&~bit[50]&bit[49]) | (~bit[53]&~bit[52]&~bit[51]&bit[50]&bit[49]) | (~bit[53]&~bit[52]&bit[51]&bit[50]&bit[49]) | (~bit[53]&bit[52]&~bit[51]&bit[50]&bit[49]) | (bit[53]&~bit[52]&bit[50]&bit[49]) | (bit[53]&bit[52]&bit[51]&~bit[50]&bit[49]) | (~bit[52]&~bit[51]&~bit[50]&~bit[49]) | (bit[53]&~bit[51]&bit[50]&bit[49]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[53]&bit[52]&bit[51]&~bit[50]&bit[49]) | (~bit[53]&bit[52]&bit[51]&bit[50]&bit[49]) | (bit[52]&~bit[51]&~bit[50]&~bit[49]) | (bit[53]&bit[52]&bit[51]&bit[50]&bit[49]) | (bit[51]&~bit[50]&~bit[49]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[53]&~bit[52]&~bit[51]&bit[50]&bit[49]) | (~bit[53]&bit[52]&bit[51]&bit[50]&bit[49]) | (~bit[52]&bit[50]&~bit[49]) | (bit[53]&bit[52]&bit[51]&~bit[50]&bit[49]) | (bit[52]&~bit[51]&~bit[49]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[52]&~bit[51]&bit[50]&~bit[49]) | (~bit[52]&~bit[51]&~bit[50]&bit[49]) | (bit[52]&~bit[51]&~bit[50]&~bit[49]) | (~bit[53]&bit[52]&~bit[51]&bit[50]&bit[49]) | (~bit[52]&bit[51]&~bit[50]&~bit[49]) | (bit[53]&bit[52]&bit[51]&~bit[50]&bit[49]) | (bit[53]&bit[52]&bit[51]&bit[50]&bit[49]) | (bit[53]&~bit[51]&bit[50]&bit[49]) | (bit[52]&bit[51]&bit[50]&~bit[49]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[53]&bit[52]&~bit[51]&bit[50]&bit[49]) | (bit[53]&~bit[52]&bit[50]&bit[49]) | (~bit[52]&bit[51]&~bit[50]&~bit[49]) | (bit[53]&bit[52]&bit[51]&~bit[50]&bit[49]) | (bit[52]&~bit[51]&~bit[50]&bit[49]) | (bit[53]&bit[52]&bit[51]&bit[50]&bit[49]) | (~bit[52]&bit[51]&bit[49]) | (~bit[52]&~bit[51]&~bit[50]&~bit[49]) | (bit[52]&bit[51]&bit[50]&~bit[49]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[53]&~bit[52]&bit[51]&bit[50]&bit[49]) | (~bit[53]&bit[52]&bit[51]&bit[50]&bit[49]) | (bit[52]&~bit[51]&~bit[50]&bit[49]) | (bit[53]&bit[52]&bit[51]&bit[50]&bit[49]) | (bit[51]&~bit[50]&~bit[49]) | (bit[52]&~bit[51]&~bit[49]) | (~bit[52]&~bit[51]&~bit[50]&~bit[49]) | (bit[53]&~bit[51]&bit[50]&bit[49]) | (bit[52]&bit[51]&bit[50]&~bit[49]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[48]);
		control=control & bit[48];

		//-------------49--------------

		out[5] = (~disable_update & ((~bit[54]&bit[53]&bit[52]&bit[51]&~bit[50]) | (bit[54]&bit[53]&bit[52]&bit[51]&~bit[50]) | (bit[54]&~bit[53]&~bit[52]&~bit[51]&bit[50]) | (bit[54]&~bit[53]&bit[52]&~bit[51]&bit[50]) | (~bit[54]&~bit[52]&~bit[51]&bit[50]) | (~bit[54]&bit[52]&~bit[51]&bit[50]) | (bit[54]&bit[53]&~bit[52]&~bit[51]&bit[50]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[54]&bit[53]&bit[52]&bit[51]&~bit[50]) | (bit[54]&bit[53]&~bit[52]&bit[51]&bit[50]) | (bit[54]&bit[53]&bit[52]&bit[51]&~bit[50]) | (bit[54]&~bit[53]&~bit[52]&~bit[51]&bit[50]) | (~bit[52]&~bit[51]&~bit[50]) | (~bit[54]&~bit[52]&~bit[51]&bit[50]) | (~bit[54]&~bit[52]&bit[51]&bit[50]) | (~bit[54]&~bit[53]&bit[52]&bit[51]&bit[50]) | (bit[54]&bit[53]&bit[52]&~bit[51]&bit[50]) | (bit[54]&~bit[53]&~bit[52]&bit[51]&bit[50]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[54]&~bit[53]&bit[52]&~bit[51]) | (~bit[54]&bit[53]&bit[52]&bit[51]&bit[50]) | (bit[52]&~bit[51]&~bit[50]) | (bit[54]&bit[52]&bit[51]&bit[50]) | (~bit[54]&~bit[52]&bit[51]&bit[50]) | (bit[54]&bit[53]&bit[52]&~bit[51]&bit[50]) | (bit[54]&~bit[53]&~bit[52]&bit[51]&bit[50]) | (bit[54]&bit[53]&~bit[52]&~bit[51]&bit[50]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[54]&~bit[53]&~bit[52]&bit[51]) | (~bit[54]&bit[53]&bit[52]&bit[51]&~bit[50]) | (bit[54]&~bit[53]&bit[52]&bit[51]&bit[50]) | (bit[54]&bit[53]&~bit[52]&bit[51]&bit[50]) | (~bit[53]&~bit[52]&~bit[50]) | (bit[54]&bit[53]&bit[52]&~bit[51]&bit[50]) | (bit[54]&bit[53]&~bit[52]&~bit[51]&bit[50]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[54]&~bit[53]&~bit[51]&bit[50]) | (bit[53]&~bit[52]&bit[51]&~bit[50]) | (~bit[54]&bit[53]&bit[52]&bit[51]&bit[50]) | (bit[54]&~bit[53]&bit[52]&bit[51]&bit[50]) | (~bit[53]&~bit[52]&~bit[51]&~bit[50]) | (bit[54]&bit[53]&bit[52]&bit[51]&~bit[50]) | (bit[54]&~bit[53]&bit[52]&~bit[51]&bit[50]) | (~bit[54]&~bit[53]&bit[52]&bit[51]&bit[50]) | (~bit[54]&bit[52]&~bit[51]&bit[50]) | (bit[54]&bit[53]&bit[52]&~bit[51]&bit[50]) | (bit[54]&~bit[53]&~bit[52]&bit[51]&bit[50]) | (bit[54]&bit[53]&~bit[52]&~bit[51]&bit[50]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[53]&bit[52]&~bit[51]&~bit[50]) | (bit[54]&bit[53]&~bit[52]&bit[51]&bit[50]) | (bit[54]&bit[53]&bit[52]&bit[51]&~bit[50]) | (bit[54]&~bit[53]&~bit[52]&~bit[51]&bit[50]) | (bit[54]&~bit[53]&bit[52]&~bit[51]&bit[50]) | (bit[54]&bit[52]&bit[51]&bit[50]) | (~bit[54]&~bit[53]&bit[52]&bit[51]&bit[50]) | (bit[54]&~bit[53]&~bit[52]&bit[51]&bit[50]) | (bit[54]&bit[53]&~bit[52]&~bit[51]&bit[50]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[49]);
		control=control & bit[49];

		//-------------50--------------

		out[5] = (~disable_update & ((~bit[55]&~bit[53]&bit[52]&~bit[51]) | (~bit[54]&bit[53]&bit[52]&~bit[51]) | (~bit[54]&~bit[53]&bit[52]&~bit[51]) | (bit[54]&bit[53]&bit[52]&~bit[51]) | (bit[55]&bit[54]&~bit[53]&bit[52]&~bit[51]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[55]&~bit[53]&~bit[52]&bit[51]) | (~bit[54]&bit[53]&~bit[52]&bit[51]) | (~bit[55]&~bit[53]&bit[52]&~bit[51]) | (~bit[55]&bit[54]&~bit[52]&bit[51]) | (~bit[54]&~bit[53]&bit[52]&~bit[51]) | (~bit[55]&~bit[53]&~bit[52]&bit[51]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[55]&bit[54]&bit[53]&~bit[52]&bit[51]) | (~bit[54]&bit[53]&bit[52]&~bit[51]) | (bit[55]&bit[54]&~bit[53]&bit[52]&~bit[51]) | (~bit[54]&~bit[53]&~bit[52]) | (~bit[55]&~bit[53]&~bit[52]&bit[51]) | (~bit[53]&bit[52]&bit[51]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[55]&bit[54]&bit[53]&~bit[52]&bit[51]) | (~bit[54]&bit[53]&~bit[52]&~bit[51]) | (~bit[55]&bit[54]&bit[53]&bit[52]) | (bit[55]&~bit[53]&~bit[52]&bit[51]) | (bit[54]&~bit[53]&~bit[52]&~bit[51]) | (bit[55]&bit[54]&~bit[53]&bit[52]&~bit[51]) | (~bit[55]&~bit[54]&bit[51]) | (bit[53]&bit[52]&bit[51]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[55]&~bit[53]&bit[52]&bit[51]) | (bit[55]&bit[54]&~bit[52]&bit[51]) | (~bit[54]&bit[53]&~bit[52]&bit[51]) | (bit[55]&bit[54]&~bit[53]&bit[52]&~bit[51]) | (bit[54]&bit[53]&~bit[51]) | (~bit[54]&bit[52]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[55]&~bit[54]&~bit[53]&bit[52]&bit[51]) | (~bit[55]&~bit[54]&bit[52]&~bit[51]) | (bit[55]&bit[54]&~bit[52]&bit[51]) | (~bit[55]&bit[54]&~bit[52]&bit[51]) | (bit[54]&bit[53]&bit[52]&~bit[51]) | (bit[54]&~bit[53]&~bit[52]&~bit[51]) | (bit[55]&bit[54]&~bit[53]&bit[52]&~bit[51]) | (~bit[55]&~bit[53]&~bit[52]&bit[51]) | (~bit[55]&bit[53]&bit[52]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[50]);
		control=control & bit[50];

		//-------------51--------------


		out[5] = (~disable_update & ((bit[56]&~bit[55]&~bit[54]&~bit[53]&bit[52]) | (bit[55]&bit[54]&~bit[52]) | (~bit[56]&~bit[54]&~bit[53]&bit[52]) | (bit[53]&~bit[52]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[56]&~bit[54]&bit[53]&bit[52]) | (bit[56]&bit[55]&~bit[53]&bit[52]) | (~bit[55]&bit[54]&~bit[53]&bit[52]) | (bit[55]&bit[54]&~bit[53]) | (~bit[55]&~bit[54]&bit[53]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[55]&bit[54]&~bit[53]&bit[52]) | (bit[56]&bit[55]&~bit[54]&bit[52]) | (bit[55]&~bit[54]&bit[53]&~bit[52]) | (~bit[55]&bit[54]&bit[53]) | (bit[54]&bit[53]&bit[52]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[56]&~bit[55]&~bit[54]&~bit[53]&bit[52]) | (~bit[56]&~bit[55]&bit[54]&bit[53]&bit[52]) | (bit[55]&bit[54]&bit[53]&~bit[52]) | (bit[56]&bit[55]&~bit[54]&bit[52]) | (bit[55]&~bit[54]&bit[53]&~bit[52]) | (bit[55]&bit[54]&~bit[53]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[56]&~bit[55]&~bit[54]&~bit[53]&bit[52]) | (bit[56]&~bit[55]&bit[53]&bit[52]) | (~bit[56]&bit[55]&bit[54]&bit[53]) | (bit[55]&bit[54]&bit[53]&~bit[52]) | (bit[56]&bit[55]&~bit[54]&bit[52]) | (~bit[56]&bit[54]&~bit[53]&bit[52]) | (~bit[54]&~bit[53]&~bit[52]) | (~bit[56]&bit[53]&~bit[52]) | (~bit[55]&~bit[54]&bit[53]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[56]&bit[55]&bit[54]&~bit[52]) | (bit[56]&bit[55]&bit[54]&bit[52]) | (bit[56]&~bit[55]&bit[54]&bit[53]) | (bit[56]&bit[55]&~bit[53]&bit[52]) | (bit[55]&~bit[54]&bit[53]&~bit[52]) | (~bit[56]&bit[54]&~bit[53]&bit[52]) | (~bit[56]&~bit[55]&~bit[54]) | (~bit[55]&~bit[54]&~bit[53]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[51]);
		control=control & bit[51];

		//-------------52--------------

		out[5] = (~disable_update & ((~bit[57]&~bit[56]&bit[55]&bit[54]&~bit[53]) | (~bit[57]&~bit[56]&bit[55]&~bit[54]&bit[53]) | (bit[57]&bit[56]&~bit[55]&bit[54]&~bit[53]) | (~bit[57]&bit[56]&~bit[55]&bit[54]&~bit[53]) | (bit[57]&~bit[56]&bit[55]&bit[54]&~bit[53]) | (~bit[55]&~bit[54]&bit[53]) | (bit[56]&bit[55]&bit[54]&~bit[53]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[57]&~bit[56]&bit[55]&bit[54]&~bit[53]) | (bit[57]&bit[56]&~bit[55]&bit[54]&~bit[53]) | (~bit[57]&bit[56]&~bit[55]&bit[54]&~bit[53]) | (~bit[57]&bit[56]&bit[55]&~bit[54]&bit[53]) | (bit[57]&bit[55]&~bit[54]&bit[53]) | (~bit[55]&bit[54]&bit[53]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[56]&~bit[55]&~bit[54]&~bit[53]) | (~bit[57]&~bit[56]&~bit[55]&bit[53]) | (~bit[57]&bit[56]&bit[55]&~bit[54]&bit[53]) | (bit[57]&bit[55]&~bit[54]&bit[53]) | (bit[57]&~bit[56]&bit[55]&bit[54]&~bit[53]) | (~bit[57]&bit[55]&bit[54]&bit[53]) | (bit[56]&bit[55]&bit[54]&~bit[53]) | (~bit[56]&bit[55]&bit[54]&bit[53]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[57]&bit[56]&~bit[55]&~bit[54]&bit[53]) | (~bit[57]&bit[56]&~bit[55]&bit[54]&~bit[53]) | (bit[57]&bit[56]&bit[55]&bit[54]&bit[53]) | (bit[56]&~bit[55]&~bit[54]&~bit[53]) | (~bit[56]&bit[55]&~bit[54]&~bit[53]) | (bit[57]&~bit[56]&bit[55]&bit[54]&~bit[53]) | (bit[57]&~bit[56]&~bit[54]&bit[53]) | (~bit[57]&bit[55]&bit[54]&bit[53]) | (bit[56]&bit[55]&bit[54]&~bit[53]) | (~bit[56]&bit[55]&bit[54]&bit[53]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[57]&bit[56]&~bit[55]&bit[54]&bit[53]) | (~bit[57]&~bit[56]&bit[55]&bit[54]&bit[53]) | (bit[57]&~bit[56]&~bit[55]&bit[54]&bit[53]) | (bit[57]&bit[56]&~bit[55]&bit[54]&~bit[53]) | (bit[57]&bit[56]&bit[55]&bit[54]&bit[53]) | (bit[57]&~bit[55]&~bit[54]&bit[53]) | (~bit[57]&bit[56]&bit[55]&~bit[54]&bit[53]) | (bit[57]&~bit[56]&bit[55]&bit[54]&~bit[53]) | (bit[57]&~bit[56]&~bit[54]&bit[53]) | (bit[56]&~bit[54]&~bit[53]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[57]&bit[56]&bit[55]&bit[54]&~bit[53]) | (bit[57]&bit[56]&bit[55]&~bit[54]&bit[53]) | (~bit[57]&~bit[56]&bit[55]&~bit[54]&bit[53]) | (~bit[57]&bit[56]&~bit[55]&~bit[54]&bit[53]) | (bit[57]&~bit[56]&~bit[55]&bit[54]&bit[53]) | (~bit[56]&~bit[55]&bit[54]&~bit[53]) | (~bit[56]&bit[55]&bit[54]&bit[53]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[52]);
		control=control & bit[52];

		//-------------53--------------

		out[5] = (~disable_update & ((~bit[58]&~bit[57]&~bit[56]&~bit[55]&bit[54]) | (bit[57]&bit[56]&~bit[55]&~bit[54]) | (bit[55]&~bit[54]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[58]&~bit[57]&~bit[56]&bit[55]&~bit[54]) | (bit[58]&~bit[57]&~bit[56]&bit[55]) | (bit[58]&~bit[56]&~bit[55]&bit[54]) | (~bit[57]&bit[56]&~bit[55]&bit[54]) | (~bit[58]&bit[57]&~bit[56]&bit[55]) | (bit[58]&bit[56]&~bit[55]&bit[54]) | (bit[57]&bit[56]&~bit[55]&~bit[54]) | (~bit[56]&bit[55]&bit[54]) | (~bit[58]&bit[57]&~bit[55]&bit[54]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[58]&bit[57]&~bit[56]&bit[55]&~bit[54]) | (bit[58]&~bit[57]&~bit[56]&~bit[55]&bit[54]) | (~bit[57]&bit[56]&bit[55]&~bit[54]) | (~bit[58]&~bit[57]&bit[56]&bit[55]) | (~bit[57]&bit[56]&~bit[55]&bit[54]) | (bit[57]&~bit[56]&~bit[55]&bit[54]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[58]&bit[57]&~bit[56]&bit[55]&~bit[54]) | (bit[58]&~bit[57]&~bit[56]&~bit[55]&bit[54]) | (~bit[58]&~bit[57]&~bit[56]&bit[55]&~bit[54]) | (~bit[58]&bit[57]&bit[56]&bit[55]&~bit[54]) | (bit[58]&bit[57]&bit[56]&~bit[55]) | (~bit[57]&~bit[56]&~bit[55]&~bit[54]) | (bit[57]&bit[56]&~bit[55]&~bit[54]) | (~bit[58]&bit[57]&~bit[55]&bit[54]) | (bit[56]&bit[55]&bit[54]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[58]&bit[57]&bit[56]&bit[55]&~bit[54]) | (~bit[58]&~bit[57]&~bit[56]&bit[55]&bit[54]) | (bit[58]&~bit[57]&bit[56]&bit[55]&bit[54]) | (~bit[58]&bit[57]&bit[56]&bit[54]) | (~bit[57]&bit[56]&~bit[55]&~bit[54]) | (bit[58]&~bit[57]&~bit[56]&bit[55]) | (bit[58]&~bit[56]&~bit[55]&bit[54]) | (bit[57]&~bit[56]&~bit[55]&~bit[54]) | (~bit[58]&bit[56]&~bit[55]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[58]&bit[57]&bit[56]&bit[55]&~bit[54]) | (~bit[58]&~bit[57]&~bit[56]&bit[55]&bit[54]) | (bit[58]&~bit[57]&bit[56]&bit[55]&bit[54]) | (~bit[58]&~bit[57]&bit[56]&bit[55]) | (~bit[58]&bit[57]&~bit[56]&bit[55]) | (bit[57]&~bit[56]&~bit[55]&bit[54]) | (bit[58]&bit[57]&bit[56]&~bit[55]) | (bit[57]&~bit[56]&~bit[55]&~bit[54]) | (bit[58]&bit[56]&~bit[55]&bit[54]) | (~bit[57]&~bit[56]&~bit[55]&~bit[54]) | (~bit[58]&bit[57]&~bit[55]&bit[54]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[53]);
		control=control & bit[53];

		//-------------54--------------

		out[5] = (~disable_update & ((bit[59]&bit[58]&bit[57]&bit[56]&bit[55]) | (bit[57]&~bit[56]&~bit[55]) | (bit[56]&~bit[55]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[59]&bit[58]&bit[57]&bit[56]&bit[55]) | (bit[57]&~bit[56]&~bit[55]) | (~bit[56]&bit[55]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[59]&~bit[58]&~bit[57]&bit[56]&~bit[55]) | (~bit[58]&~bit[57]&bit[55]) | (~bit[57]&bit[56]&bit[55]) | (~bit[59]&~bit[57]&~bit[56]&bit[55]) | (~bit[59]&~bit[57]&bit[56]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[59]&bit[58]&bit[57]&bit[56]&bit[55]) | (bit[59]&bit[58]&~bit[57]&bit[56]&~bit[55]) | (bit[59]&~bit[57]&~bit[56]&bit[55]) | (~bit[58]&bit[57]&~bit[55]) | (~bit[58]&~bit[57]&bit[56]&bit[55]) | (~bit[59]&~bit[58]&bit[56]) | (~bit[58]&~bit[56]&bit[55]) | (~bit[59]&~bit[57]&~bit[56]&bit[55]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[59]&~bit[58]&~bit[57]&bit[56]&~bit[55]) | (~bit[59]&bit[58]&bit[57]) | (bit[59]&bit[58]&bit[57]&bit[56]&bit[55]) | (bit[59]&bit[58]&~bit[57]&bit[56]&~bit[55]) | (~bit[59]&~bit[58]&~bit[57]&bit[55]) | (bit[59]&~bit[57]&~bit[56]&bit[55]) | (~bit[58]&~bit[57]&~bit[56]&~bit[55]) | (~bit[58]&bit[57]&bit[56]&bit[55]) | (~bit[59]&bit[57]&bit[56]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[60]&bit[59]&bit[57]&bit[56]&bit[55]) | (~bit[59]&bit[58]&bit[57]&~bit[55]) | (bit[59]&bit[58]&~bit[57]&bit[56]&~bit[55]) | (~bit[58]&~bit[57]&~bit[56]&~bit[55]) | (~bit[59]&bit[57]&~bit[56]) | (~bit[58]&~bit[57]&bit[56]&bit[55]) | (~bit[58]&bit[57]&bit[56]&bit[55]) | (~bit[59]&~bit[57]&~bit[56]&bit[55]) | (~bit[59]&~bit[57]&bit[56]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[54]);
		control=control & bit[54];

		//-------------55--------------

		out[5] = (~disable_update & ((bit[59]&~bit[58]&bit[57]&~bit[56]) | (~bit[60]&bit[59]&~bit[58]&~bit[56]) | (bit[60]&bit[59]&~bit[57]&~bit[56]) | (bit[59]&bit[58]&~bit[57]&~bit[56]) | (~bit[59]&~bit[56]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[60]&~bit[59]&~bit[57]) | (bit[59]&bit[58]&bit[57]&~bit[56]) | (~bit[57]&bit[56]) | (~bit[58]&~bit[57]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[60]&~bit[59]&~bit[58]&bit[57]) | (bit[60]&bit[58]&~bit[57]&~bit[56]) | (bit[59]&bit[58]&~bit[57]&~bit[56]) | (bit[59]&bit[58]&bit[57]&~bit[56]) | (~bit[58]&bit[56]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[60]&~bit[59]&~bit[58]&bit[57]&~bit[56]) | (bit[60]&~bit[59]&~bit[57]) | (~bit[60]&bit[59]&bit[58]&~bit[57]) | (bit[59]&~bit[58]&bit[57]&~bit[56]) | (~bit[60]&~bit[59]&~bit[58]&~bit[57]) | (bit[59]&bit[58]&bit[57]&~bit[56]) | (~bit[59]&bit[56]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[59]&bit[58]&~bit[57]&bit[56]) | (bit[60]&~bit[59]&~bit[58]&bit[57]&~bit[56]) | (~bit[60]&bit[59]&~bit[58]&~bit[56]) | (bit[59]&~bit[58]&bit[56]) | (~bit[60]&bit[58]&bit[57]) | (bit[60]&bit[58]&~bit[57]) | (bit[59]&bit[58]&bit[57]&~bit[56]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[60]&bit[59]&bit[58]&bit[57]) | (bit[60]&~bit[59]&~bit[58]&bit[57]&~bit[56]) | (bit[60]&~bit[59]&bit[58]&~bit[56]) | (bit[60]&bit[59]&~bit[57]&~bit[56]) | (~bit[60]&~bit[59]&~bit[58]&~bit[57]) | (bit[58]&bit[57]&bit[56]) | (~bit[60]&bit[56]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[55]);
		control=control & bit[55];

		//-------------56--------------

		out[5] = (~disable_update & ((bit[60]&~bit[59]&bit[58]&~bit[57]) | (bit[60]&~bit[59]&~bit[58]&~bit[57]) | (bit[59]&~bit[58]&~bit[57]) | (~bit[60]&~bit[57]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[60]&bit[59]&bit[58]&~bit[57]) | (bit[60]&~bit[59]&~bit[58]&~bit[57]) | (~bit[60]&~bit[58]&bit[57]) | (~bit[60]&~bit[59]&~bit[58]) | (~bit[59]&~bit[58]&bit[57]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[60]&bit[59]&~bit[58]&bit[57]) | (bit[60]&bit[59]&bit[58]&~bit[57]) | (~bit[59]&bit[58]&bit[57]) | (~bit[60]&~bit[59]&bit[58]) | (bit[59]&~bit[58]&~bit[57]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[60]&bit[59]&~bit[58]&bit[57]) | (bit[61]&~bit[60]&~bit[58]&~bit[57]) | (~bit[60]&bit[59]&bit[58]&bit[57]) | (bit[60]&bit[59]&bit[58]&~bit[57]) | (bit[60]&~bit[59]&bit[58]&~bit[57]) | (~bit[61]&bit[59]&~bit[57]) | (~bit[60]&~bit[59]&~bit[58]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[61]&~bit[60]&bit[59]&bit[58]&~bit[57]) | (bit[61]&bit[60]&bit[59]&~bit[58]&~bit[57]) | (~bit[61]&bit[60]&~bit[58]&bit[57]) | (~bit[61]&~bit[59]&~bit[58]) | (~bit[61]&bit[59]&bit[58]&bit[57]) | (~bit[61]&bit[60]&bit[58]&~bit[57]) | (~bit[61]&~bit[60]&~bit[58]&~bit[57]) | (~bit[60]&~bit[59]&bit[57]) | (~bit[59]&~bit[58]&bit[57]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[61]&~bit[59]&bit[58]) | (~bit[61]&~bit[60]&bit[57]) | (bit[61]&~bit[60]&~bit[58]&~bit[57]) | (~bit[60]&bit[59]&bit[58]&bit[57]) | (bit[61]&bit[60]&bit[59]) | (bit[60]&~bit[59]&bit[58]&~bit[57]) | (bit[60]&~bit[59]&~bit[58]&~bit[57]) | (~bit[61]&~bit[59]&bit[57]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[56]);
		control=control & bit[56];

		//-------------57--------------

		out[5] = (~disable_update & ((bit[61]&bit[60]&~bit[59]&~bit[58]) | (bit[61]&~bit[60]&~bit[58]) | (~bit[61]&~bit[58]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[61]&~bit[59]&~bit[58]) | (bit[61]&bit[60]&bit[59]&~bit[58]) | (~bit[62]&~bit[61]&~bit[59]) | (~bit[60]&~bit[59]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[61]&bit[60]&bit[59]&~bit[58]) | (bit[62]&~bit[61]&~bit[59]&bit[58]) | (~bit[62]&~bit[60]&bit[58]) | (~bit[61]&~bit[60]&bit[59]) | (~bit[62]&~bit[61]&~bit[60]) | (bit[61]&bit[60]&~bit[59]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[62]&bit[60]&~bit[59]&bit[58]) | (~bit[62]&~bit[61]&~bit[60]&~bit[59]&bit[58]) | (~bit[61]&bit[60]&bit[59]&bit[58]) | (bit[62]&bit[61]&~bit[60]&bit[58]) | (bit[61]&bit[60]&bit[59]&~bit[58]) | (bit[62]&~bit[60]&~bit[59]&~bit[58]) | (bit[61]&~bit[60]&~bit[58]) | (bit[61]&bit[60]&~bit[59]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[62]&~bit[61]&bit[59]&bit[58]) | (bit[62]&~bit[61]&~bit[60]&~bit[59]) | (~bit[62]&bit[60]&~bit[58]) | (bit[61]&bit[60]&~bit[59]&~bit[58]) | (bit[62]&~bit[61]&~bit[59]&bit[58]) | (bit[62]&bit[61]&bit[59]) | (~bit[62]&bit[60]&bit[59]) | (~bit[62]&bit[61]&~bit[58]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[62]&~bit[60]&bit[59]&~bit[58]) | (bit[62]&~bit[61]&bit[60]
&~bit[58]) | (~bit[61]&~bit[60]&bit[59]&bit[58]) | (bit[62]&bit[61]&~bit[60]&bit[58]) | (bit[62]&~bit[60]&~bit[59]&~bit[58]) | (~bit[62]&bit[61]&bit[60]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[57]);
		control=control & bit[57];

		//-------------58--------------

		out[5] = (~disable_update & ((~bit[63]&bit[61]&~bit[60]&~bit[59]) | (bit[63]&bit[62]&bit[61]&~bit[60]&~bit[59]) | (~bit[62]&bit[61]&~bit[60]&~bit[59]) | (bit[62]&~bit[61]&~bit[60]&~bit[59]) | (bit[60]&~bit[59]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[63]&bit[61]&~bit[60]&~bit[59]) | (~bit[62]&bit[61]&~bit[60]&~bit[59]) | (bit[62]&~bit[61]&~bit[60]&~bit[59]) | (~bit[60]&bit[59]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[63]&bit[62]&~bit[61]&bit[60]&~bit[59]) | (bit[63]&bit[62]&bit[61]&~bit[60]&~bit[59]) | (~bit[63]&~bit[61]&~bit[60]&bit[59]) | (~bit[63]&~bit[62]&bit[60]&bit[59]) | (~bit[62]&~bit[61]&bit[59]) | (~bit[62]&~bit[61]&bit[60]) | (~bit[61]&bit[60]&bit[59]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[63]&bit[62]&~bit[61]&bit[60]&~bit[59]) | (~bit[63]&bit[62]&bit[61]&bit[59]) | (bit[63]&bit[62]&bit[61]&~bit[60]&~bit[59]) | (~bit[63]&~bit[62]&bit[60]&~bit[59]) | (bit[63]&bit[62]&~bit[61]&~bit[60]&bit[59]) | (bit[62]&~bit[61]&~bit[60]&~bit[59]) | (bit[63]&~bit[62]&bit[60]&bit[59]) | (~bit[62]&~bit[60]&bit[59]) | (~bit[61]&bit[60]&bit[59]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[63]&bit[62]&bit[61]&bit[60]&bit[59]) | (~bit[63]&bit[62]&~bit[61]&bit[60]&~bit[59]) | (~bit[63]&bit[62]&~bit[61]&~bit[60]&~bit[59]) | (~bit[63]&~bit[62]&bit[61]&~bit[60]) | (bit[63]&bit[62]&bit[61]&~bit[60]&~bit[59]) | (bit[63]&bit[62]&~bit[61]&~bit[60]&bit[59]) | (~bit[62]&~bit[61]&bit[60]&bit[59]) | (~bit[63]&~bit[61]&~bit[60]&bit[59]) | (bit[63]&~bit[62]&bit[60]&~bit[59]) | (bit[63]&~bit[62]&bit[60]&bit[59]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[63]&bit[62]&bit[61]&bit[60]&~bit[59]) | (bit[63]&bit[62]&bit[61]&bit[60]&bit[59]) | (bit[63]&bit[62]&~bit[61]&bit[60]&~bit[59]) | (~bit[63]&bit[62]&~bit[61]&~bit[60]&~bit[59]) | (bit[63]&~bit[62]&~bit[60]&bit[59]) | (bit[63]&bit[62]&~bit[61]&~bit[60]&bit[59]) | (~bit[63]&~bit[61]&bit[60]&bit[59]) | (~bit[62]&bit[61]&~bit[60]&~bit[59]) | (bit[63]&~bit[62]&bit[60]&~bit[59]) | (~bit[63]&~bit[62]&bit[60]&bit[59]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[58]);
		control=control & bit[58];

		//-------------59--------------


		out[5] = (~disable_update & ((~bit[64]&~bit[63]&~bit[62]&bit[61]&~bit[60]) | (bit[64]&bit[63]&bit[61]&~bit[60]) | (~bit[64]&bit[62]&bit[61]&~bit[60]) | (~bit[63]&bit[62]&~bit[61]&~bit[60]) | (bit[64]&~bit[63]&bit[61]&~bit[60]) | (~bit[62]&~bit[61]&bit[60]) | (bit[63]&~bit[61]&~bit[60]) | (bit[63]&~bit[62]&~bit[60]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[64]&~bit[63]&~bit[62]&bit[61]&~bit[60]) | (~bit[62]&bit[61]&bit[60]) | (bit[63]&~bit[61]&~bit[60]) | (bit[62]&~bit[61]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[64]&bit[62]&bit[61]&~bit[60]) | (bit[64]&~bit[63]&bit[61]&~bit[60]) | (bit[63]&~bit[62]&~bit[60]) | (bit[62]&bit[60]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[64]&bit[63]&~bit[62]&bit[61]&~bit[60]) | (bit[64]&bit[63]&bit[61]&~bit[60]) | (bit[64]&~bit[63]&~bit[62]&bit[61]) | (~bit[64]&~bit[63]&bit[61]&bit[60]) | (~bit[64]&~bit[63]&bit[62]&~bit[61]) | (~bit[63]&bit[62]&~bit[61]&~bit[60]) | (~bit[63]&~bit[62]&bit[60]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[64]&bit[62]&bit[61]&~bit[60]) | (~bit[64]&bit[63]&bit[62]&~bit[61]&~bit[60]) | (~bit[64]&bit[63]&~bit[62]&bit[61]&~bit[60]) | (bit[64]&~bit[63]&bit[62]&bit[60]) | (~bit[64]&~bit[63]&bit[62]&bit[61]) | (bit[64]&~bit[63]&bit[61]&~bit[60]) | (~bit[64]&~bit[62]&bit[60]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[64]&bit[63]&bit[62]&~bit[60]) | (~bit[63]&~bit[62]&~bit[61]&~bit[60]) | (~bit[64]&~bit[62]&~bit[61]&~bit[60]) | (bit[64]&~bit[63]&~bit[62]&bit[61]) | (~bit[64]&~bit[63]&bit[61]&bit[60]) | (~bit[64]&~bit[63]&bit[62]&~bit[61]) | (~bit[64]&~bit[63]&bit[62]&bit[61]) | (~bit[64]&bit[62]&bit[60]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[59]);
		control=control & bit[59];


		//-------------60--------------

		out[5] = (~disable_update & ((bit[64]&~bit[63]&bit[62]&~bit[61]) | (~bit[63]&~bit[62]&bit[61]) | (bit[63]&bit[62]&~bit[61]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[65]&bit[64]&~bit[63]&bit[62]&bit[61]) | (~bit[65]&~bit[64]&bit[63]&bit[62]) | (bit[64]&~bit[63]&bit[62]&~bit[61]) | (bit[65]&~bit[64]&bit[62]&bit[61]) | (~bit[65]&~bit[63]&bit[62]&bit[61]) | (bit[63]&~bit[62]&bit[61]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[64]&~bit[63]&bit[62]&bit[61]) | (bit[65]&bit[63]&bit[62]&~bit[61]) | (~bit[65]&~bit[64]&~bit[62]&bit[61]) | (~bit[65]&~bit[63]&bit[62]&bit[61]) | (bit[64]&bit[63]&bit[62]) | (bit[63]&~bit[62]&bit[61]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[65]&~bit[64]&bit[63]&~bit[61]) | (bit[65]&bit[64]&~bit[63]&bit[62]&bit[61]) | (bit[64]&bit[63]&bit[62]&bit[61]) | (bit[65]&~bit[64]&~bit[62]&bit[61]) | (~bit[65]&bit[64]&~bit[61]) | (~bit[65]&bit[64]&~bit[62]) | (~bit[65]&bit[63]&bit[61]) | (~bit[62]&~bit[61]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[65]&bit[64]&bit[63]&bit[62]&bit[61]) | (bit[65]&bit[64]&~bit[63]&bit[62]&bit[61]) | (~bit[65]&~bit[64]&~bit[63]&bit[62]) | (bit[65]&bit[64]&~bit[62]&bit[61]) | (~bit[64]&bit[63]&~bit[62]&bit[61]) | (bit[65]&bit[63]&bit[62]&~bit[61]) | (bit[65]&~bit[64]&~bit[62]&bit[61]) | (~bit[63]&~bit[61]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[65]&bit[64]&~bit[62]&bit[61]) | (~bit[65]&~bit[64]&~bit[62]&bit[61]) | (bit[65]&~bit[64]&bit[62]&bit[61]) | (~bit[64]&~bit[63]&~bit[61]) | (~bit[64]&~bit[62]&~bit[61]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[60]);
		control=control & bit[60];

		//-------------61--------------

		out[5] = (~disable_update & ((~bit[66]&bit[65]&~bit[64]&bit[63]&~bit[62]) | (~bit[66]&~bit[65]&bit[63]&~bit[62]) | (bit[66]&~bit[64]&bit[63]&~bit[62]) | (~bit[64]&~bit[63]&bit[62]) | (bit[64]&bit[63]&~bit[62]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[66]&bit[65]&~bit[64]&bit[63]&~bit[62]) | (~bit[65]&~bit[64]&bit[63]) | (bit[64]&~bit[63]&bit[62]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[66]&bit[65]&bit[63]&~bit[62]) | (bit[65]&bit[63]&bit[62]) | (~bit[66]&~bit[65]&bit[63]&~bit[62]) | (bit[64]&bit[63]&~bit[62]) | (~bit[65]&bit[64]&bit[62]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[66]&~bit[65]&bit[64]&bit[63]&~bit[62]) | (~bit[66]&~bit[65]&bit[64]&bit[63]) | (bit[65]&~bit[64]&bit[63]&bit[62]) | (bit[66]&~bit[64]&bit[63]&~bit[62]) | (~bit[64]&~bit[63]&bit[62]) | (~bit[65]&~bit[64]&~bit[63]) | (bit[64]&~bit[63]&bit[62]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[66]&~bit[65]&bit[63]&bit[62]) | (~bit[66]&bit[64]&~bit[63]&bit[62]) | (~bit[66]&bit[65]&bit[64]&bit[63]) | (bit[65]&~bit[64]&~bit[63]&~bit[62]) | (bit[65]&~bit[64]&bit[63]&bit[62]) | (~bit[66]&~bit[65]&bit[63]&~bit[62]) | (bit[66]&~bit[64]&bit[63]&~bit[62]) | (~bit[65]&~bit[64]&~bit[63]) | (~bit[65]&~bit[64]&bit[63]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[66]&bit[65]&~bit[64]&bit[63]&~bit[62]) | (bit[66]&~bit[65]&bit[64]&bit[63]&~bit[62]) | (bit[66]&bit[65]&bit[63]&~bit[62]) | (bit[65]&~bit[64]&~bit[63]&~bit[62]) | (~bit[66]&~bit[64]&bit[62]) | (~bit[65]&bit[64]&~bit[63]) | (~bit[65]&bit[64]&bit[62]) | (bit[64]&~bit[63]&bit[62]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[61]);
		control=control & bit[61];

		//-------------62--------------

		out[5] = (~disable_update & ((~bit[67]&~bit[66]&~bit[65]&~bit[64]&bit[63]) | (bit[67]&bit[66]&bit[65]&~bit[64]&~bit[63]) | (~bit[67]&bit[66]&bit[65]&~bit[64]&~bit[63]) | (bit[65]&bit[64]&~bit[63]) | (~bit[65]&bit[64]&~bit[63]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[67]&bit[66]&bit[65]&~bit[64]&~bit[63]) | (~bit[67]&bit[66]&~bit[64]&bit[63]) | (bit[67]&bit[66]&bit[65]&~bit[64]&bit[63]) | (bit[67]&~bit[65]&~bit[64]&bit[63]) | (~bit[67]&bit[66]&bit[65]&~bit[64]&~bit[63]) | (bit[67]&~bit[66]&~bit[64]&bit[63]) | (~bit[66]&~bit[65]&bit[64]) | (~bit[67]&bit[65]&~bit[64]&bit[63]) | (~bit[65]&bit[64]&~bit[63]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[67]&~bit[65]&~bit[64]&bit[63]) | (~bit[67]&bit[66]&bit[65]&~bit[64]&~bit[63]) | (~bit[66]&bit[65]&bit[64]) | (bit[66]&~bit[65]&bit[63]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[67]&bit[66]&bit[65]&~bit[64]&~bit[63]) | (bit[67]&~bit[66]&~bit[64]&bit[63]) | (~bit[66]&~bit[65]&~bit[64]&~bit[63]) | (bit[66]&bit[64]&bit[63]) | (bit[65]&bit[64]&~bit[63]) | (~bit[67]&bit[65]&~bit[64]&bit[63]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[67]&~bit[66]&~bit[65]&~bit[64]&bit[63]) | (~bit[67]&bit[65]&bit[64]&~bit[63]) | (bit[67]&bit[66]&bit[65]&~bit[64]&bit[63]) | (~bit[66]&bit[65]&~bit[64]&bit[63]) | (~bit[66]&~bit[65]&~bit[64]&~bit[63]) | (~bit[67]&bit[66]&bit[64]&bit[63]) | (~bit[66]&bit[64]&~bit[63]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[67]&bit[66]&~bit[64]&bit[63]) | (bit[67]&bit[66]&bit[65]&~bit[64]&bit[63]) | (bit[66]&~bit[65]&~bit[64]&~bit[63]) | (~bit[67]&bit[66]&bit[65]&~bit[64]&~bit[63]) | (~bit[67]&bit[66]&bit[64]&bit[63]) | (~bit[67]&~bit[66]&bit[64]) | (~bit[67]&~bit[65]&bit[64]) | (~bit[67]&bit[65]&~bit[64]&bit[63]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[62]);
		control=control & bit[62];

		//-------------63--------------

		out[5] = (~disable_update & ((bit[68]&~bit[67]&~bit[66]&~bit[65]&bit[64]) | (~bit[68]&~bit[66]&~bit[65]&bit[64]) | (bit[66]&~bit[65]&~bit[64]) | (bit[65]&~bit[64]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[68]&bit[67]&~bit[65]&bit[64]) | (~bit[68]&~bit[66]&bit[65]&bit[64]) | (~bit[67]&bit[66]&~bit[65]&bit[64]) | (~bit[68]&bit[67]&bit[66]&~bit[65]) | (bit[66]&~bit[65]&~bit[64]) | (~bit[67]&~bit[66]&bit[65]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[68]&bit[67]&~bit[66]&bit[64]) | (bit[67]&~bit[66]&bit[65]&~bit[64]) | (~bit[68]&bit[67]&bit[66]&bit[65]) | (~bit[68]&~bit[67]&bit[66]) | (bit[66]&bit[65]&bit[64]) | (~bit[67]&bit[66]&bit[65]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[68]&bit[67]&bit[66]&~bit[64]) | (bit[68]&bit[67]&~bit[65]&bit[64]) | (~bit[68]&~bit[67]&~bit[65]&bit[64]) | (bit[67]&~bit[66]&bit[65]&~bit[64]) | (bit[68]&bit[67]&~bit[66]&bit[65]) | (~bit[68]&~bit[67]&bit[66]&bit[65]) | (bit[68]&~bit[67]&bit[66]&~bit[65]) | (~bit[68]&bit[67]&bit[66]&~bit[65]) | (~bit[67]&bit[66]&bit[64]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[68]&~bit[67]&~bit[66]&~bit[65]&bit[64]) | (bit[68]&bit[66]&bit[65]&~bit[64]) | (~bit[67]&~bit[66]&~bit[65]&~bit[64]) | (~bit[68]&~bit[66]&bit[65]&~bit[64]) | (bit[68]&bit[67]&~bit[66]&bit[64]) | (~bit[68]&~bit[67]&bit[65]&bit[64]) | (~bit[67]&~bit[66]&bit[65]&bit[64]) | (bit[68]&~bit[67]&bit[66]&~bit[65]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[68]&~bit[67]&~bit[66]&bit[65]&bit[64]) | (bit[68]&~bit[67]&~bit[66]&~bit[65]&bit[64]) | (~bit[68]&bit[67]&bit[66]&bit[65]) | (bit[68]&bit[67]&~bit[66]&bit[65]) | (~bit[67]&bit[66]&~bit[65]&bit[64]) | (~bit[68]&bit[67]&bit[66]&~bit[65]) | (bit[67]&~bit[66]&~bit[65]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[63]);
		control=control & bit[63];

		//-------------64--------------

		out[5] = (~disable_update & ((bit[69]&bit[68]&~bit[67]&~bit[66]&bit[65]) | (bit[69]&~bit[68]&~bit[67]&~bit[66]&bit[65]) | (~bit[69]&~bit[68]&~bit[66]&bit[65]) | (~bit[69]&~bit[67]&~bit[66]&bit[65]) | (bit[66]&~bit[65]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[69]&~bit[68]&bit[67]&~bit[66]&bit[65]) | (bit[68]&bit[67]&~bit[66]&bit[65]) | (~bit[69]&~bit[68]&~bit[67]&bit[66]) | (~bit[69]&~bit[68]&bit[67]&bit[66]) | (~bit[68]&bit[66]&~bit[65]) | (bit[69]&~bit[68]&~bit[67]&bit[66]) | (bit[68]&~bit[67]&bit[66]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[69]&~bit[68]&bit[67]&~bit[66]&bit[65]) | (~bit[69]&bit[68]&bit[67]&bit[66]) | (bit[69]&~bit[68]&bit[67]&bit[66]&bit[65]) | (bit[68]&bit[67]&~bit[66]&bit[65]) | (~bit[69]&~bit[68]&~bit[67]&bit[66]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[69]&~bit[68]&bit[67]&~bit[66]&bit[65]) | (bit[69]&~bit[68]&~bit[67]&~bit[66]&bit[65]) | (bit[69]&~bit[68]&bit[67]&bit[66]&bit[65]) | (~bit[69]&~bit[67]&~bit[66]&bit[65]) | (bit[69]&bit[68]&bit[67]&bit[66]) | (bit[68]&bit[67]&bit[66]&~bit[65]) | (~bit[67]&~bit[66]&~bit[65]) | (bit[69]&~bit[68]&~bit[67]&bit[66]) | (bit[68]&~bit[67]&bit[66]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[69]&bit[68]&bit[67]&~bit[66]&bit[65]) | (bit[69]&bit[68]&~bit[67]&~bit[66]&bit[65]) | (~bit[69]&~bit[68]&~bit[67]&~bit[66]) | (~bit[69]&~bit[67]&bit[66]&~bit[65]) | (bit[69]&bit[68]&bit[67]&bit[66]) | (~bit[69]&~bit[68]&bit[67]&bit[66]) | (~bit[68]&~bit[67]&~bit[65]) | (bit[69]&~bit[68]&~bit[67]&bit[66]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[69]&~bit[68]&~bit[67]&~bit[66]&bit[65]) | (bit[69]&~bit[68]&bit[67]&bit[66]&bit[65]) | (~bit[68]&bit[67]&~bit[66]&~bit[65]) | (~bit[69]&bit[68]&bit[66]&bit[65]) | (~bit[69]&bit[67]&bit[66]&~bit[65]) | (~bit[68]&~bit[67]&bit[66]&~bit[65]) | (bit[68]&bit[67]&~bit[66]&bit[65]) | (bit[68]&bit[67]&bit[66]&~bit[65]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[64]);
		control=control & bit[64];

		//-------------65--------------

		out[5] = (~disable_update & ((~bit[70]&bit[69]&~bit[68]&~bit[67]&bit[66]) | (bit[70]&~bit[69]&bit[68]&bit[67]&~bit[66]) | (~bit[69]&~bit[68]&bit[67]&~bit[66]) | (bit[70]&bit[69]&bit[68]&bit[67]&~bit[66]) | (~bit[69]&~bit[68]&~bit[67]&bit[66]) | (bit[69]&bit[68]&~bit[67]&~bit[66]) | (~bit[70]&bit[67]&~bit[66]) | (bit[69]&~bit[68]&bit[67]&~bit[66]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[70]&bit[69]&~bit[67]&bit[66]) | (bit[70]&~bit[69]&bit[68]&~bit[67]&bit[66]) | (~bit[70]&~bit[68]&bit[67]&bit[66]) | (bit[70]&~bit[69]&~bit[68]&bit[67]) | (~bit[69]&~bit[68]&bit[67]&~bit[66]) | (~bit[70]&bit[68]&~bit[67]&bit[66]) | (bit[69]&bit[68]&~bit[67]&~bit[66]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[70]&~bit[69]&bit[68]&~bit[67]&bit[66]) | (bit[70]&~bit[69]&bit[68]&bit[67]&~bit[66]) | (~bit[70]&bit[68]&~bit[67]&bit[66]) | (~bit[69]&bit[68]&bit[67]&bit[66]) | (bit[69]&bit[68]&~bit[67]&~bit[66]) | (~bit[70]&bit[68]&bit[67]&bit[66]) | (bit[70]&bit[69]&~bit[68]&bit[66]) | (~bit[70]&bit[67]&~bit[66]) | (bit[69]&~bit[68]&bit[67]&~bit[66]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[70]&~bit[69]&bit[68]&bit[67]&~bit[66]) | (~bit[70]&~bit[69]&~bit[67]&bit[66]) | (bit[70]&bit[69]&bit[68]&bit[66]) | (bit[70]&bit[69]&bit[68]&bit[67]&~bit[66]) | (~bit[69]&~bit[68]&~bit[67]&bit[66]) | (bit[70]&bit[69]&~bit[68]&bit[66]) | (bit[69]&~bit[68]&bit[67]&~bit[66]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[70]&bit[69]&bit[68]&~bit[67]&~bit[66]) | (~bit[70]&bit[69]&~bit[68]&~bit[67]&bit[66]) | (~bit[69]&~bit[68]&~bit[67]&~bit[66]) | (bit[70]&~bit[69]&bit[68]&~bit[67]&bit[66]) | (~bit[70]&~bit[69]&bit[67]&bit[66]) | (bit[70]&bit[69]&bit[68]&bit[67]&~bit[66]) | (~bit[69]&bit[68]&bit[67]&bit[66]) | (bit[70]&bit[69]&~bit[68]&bit[66]) | (bit[69]&~bit[68]&bit[67]&~bit[66]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[70]&bit[69]&~bit[68]&~bit[66]) | (bit[70]&~bit[69]&bit[68]&bit[67]&~bit[66]) | (bit[70]&~bit[68]&bit[67]&bit[66]) | (~bit[70]&~bit[68]&~bit[67]&bit[66]) | (bit[70]&~bit[69]&~bit[68]&bit[67]) | (bit[70]&bit[69]&bit[68]&bit[66]) | (bit[70]&bit[69]&bit[68]&bit[67]&~bit[66]) | (bit[69]&~bit[67]&~bit[66]) | (~bit[70]&bit[68]&bit[67]&bit[66]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[65]);
		control=control & bit[65];

		//-------------66--------------

		out[5] = (~disable_update & ((bit[71]&~bit[69]&bit[68]&~bit[67]) | (~bit[70]&bit[69]&~bit[67]) | (~bit[71]&bit[68]&~bit[67]) | (bit[70]&~bit[68]&~bit[67]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[71]&bit[70]&bit[69]&bit[68]&~bit[67]) | (~bit[70]&bit[69]&~bit[68]) | (bit[70]&~bit[68]&bit[67]) | (bit[70]&~bit[69]&~bit[68]) | (~bit[69]&bit[67]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[71]&bit[70]&~bit[69]&bit[68]&~bit[67]) | (~bit[70]&~bit[69]&bit[68]&~bit[67]) | (bit[71]&bit[70]&bit[69]&bit[68]&~bit[67]) | (~bit[71]&bit[69]&bit[68]&bit[67]) | (~bit[70]&~bit[68]&bit[67]) | (~bit[70]&bit[69]&bit[68]&bit[67]) | (bit[70]&~bit[69]&~bit[68]) | (bit[70]&~bit[68]&~bit[67]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[71]&bit[70]&~bit[69]&bit[68]&~bit[67]) | (bit[71]&bit[70]&bit[69]&bit[68]&bit[67]) | (~bit[71]&bit[70]&~bit[68]&~bit[67]) | (~bit[71]&~bit[70]&bit[68]) | (bit[71]&bit[70]&bit[69]&bit[68]&~bit[67]) | (~bit[70]&~bit[69]&bit[67]) | (~bit[70]&bit[69]&bit[68]&bit[67]) | (bit[70]&bit[69]&~bit[68]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[71]&bit[70]&bit[69]&bit[68]&bit[67]) | (bit[71]&~bit[69]&bit[68]&~bit[67]) | (bit[71]&bit[70]&bit[69]&bit[68]&~bit[67]) | (~bit[71]&bit[69]&bit[68]&bit[67]) | (bit[70]&~bit[68]&bit[67]) | (bit[70]&bit[69]&~bit[68]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[71]&bit[70]&~bit[69]&bit[68]&~bit[67]) | (~bit[71]&bit[70]&~bit[69]&bit[68]&~bit[67]) | (bit[71]&~bit[70]&bit[69]&bit[68]) | (bit[71]&bit[70]&bit[69]&bit[68]&bit[67]) | (~bit[71]&~bit[69]&bit[67]) | (~bit[71]&bit[69]&~bit[68]) | (~bit[70]&bit[69]&bit[68]&bit[67]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[66]);
		control=control & bit[66];

		//-------------67--------------

		out[5] = (~disable_update & ((bit[72]&~bit[71]&bit[70]&~bit[69]&~bit[68]) | (bit[72]&~bit[71]&bit[70]&bit[69]&~bit[68]) | (bit[72]&bit[71]&bit[70]&~bit[69]&~bit[68]) | (bit[72]&bit[71]&~bit[70]&~bit[69]&~bit[68]) | (bit[71]&bit[70]&bit[69]&~bit[68]) | (~bit[72]&bit[71]&bit[69]&~bit[68]) | (~bit[72]&~bit[71]&bit[69]&~bit[68]) | (bit[72]&~bit[70]&bit[69]&~bit[68]) | (~bit[71]&~bit[70]&~bit[69]) | (~bit[72]&~bit[69]&~bit[68]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[72]&~bit[71]&~bit[70]&bit[69]&bit[68]) | (bit[72]&~bit[71]&bit[70]&~bit[69]&~bit[68]) | (~bit[72]&~bit[71]&bit[70]&~bit[69]&bit[68]) | (bit[72]&bit[71]&~bit[70]&~bit[69]&~bit[68]) | (bit[72]&~bit[71]&bit[70]&~bit[69]&bit[68]) | (~bit[71]&~bit[70]&~bit[69]&~bit[68]) | (bit[71]&~bit[69]&bit[68]) | (~bit[72]&~bit[69]&~bit[68]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[72]&~bit[71]&~bit[70]&bit[69]&bit[68]) | (~bit[72]&~bit[71]&bit[70]&~bit[69]&bit[68]) | (bit[72]&bit[71]&bit[70]&~bit[69]&~bit[68]) | (bit[71]&~bit[70]&bit[68]) | (~bit[72]&~bit[71]&bit[69]&~bit[68]) | (bit[72]&~bit[70]&bit[69]&~bit[68]) | (~bit[72]&~bit[70]&~bit[68]) | (~bit[71]&~bit[70]&~bit[69]&~bit[68]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[72]&~bit[71]&bit[70]&~bit[69]&~bit[68]) | (~bit[72]&~bit[71]&~bit[70]&bit[69]&~bit[68]) | (bit[72]&~bit[71]&~bit[70]&bit[69]&bit[68]) | (~bit[71]&bit[70]&bit[69]&bit[68]) | (bit[72]&~bit[71]&bit[70]&bit[69]&~bit[68]) | (bit[72]&bit[71]&bit[70]&~bit[69]&~bit[68]) | (bit[72]&bit[71]&~bit[70]&~bit[69]&~bit[68]) | (~bit[72]&bit[71]&~bit[70]&bit[68]) | (bit[71]&bit[70]&bit[69]&~bit[68]) | (bit[72]&~bit[71]&bit[70]&~bit[69]&bit[68]) | (bit[71]&~bit[69]&bit[68]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[71]&bit[70]&bit[69]&bit[68]) | (bit[72]&~bit[71]&bit[69]&~bit[68]) | (bit[72]&~bit[71]&bit[70]&~bit[69]&~bit[68]) | (bit[72]&~bit[71]&~bit[70]&bit[69]&bit[68]) | (~bit[72]&~bit[71]&~bit[70]&~bit[69]) | (bit[72]&bit[71]&~bit[70]&~bit[69]&~bit[68]) | (bit[72]&~bit[71]&bit[70]&~bit[69]&bit[68]) | (~bit[72]&bit[71]&bit[69]&~bit[68]) | (~bit[72]&bit[70]&bit[68]) | (~bit[72]&~bit[69]&bit[68]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[72]&~bit[71]&~bit[70]&bit[69]&bit[68]) | (~bit[72]&bit[71]&bit[69]&bit[68]) | (~bit[72]&~bit[71]&bit[70]&~bit[69]&bit[68]) | (bit[72]&~bit[71]&bit[70]&bit[69]&~bit[68]) | (bit[72]&bit[71]&bit[70]&~bit[69]&~bit[68]) | (~bit[72]&bit[71]&~bit[70]&bit[68]) | (bit[72]&~bit[71]&bit[70]&~bit[69]&bit[68]) | (bit[72]&~bit[70]&bit[69]&~bit[68]) | (~bit[71]&~bit[70]&~bit[69]&~bit[68]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[67]);
		control=control & bit[67];

		//-------------68--------------

		out[5] = (~disable_update & ((bit[73]&bit[72]&~bit[71]&bit[70]&~bit[69]) | (bit[73]&bit[72]&~bit[71]&~bit[70]&bit[69]) | (~bit[73]&bit[72]&~bit[71]&~bit[70]&bit[69]) | (bit[73]&~bit[72]&~bit[71]&~bit[70]&bit[69]) | (~bit[73]&~bit[72]&~bit[70]&bit[69]) | (~bit[73]&~bit[71]&bit[70]&~bit[69]) | (bit[72]&bit[71]&bit[70]&~bit[69]) | (~bit[72]&bit[70]&~bit[69]) | (bit[71]&~bit[70]&~bit[69]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[73]&bit[72]&~bit[71]&bit[70]&~bit[69]) | (bit[73]&~bit[72]&bit[71]&~bit[70]&bit[69]) | (~bit[73]&~bit[71]&bit[70]&bit[69]) | (bit[72]&bit[71]&~bit[70]&bit[69]) | (~bit[72]&~bit[71]&bit[70]&bit[69]) | (~bit[73]&~bit[71]&bit[70]&~bit[69]) | (~bit[72]&bit[70]&~bit[69]) | (bit[71]&~bit[70]&~bit[69]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[73]&bit[72]&bit[71]&~bit[70]) | (bit[73]&~bit[72]&bit[71]&~bit[70]&bit[69]) | (~bit[73]&bit[72]&~bit[71]&~bit[70]&bit[69]) | (~bit[72]&~bit[71]&~bit[70]&bit[69]) | (bit[73]&bit[72]&~bit[71]&bit[70]&bit[69]) | (~bit[72]&bit[71]&bit[70]&bit[69]) | (bit[72]&bit[71]&bit[70]&~bit[69]) | (bit[71]&~bit[70]&~bit[69]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[72]&~bit[71]&bit[70]&~bit[69]) | (bit[73]&bit[72]&~bit[71]&~bit[70]&bit[69]) | (~bit[73]&~bit[72]&~bit[70]&bit[69]) | (bit[73]&bit[72]&~bit[71]&bit[70]&bit[69]) | (bit[72]&bit[71]&~bit[70]&bit[69]) | (~bit[73]&~bit[71]&bit[70]&~bit[69]) | (~bit[72]&bit[71]&~bit[70]) | (bit[72]&bit[71]&bit[70]&~bit[69]) | (~bit[73]&bit[71]&bit[69]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[73]&bit[72]&~bit[71]&bit[70]&~bit[69]) | (~bit[73]&bit[72]&bit[71]&~bit[69]) | (bit[73]&~bit[72]&bit[71]&~bit[70]&bit[69]) | (~bit[73]&~bit[72]&~bit[71]&bit[70]) | (bit[73]&bit[72]&~bit[71]&~bit[70]&bit[69]) | (bit[73]&~bit[72]&~bit[71]&~bit[70]&bit[69]) | (bit[73]&bit[72]&bit[71]&bit[69]) | (bit[73]&bit[72]&~bit[71]&bit[70]&bit[69]) | (~bit[72]&~bit[71]&bit[70]&bit[69]) | (bit[72]&bit[71]&bit[70]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[73]&bit[72]&~bit[71]&~bit[70]&bit[69]) | (bit[73]&~bit[72]&~bit[71]&~bit[70]&bit[69]) | (bit[73]&bit[72]&bit[71]&bit[69]) | (~bit[73]&~bit[71]&bit[70]&bit[69]) | (bit[73]&bit[72]&~bit[71]&bit[70]&bit[69]) | (~bit[72]&bit[71]&bit[70]&bit[69]) | (~bit[73]&bit[71]&~bit[69]) | (~bit[72]&~bit[71]&~bit[69]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[68]);
		control=control & bit[68];

		//-------------69--------------

		out[5] = (~disable_update & ((bit[75]&bit[74]&bit[73]&bit[72]&bit[71]&bit[70]) | (~bit[75]&bit[73]&bit[72]&bit[71]&bit[70]) | (~bit[73]&~bit[72]&~bit[71]&bit[70]) | (bit[74]&~bit[73]&bit[71]&~bit[70]) | (~bit[74]&bit[73]&bit[72]&bit[71]&bit[70]) | (bit[73]&bit[72]&~bit[71]&~bit[70]) | (~bit[74]&~bit[72]&bit[71]&~bit[70]) | (bit[73]&bit[72]&bit[71]&~bit[70]) | (bit[74]&bit[73]&~bit[72]&bit[71]&~bit[70]) | (~bit[73]&bit[72]&~bit[70]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[75]&bit[74]&bit[73]&bit[72]&bit[71]&bit[70]) | (~bit[75]&bit[73]&bit[72]&bit[71]&bit[70]) | (~bit[73]&~bit[72]&bit[71]&~bit[70]) | (bit[74]&~bit[73]&bit[72]&~bit[71]) | (~bit[74]&bit[73]&bit[72]&bit[71]&bit[70]) | (bit[74]&bit[72]&~bit[71]&~bit[70]) | (~bit[74]&~bit[72]&bit[71]&~bit[70]) | (bit[73]&~bit[72]&~bit[71]&bit[70]) | (~bit[74]&bit[72]&~bit[71]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[74]&~bit[73]&~bit[72]&bit[71]&bit[70]) | (~bit[75]&bit[73]&bit[72]&bit[71]&bit[70]) | (bit[74]&bit[73]&bit[72]&~bit[71]&bit[70]) | (~bit[74]&~bit[73]&bit[72]&~bit[71]) | (~bit[74]&bit[72]&~bit[71]&~bit[70]) | (~bit[74]&bit[73]&bit[72]&bit[71]&bit[70]) | (bit[74]&bit[73]&~bit[72]&bit[71]&~bit[70]) | (bit[73]&~bit[72]&~bit[71]&bit[70]) | (~bit[73]&bit[72]&~bit[70]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[75]&bit[74]&bit[73]&bit[72]&bit[71]&bit[70]) | (~bit[74]&~bit[73]&bit[72]&~bit[71]&~bit[70]) | (bit[74]&bit[73]&bit[72]&~bit[71]&bit[70]) | (bit[74]&~bit[72]&bit[71]&bit[70]) | (~bit[74]&bit[73]&~bit[72]&bit[70]) | (bit[73]&bit[72]&bit[71]&~bit[70]) | (bit[74]&bit[73]&~bit[72]&bit[71]&~bit[70]) | (bit[73]&~bit[72]&~bit[71]&bit[70]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[75]&~bit[74]&bit[73]&bit[71]&bit[70]) | (bit[74]&bit[73]&bit[72]&~bit[71]&bit[70]) | (~bit[74]&~bit[73]&bit[71]&~bit[70]) | (bit[74]&~bit[73]&bit[72]&~bit[71]) | (~bit[74]&bit[72]&bit[71]&~bit[70]) | (~bit[74]&bit[73]&~bit[72]&bit[70]) | (bit[74]&bit[72]&~bit[71]&~bit[70]) | (bit[74]&bit[73]&~bit[72]&bit[71]&~bit[70]) | (~bit[73]&~bit[72]&bit[70]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[75]&bit[74]&bit[73]&bit[72]&bit[71]&bit[70]) | (~bit[73]&~bit[72]&~bit[71]&~bit[70]) | (~bit[74]&~bit[72]&~bit[71]&bit[70]) | (bit[74]&~bit[72]&bit[71]&bit[70]) | (~bit[74]&bit[72]&bit[71]&bit[70]) | (bit[74]&~bit[73]&bit[71]&~bit[70]) | (bit[73]&bit[72]&~bit[71]&~bit[70]) | (bit[74]&bit[73]&~bit[72]&bit[71]&~bit[70]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[69]);
		control=control & bit[69];

		//-------------70--------------

		out[5] = (~disable_update & ((bit[76]&bit[75]&~bit[74]&bit[73]&bit[72]&bit[71]) | (~bit[76]&~bit[75]&bit[73]&bit[72]&bit[71]) | (bit[76]&~bit[75]&bit[73]&bit[72]&bit[71]) | (~bit[76]&~bit[74]&bit[73]&bit[72]&bit[71]) | (bit[75]&bit[74]&bit[73]&bit[72]&bit[71]) | (bit[75]&bit[74]&bit[73]&~bit[72]&~bit[71]) | (~bit[73]&bit[72]&~bit[71]) | (~bit[75]&~bit[72]&~bit[71]) | (~bit[74]&~bit[71]) | (~bit[73]&~bit[72]&~bit[71]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[76]&bit[75]&~bit[74]&bit[73]&bit[72]&bit[71]) | (bit[74]&~bit[73]&~bit[72]&bit[71]) | (~bit[76]&~bit[75]&bit[73]&bit[72]&bit[71]) | (bit[76]&~bit[75]&bit[73]&bit[72]&bit[71]) | (~bit[76]&~bit[74]&bit[73]&bit[72]&bit[71]) | (bit[75]&bit[74]&bit[73]&bit[72]&bit[71]) | (~bit[74]&~bit[72]&bit[71]) | (bit[74]&bit[73]&bit[72]&~bit[71]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[76]&bit[75]&~bit[74]&bit[73]&bit[72]&bit[71]) | (~bit[76]&~bit[75]&bit[73]&bit[72]&bit[71]) | (bit[76]&~bit[75]&bit[73]&bit[72]&bit[71]) | (~bit[76]&~bit[74]&bit[73]&bit[72]&bit[71]) | (~bit[75]&~bit[74]&~bit[73]&~bit[72]) | (bit[74]&bit[73]&~bit[72]&bit[71]) | (~bit[74]&bit[73]&~bit[72]&~bit[71]) | (~bit[75]&~bit[72]&~bit[71]) | (bit[74]&bit[73]&bit[72]&~bit[71]) | (~bit[73]&~bit[72]&~bit[71]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[75]&~bit[74]&bit[72]&bit[71]) | (bit[75]&~bit[74]&~bit[73]&bit[72]&~bit[71]) | (bit[75]&~bit[74]&~bit[73]&~bit[72]) | (bit[74]&~bit[73]&~bit[72]&bit[71]) | (~bit[76]&~bit[74]&bit[73]&bit[72]&bit[71]) | (bit[75]&bit[74]&bit[73]&bit[72]&bit[71]) | (bit[75]&bit[74]&bit[73]&~bit[72]&~bit[71]) | (bit[74]&bit[73]&~bit[72]&bit[71]) | (~bit[75]&~bit[74]&~bit[73]&~bit[71]) | (~bit[74]&~bit[73]&bit[72]&bit[71]) | (bit[74]&bit[73]&bit[72]&~bit[71]) | (~bit[73]&~bit[72]&~bit[71]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[75]&bit[74]&bit[72]&~bit[71]) | (bit[76]&bit[75]&~bit[74]&bit[73]&bit[72]&bit[71]) | (~bit[75]&bit[74]&~bit[72]&bit[71]) | (~bit[75]&~bit[74]&bit[73]&~bit[72]) | (bit[75]&~bit[74]&~bit[73]&~bit[72]) | (~bit[76]&~bit[75]&bit[73]&bit[72]&bit[71]) | (bit[75]&bit[74]&bit[73]&bit[72]&bit[71]) | (bit[75]&bit[74]&bit[73]&~bit[72]&~bit[71]) | (~bit[75]&~bit[74]&~bit[73]&~bit[71]) | (~bit[74]&~bit[73]&bit[72]&bit[71]) | (bit[74]&~bit[73]&bit[72]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[76]&bit[75]&bit[74]&bit[73]&bit[72]) | (bit[75]&~bit[74]&~bit[73]&bit[72]&~bit[71]) | (bit[76]&bit[75]&~bit[74]&bit[73]&bit[72]&bit[71]) | (~bit[75]&bit[74]&~bit[73]&~bit[71]) | (~bit[75]&~bit[73]&bit[72]&bit[71]) | (~bit[75]&~bit[74]&bit[73]&~bit[71]) | (bit[76]&~bit[75]&bit[73]&bit[72]&bit[71]) | (~bit[75]&~bit[74]&~bit[73]&~bit[72]) | (bit[75]&bit[74]&bit[73]&~bit[72]&~bit[71]) | (~bit[74]&bit[73]&~bit[72]&~bit[71]) | (~bit[74]&~bit[72]&bit[71]) | (bit[74]&bit[73]&bit[72]&~bit[71]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[70]);
		control=control & bit[70];

		//-------------71--------------

		out[5] = (~disable_update & ((~bit[75]&~bit[74]&bit[73]&~bit[72]) | (bit[75]&bit[74]&bit[73]&bit[72]) | (bit[76]&bit[74]&bit[73]&bit[72]) | (bit[75]&~bit[74]&bit[73]&~bit[72]) | (~bit[76]&~bit[75]&~bit[72]) | (~bit[73]&~bit[72]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[76]&~bit[75]&~bit[73]&bit[72]) | (~bit[76]&bit[75]&bit[74]&bit[73]&~bit[72]) | (bit[76]&bit[75]&~bit[74]&~bit[73]&bit[72]) | (bit[76]&bit[75]&bit[74]&bit[73]&~bit[72]) | (bit[75]&bit[74]&bit[73]&bit[72]) | (bit[76]&~bit[75]&~bit[74]&~bit[73]) | (bit[76]&~bit[75]&bit[74]&bit[73]) | (~bit[76]&~bit[74]&~bit[73]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[76]&~bit[75]&bit[74]&~bit[73]&~bit[72]) | (~bit[76]&~bit[75]&~bit[74]&bit[72]) | (bit[76]&bit[75]&~bit[74]&~bit[73]&~bit[72]) | (bit[76]&bit[75]&bit[74]&bit[73]&~bit[72]) | (bit[76]&~bit[75]&bit[74]&~bit[73]) | (bit[75]&bit[74]&~bit[73]&bit[72]) | (bit[76]&~bit[75]&bit[74]&bit[73]) | (~bit[76]&bit[75]&bit[74]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[76]&~bit[75]&~bit[74]&~bit[73]&~bit[72]) | (~bit[76]&bit[75]&bit[74]&bit[73]&~bit[72]) | (bit[76]&bit[75]&~bit[74]&~bit[73]&~bit[72]) | (~bit[76]&bit[75]&~bit[74]&bit[72]) | (bit[76]&bit[75]&bit[74]&~bit[73]&~bit[72]) | (~bit[75]&~bit[74]&bit[73]&~bit[72]) | (bit[76]&bit[74]&bit[73]&bit[72]) | (bit[76]&~bit[75]&bit[74]&bit[73]) | (bit[76]&~bit[75]&bit[72]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[77]&bit[76]&bit[74]&bit[73]&bit[72]) | (~bit[77]&~bit[76]&bit[75]&bit[74]&bit[72]) | (~bit[76]&~bit[75]&bit[74]&~bit[73]&~bit[72]) | (bit[76]&bit[75]&~bit[74]&~bit[73]&bit[72]) | (bit[76]&~bit[75]&~bit[74]&bit[73]&bit[72]) | (bit[76]&bit[75]&~bit[74]&~bit[73]&~bit[72]) | (bit[76]&bit[75]&bit[74]&bit[73]&~bit[72]) | (~bit[76]&~bit[74]&bit[73]&~bit[72]) | (bit[76]&bit[75]&bit[74]&~bit[73]&~bit[72]) | (bit[75]&~bit[74]&bit[73]&~bit[72]) | (bit[75]&bit[74]&~bit[73]&bit[72]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[77]&bit[76]&bit[75]&bit[73]&bit[72]) | (~bit[77]&~bit[76]&bit[75]&bit[74]&bit[72]) | (~bit[76]&~bit[75]&~bit[74]&~bit[73]&~bit[72]) | (bit[76]&~bit[75]&~bit[74]&bit[73]&bit[72]) | (~bit[76]&bit[74]&~bit[73]&bit[72]) | (~bit[76]&bit[75]&~bit[74]&bit[73]) | (bit[76]&bit[75]&bit[74]&~bit[73]&~bit[72]) | (bit[75]&~bit[74]&bit[73]&bit[72]) | (bit[76]&~bit[75]&~bit[74]&~bit[73]) | (bit[76]&~bit[75]&bit[74]&~bit[73]) | (bit[76]&~bit[75]&~bit[72]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[71]);
		control=control & bit[71];

		//-------------72--------------

		out[5] = (~disable_update & ((~bit[78]&~bit[77]&bit[76]&bit[75]&bit[74]&bit[73]) | (~bit[78]&bit[77]&bit[75]&bit[74]&bit[73]) | (bit[78]&bit[76]&bit[75]&bit[74]&bit[73]) | (~bit[76]&~bit[75]&~bit[73]) | (bit[77]&~bit[76]&bit[75]&bit[74]&bit[73]) | (~bit[74]&~bit[73]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[78]&~bit[77]&bit[76]&bit[75]&bit[74]&bit[73]) | (~bit[77]&~bit[75]&~bit[74]) | (~bit[78]&bit[77]&bit[75]&bit[74]&bit[73]) | (bit[78]&bit[76]&bit[75]&bit[74]&bit[73]) | (~bit[76]&~bit[75]&~bit[74]&~bit[73]) | (~bit[76]&bit[75]&bit[74]&~bit[73]) | (~bit[75]&~bit[74]&bit[73]) | (bit[77]&~bit[76]&bit[75]&bit[74]&bit[73]) | (bit[76]&bit[75]&bit[74]&~bit[73]) | (bit[76]&~bit[75]&bit[74]&~bit[73]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[78]&~bit[77]&bit[76]&bit[75]&bit[74]&bit[73]) | (~bit[78]&bit[77]&bit[75]&bit[74]&bit[73]) | (bit[78]&bit[76]&bit[75]&bit[74]&bit[73]) | (bit[77]&bit[76]&~bit[75]&~bit[74]&~bit[73]) | (~bit[76]&bit[75]&bit[74]&~bit[73]) | (~bit[77]&bit[75]&~bit[74]) | (bit[77]&~bit[76]&bit[75]&bit[74]&bit[73]) | (bit[77]&~bit[76]&bit[75]&~bit[73]) | (bit[76]&~bit[75]&bit[74]&~bit[73]) | (~bit[76]&bit[75]&~bit[74]&bit[73]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[78]&~bit[77]&bit[76]&bit[75]&bit[74]&bit[73]) | (~bit[77]&~bit[76]&bit[75]&~bit[73]) | (bit[77]&bit[76]&bit[75]&~bit[74]) | (bit[77]&bit[76]&~bit[75]&~bit[74]&~bit[73]) | (~bit[77]&~bit[76]&bit[75]&~bit[74]) | (~bit[76]&~bit[75]&bit[73]) | (bit[77]&~bit[76]&bit[75]&bit[74]&bit[73]) | (bit[76]&bit[75]&bit[74]&~bit[73]) | (bit[76]&~bit[75]&bit[74]&~bit[73]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[78]&~bit[77]&bit[76]&bit[73]) | (~bit[77]&~bit[76]&~bit[75]&~bit[73]) | (~bit[77]&bit[76]&~bit[74]&bit[73]) | (bit[76]&~bit[75]&bit[74]&bit[73]) | (~bit[78]&bit[77]&bit[75]&bit[74]&bit[73]) | (bit[77]&bit[76]&bit[75]&~bit[74]) | (bit[77]&bit[76]&~bit[75]&~bit[74]&~bit[73]) | (~bit[77]&bit[76]&bit[74]&~bit[73]) | (~bit[77]&~bit[76]&bit[75]&~bit[74]) | (bit[77]&~bit[76]&bit[75]&bit[74]&bit[73]) | (bit[77]&~bit[76]&bit[75]&~bit[73]) | (bit[76]&bit[75]&bit[74]&~bit[73]) | (~bit[76]&bit[75]&~bit[74]&bit[73]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[78]&bit[77]&~bit[76]&bit[75]) | (~bit[78]&~bit[77]&bit[76]&bit[75]&bit[74]&bit[73]) | (~bit[77]&~bit[75]&bit[74]&bit[73]) | (~bit[77]&~bit[76]&~bit[74]&bit[73]) | (bit[78]&bit[76]&bit[75]&bit[74]&bit[73]) | (bit[77]&bit[76]&~bit[75]&~bit[74]&~bit[73]) | (~bit[76]&~bit[75]&~bit[74]&~bit[73]) | (~bit[77]&bit[76]&bit[74]&~bit[73]) | (bit[77]&~bit[76]&bit[75]&~bit[73]) | (bit[76]&~bit[75]&bit[74]&~bit[73]) | (~bit[76]&bit[75]&~bit[74]&bit[73]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[72]);
		control=control & bit[72];

		//-------------73--------------

		out[5] = (~disable_update & ((bit[77]&bit[76]&~bit[75]&~bit[74]) | (~bit[78]&bit[77]&~bit[76]&~bit[74]) | (bit[78]&~bit[76]&~bit[74]) | (~bit[77]&~bit[74]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[78]&~bit[77]&~bit[76]&bit[74]) | (bit[77]&bit[76]&bit[75]&~bit[74]) | (~bit[78]&~bit[77]&~bit[75]) | (~bit[76]&~bit[75]) | (~bit[75]&bit[74]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[78]&~bit[77]&~bit[76]&bit[75]&~bit[74]) | (bit[78]&~bit[77]&bit[76]&~bit[75]&~bit[74]) | (bit[77]&~bit[76]&bit[75]&bit[74]) | (~bit[77]&bit[76]&bit[75]&bit[74]) | (~bit[78]&~bit[76]&~bit[75]&bit[74]) | (~bit[78]&~bit[77]&~bit[76]&~bit[75]) | (bit[77]&bit[76]&bit[75]&~bit[74]) | (bit[78]&~bit[77]&~bit[76]&bit[75]&bit[74]) | (bit[77]&bit[76]&~bit[75]&~bit[74]) | (~bit[77]&~bit[76]&~bit[75]&bit[74]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[78]&bit[77]&~bit[76]&~bit[75]&bit[74]) | (bit[78]&~bit[77]&bit[76]&~bit[75]&~bit[74]) | (bit[78]&~bit[77]&~bit[76]&~bit[74]) | (~bit[77]&bit[76]&~bit[75]&bit[74]) | (bit[77]&bit[76]&bit[75]&~bit[74]) | (bit[78]&~bit[77]&~bit[76]&bit[75]&bit[74]) | (bit[77]&bit[76]&~bit[75]&~bit[74]) | (~bit[78]&bit[77]&~bit[76]&~bit[74]) | (bit[77]&bit[75]&bit[74]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[78]&bit[77]&~bit[76]&~bit[75]&bit[74]) | (~bit[78]&~bit[77]&bit[76]&bit[75]&bit[74]) | (~bit[78]&~bit[77]&~bit[76]&bit[75]&~bit[74]) | (~bit[78]&bit[77]&bit[75]&bit[74]) | (bit[78]&~bit[77]&bit[76]&~bit[75]&~bit[74]) | (~bit[78]&~bit[77]&~bit[75]&bit[74]) | (bit[78]&~bit[77]&~bit[76]&bit[75]&bit[74]) | (~bit[78]&bit[77]&bit[76]) | (bit[78]&~bit[76]&~bit[74]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[78]&~bit[77]&bit[76]&bit[75]&bit[74]) | (bit[78]&bit[77]&~bit[75]&bit[74]) | (~bit[78]&~bit[77]&bit[76]&~bit[74]) | (bit[78]&~bit[77]&~bit[75]&~bit[74]) | (bit[78]&~bit[77]&~bit[76]&bit[75]&bit[74]) | (~bit[78]&bit[77]&~bit[76]&~bit[74]) | (bit[77]&bit[75]&~bit[74]) | (~bit[77]&~bit[76]&~bit[75]&bit[74]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[73]);
		control=control & bit[73];

		//-------------74--------------

		out[5] = (~disable_update & ((~bit[79]&~bit[78]&bit[77]&~bit[75]) | (~bit[77]&bit[76]&~bit[75]) | (bit[77]&~bit[76]&~bit[75]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[79]&bit[78]&~bit[77]&~bit[76]&bit[75]) | (~bit[78]&~bit[77]&bit[76]&~bit[75]) | (~bit[79]&~bit[78]&~bit[77]&bit[75]) | (~bit[79]&bit[78]&bit[77]&bit[76]&~bit[75]) | (~bit[78]&bit[77]&~bit[76]&bit[75]) | (~bit[79]&~bit[77]&~bit[76]&bit[75]) | (bit[78]&bit[77]&~bit[76]&bit[75]) | (~bit[78]&~bit[77]&~bit[76]&bit[75]) | (bit[79]&bit[77]&bit[76]&~bit[75]) | (bit[77]&~bit[76]&~bit[75]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[79]&~bit[78]&~bit[77]&bit[76]&bit[75]) | (~bit[79]&bit[78]&bit[77]&bit[76]&~bit[75]) | (~bit[79]&~bit[77]&~bit[76]&bit[75]) | (bit[78]&~bit[77]&bit[76]) | (~bit[78]&~bit[77]&~bit[76]&bit[75]) | (bit[79]&bit[77]&bit[76]&~bit[75]) | (bit[77]&~bit[76]&~bit[75]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[79]&bit[78]&~bit[77]&~bit[76]&bit[75]) | (bit[79]&~bit[78]&~bit[77]&bit[76]&bit[75]) | (~bit[79]&~bit[78]&bit[77]&~bit[75]) | (~bit[78]&bit[77]&~bit[76]&bit[75]) | (~bit[78]&bit[77]&bit[76]&bit[75]) | (~bit[79]&bit[76]&~bit[75]) | (bit[79]&bit[77]&bit[76]&~bit[75]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[79]&bit[78]&~bit[77]&~bit[76]&bit[75]) | (~bit[79]&~bit[78]&~bit[77]&~bit[76]) | (bit[79]&~bit[78]&~bit[77]&bit[76]&bit[75]) | (~bit[79]&bit[78]&bit[77]&bit[76]&~bit[75]) | (~bit[79]&bit[77]&bit[76]&bit[75]) | (bit[79]&~bit[78]&~bit[75]) | (bit[78]&bit[77]&~bit[76]&bit[75]) | (~bit[78]&bit[77]&bit[76]&bit[75]) | (~bit[77]&bit[76]&~bit[75]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[79]&bit[78]&~bit[77]&~bit[75]) | (~bit[79]&bit[78]&~bit[76]&~bit[75]) | (bit[78]&bit[77]&bit[76]&bit[75]) | (~bit[79]&bit[78]&bit[76]&bit[75]) | (~bit[79]&bit[77]&bit[75]) | (~bit[78]&~bit[77]&~bit[76]&bit[75]) | (bit[79]&bit[77]&bit[76]&~bit[75]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[74]);
		control=control & bit[74];

		//-------------75--------------

		out[6] = (~disable_update & ((~bit[80]&~bit[79]&~bit[78]&~bit[77]&~bit[76]))) | (disable_update & out[6]);
		out[5] = (~disable_update & ((bit[79]&~bit[78]&~bit[77]&bit[76]) | (~bit[80]&~bit[78]&~bit[77]&bit[76]) | (bit[80]&~bit[78]&bit[77]&~bit[76]) | (bit[80]&~bit[79]&~bit[78]&~bit[77]) | (~bit[79]&bit[78]&~bit[77]) | (bit[79]&~bit[78]&~bit[77]&~bit[76]) | (~bit[80]&~bit[78]&bit[77]&~bit[76]) | (bit[78]&~bit[76]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[80]&~bit[78]&bit[77]&bit[76]) | (~bit[80]&~bit[78]&bit[77]&bit[76]) | (bit[80]&~bit[79]&~bit[78]&~bit[76]) | (~bit[80]&~bit[79]&bit[77]) | (bit[80]&~bit[78]&bit[77]&~bit[76]) | (~bit[79]&bit[78]&~bit[77]&~bit[76]) | (bit[79]&~bit[78]&~bit[77]&~bit[76]) | (bit[79]&bit[78]&~bit[77]) | (~bit[80]&~bit[78]&bit[77]&~bit[76]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[79]&bit[78]&bit[77]&~bit[76]) | (~bit[80]&bit[79]&bit[78]&bit[76]) | (bit[80]&~bit[79]&bit[78]&bit[77]) | (bit[80]&~bit[79]&~bit[78]&~bit[77]) | (~bit[79]&~bit[78]&bit[76]) | (~bit[79]&bit[78]&~bit[77]&~bit[76]) | (bit[79]&~bit[78]&~bit[77]&~bit[76]) | (bit[79]&bit[78]&~bit[77]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[80]&bit[78]&~bit[77]&bit[76]) | (bit[79]&bit[78]&bit[77]&~bit[76]) | (bit[80]&bit[78]&bit[77]) | (bit[80]&~bit[79]&~bit[78]&~bit[76]) | (bit[79]&~bit[78]&bit[76]) | (bit[79]&~bit[78]&~bit[77]&~bit[76]) | (~bit[80]&~bit[78]&bit[77]&~bit[76]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[79]&~bit[78]&bit[77]&~bit[76]) | (~bit[80]&bit[79]&~bit[78]&~bit[77]) | (~bit[80]&bit[79]&bit[78]&bit[76]) | (bit[80]&bit[78]&~bit[77]&bit[76]) | (bit[79]&~bit[78]&~bit[77]&bit[76]) | (~bit[80]&~bit[78]&bit[77]&bit[76]) | (bit[80]&~bit[79]&bit[78]&bit[77]) | (bit[80]&~bit[79]&~bit[78]&~bit[76]) | (~bit[80]&~bit[78]&~bit[77]&bit[76]) | (bit[80]&~bit[78]&bit[77]&~bit[76]) | (~bit[79]&bit[78]&~bit[77]&~bit[76]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[80]&bit[79]&~bit[78]&~bit[77]&bit[76]) | (~bit[79]&bit[78]&bit[76]) | (bit[80]&~bit[78]&bit[77]&bit[76]) | (bit[80]&bit[78]&~bit[77]&bit[76]) | (bit[80]&~bit[79]&bit[78]&bit[77]) | (bit[80]&~bit[79]&~bit[78]&~bit[77]) | (~bit[80]&bit[78]&~bit[76]) | (~bit[80]&bit[78]&bit[77]) | (~bit[80]&~bit[78]&bit[77]&~bit[76]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[75]);
		control=control & bit[75];

		//-------------76--------------

		out[6] = (~disable_update & (out[6]&0)) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((~bit[81]&bit[80]&~bit[79]&~bit[78]&bit[77]) | (~bit[81]&~bit[80]&~bit[78]&bit[77]) | (~bit[81]&bit[80]&~bit[79]&bit[78]&~bit[77]) | (bit[81]&~bit[79]&~bit[78]&bit[77]) | (bit[81]&bit[80]&~bit[79]&bit[78]&~bit[77]) | (~bit[80]&~bit[79]&bit[78]&~bit[77]) | (bit[80]&bit[79]&~bit[78]&~bit[77]) | (bit[79]&bit[78]&~bit[77]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[81]&~bit[80]&bit[79]&~bit[78]&bit[77]) | (bit[81]&bit[80]&bit[79]&~bit[78]&bit[77]) | (~bit[81]&bit[80]&~bit[79]&bit[78]&~bit[77]) | (bit[81]&~bit[79]&bit[78]&bit[77]) | (~bit[81]&~bit[80]&bit[78]&bit[77]) | (~bit[80]&~bit[79]&bit[78]&~bit[77]) | (bit[80]&bit[79]&~bit[78]&~bit[77]) | (~bit[81]&~bit[79]&bit[78]&bit[77]) | (~bit[81]&bit[80]&bit[79]&~bit[78]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[81]&~bit[80]&~bit[79]&~bit[78]&bit[77]) | (bit[81]&~bit[80]&bit[79]&~bit[78]&bit[77]) | (~bit[81]&~bit[80]&bit[78]&~bit[77]) | (bit[81]&bit[80]&~bit[79]&bit[78]&~bit[77]) | (bit[81]&bit[79]&bit[78]&bit[77]) | (bit[80]&bit[79]&~bit[78]&~bit[77]) | (~bit[81]&bit[80]&bit[79]&bit[78]) | (~bit[81]&bit[80]&bit[79]&~bit[78]) | (bit[79]&bit[78]&~bit[77]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[81]&bit[80]&bit[79]&~bit[78]&~bit[77]) | (~bit[81]&bit[80]&~bit[79]&~bit[78]&bit[77]) | (~bit[81]&~bit[80]&bit[79]&bit[78]&~bit[77]) | (bit[81]&bit[80]&bit[79]&~bit[78]&bit[77]) | (bit[81]&bit[80]&~bit[79]&bit[78]&~bit[77]) | (bit[81]&~bit[80]&bit[77]) | (~bit[81]&~bit[79]&bit[78]&bit[77]) | (bit[81]&~bit[80]&bit[78]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[81]&~bit[80]&~bit[79]&bit[78]&bit[77]) | (~bit[81]&~bit[80]&bit[79]&bit[78]&~bit[77]) | (~bit[80]&~bit[79]&~bit[78]&~bit[77]) | (bit[81]&bit[80]&bit[79]&~bit[78]&bit[77]) | (~bit[81]&bit[80]&~bit[79]&bit[78]&~bit[77]) | (bit[81]&~bit[79]&~bit[78]&bit[77]) | (bit[81]&bit[80]&~bit[79]&bit[78]&~bit[77]) | (bit[81]&bit[79]&bit[78]&bit[77]) | (~bit[81]&bit[80]&bit[79]&bit[78]) | (~bit[81]&bit[80]&bit[79]&~bit[78]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[81]&bit[80]&bit[79]&~bit[78]&bit[77]) | (bit[81]&~bit[79]&bit[78]&bit[77]) | (~bit[81]&bit[80]&bit[79]&bit[78]) | (bit[80]&~bit[77]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[76]);
		control=control & bit[76];

		//-------------77--------------

		out[6] = (~disable_update & ((~bit[83]&~bit[82]&bit[81]&bit[80]&bit[79]&bit[78]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((~bit[82]&~bit[81]&~bit[80]&~bit[79]&bit[78]) | (bit[82]&~bit[81]&bit[80]&bit[79]&~bit[78]) | (bit[83]&~bit[82]&bit[81]&bit[80]&bit[79]&bit[78]) | (~bit[82]&bit[81]&~bit[80]&bit[79]&~bit[78]) | (~bit[82]&bit[81]&bit[80]&bit[79]&~bit[78]) | (bit[82]&bit[81]&bit[80]&bit[79]&bit[78]) | (bit[82]&bit[81]&bit[79]&~bit[78]) | (~bit[81]&~bit[80]&bit[79]&~bit[78]) | (~bit[82]&~bit[81]&bit[79]&~bit[78]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[82]&bit[81]&~bit[80]&~bit[79]&bit[78]) | (bit[83]&~bit[82]&bit[81]&bit[80]&bit[79]&bit[78]) | (~bit[82]&bit[81]&~bit[80]&bit[79]&~bit[78]) | (bit[82]&~bit[80]&~bit[79]&bit[78]) | (bit[82]&bit[81]&bit[80]&bit[79]&bit[78]) | (bit[80]&~bit[79]&bit[78]) | (~bit[81]&~bit[80]&bit[79]&~bit[78]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[82]&bit[81]&~bit[80]&bit[79]) | (bit[82]&~bit[81]&bit[80]&bit[79]&~bit[78]) | (bit[83]&~bit[82]&bit[81]&bit[80]&bit[79]&bit[78]) | (bit[82]&~bit[81]&~bit[80]&bit[78]) | (bit[82]&bit[81]&bit[80]&bit[79]&bit[78]) | (~bit[82]&~bit[80]&bit[79]&bit[78]) | (bit[81]&~bit[80]&bit[78]) | (~bit[82]&~bit[81]&bit[79]&~bit[78]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[82]&bit[81]&~bit[80]&~bit[79]&bit[78]) | (bit[83]&~bit[82]&bit[81]&bit[80]&bit[79]&bit[78]) | (~bit[82]&bit[81]&bit[80]&bit[79]&~bit[78]) | (bit[82]&~bit[81]&~bit[80]&bit[78]) | (bit[82]&bit[81]&bit[80]&bit[79]&bit[78]) | (~bit[82]&~bit[81]&bit[80]&bit[78]) | (~bit[81]&~bit[79]&~bit[78]) | (~bit[82]&~bit[81]&bit[79]&bit[78]) | (bit[82]&bit[81]&bit[79]&~bit[78]) | (~bit[81]&~bit[80]&bit[79]&~bit[78]) | (~bit[82]&~bit[81]&bit[79]&~bit[78]) | (~bit[80]&~bit[79]&~bit[78]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[81]&~bit[80]&~bit[79]&~bit[78]) | (bit[82]&~bit[81]&bit[80]&bit[79]&~bit[78]) | (bit[83]&~bit[82]&bit[81]&bit[80]&bit[79]&bit[78]) | (~bit[81]&bit[80]&~bit[79]&bit[78]) | (~bit[82]&bit[81]&bit[80]&bit[79]&~bit[78]) | (bit[82]&~bit[80]&bit[79]&~bit[78]) | (~bit[82]&~bit[81]&bit[80]&bit[78]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[82]&bit[81]&bit[80]&~bit[79]&bit[78]) | (~bit[83]&bit[80]&bit[79]&bit[78]) | (bit[82]&~bit[81]&bit[80]&bit[78]) | (~bit[82]&bit[81]&~bit[80]&bit[79]&~bit[78]) | (bit[82]&~bit[80]&~bit[79]&bit[78]) | (bit[82]&~bit[80]&bit[79]&~bit[78]) | (~bit[82]&~bit[80]&bit[79]&bit[78]) | (~bit[82]&~bit[81]&bit[79]&bit[78]) | (bit[82]&bit[81]&bit[79]&~bit[78]) | (~bit[80]&~bit[79]&~bit[78]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[77]);
		control=control & bit[77];

		//-------------78--------------

		out[6] = (~disable_update & ((~bit[84]&~bit[83]&bit[82]&bit[81]&bit[80]&bit[79]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((bit[83]&bit[82]&bit[81]&bit[80]&bit[79]) | (bit[84]&~bit[83]&bit[82]&bit[81]&bit[80]&bit[79]) | (bit[83]&bit[82]&~bit[80]&~bit[79]) | (~bit[82]&bit[81]&~bit[80]&~bit[79]) | (~bit[83]&~bit[82]&~bit[79]) | (~bit[83]&~bit[80]&~bit[79]) | (~bit[81]&~bit[79]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[83]&bit[82]&bit[81]&bit[80]&bit[79]) | (bit[84]&~bit[83]&bit[82]&bit[81]&bit[80]&bit[79]) | (bit[83]&~bit[82]&bit[81]&bit[80]&~bit[79]) | (~bit[82]&bit[81]&~bit[80]&~bit[79]) | (~bit[83]&~bit[82]&~bit[80]) | (bit[82]&bit[81]&bit[80]&~bit[79]) | (~bit[83]&~bit[80]&~bit[79]) | (~bit[81]&~bit[80]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[83]&~bit[82]&bit[81]&~bit[80]&bit[79]) | (bit[84]&~bit[83]&bit[82]&bit[81]&bit[80]&bit[79]) | (~bit[83]&bit[82]&bit[81]&~bit[80]&bit[79]) | (bit[83]&~bit[82]&bit[81]&bit[80]&~bit[79]) | (bit[82]&bit[81]&bit[80]&~bit[79]) | (~bit[83]&~bit[81]&~bit[79]) | (bit[83]&bit[82]&bit[81]) | (~bit[82]&~bit[81]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[83]&~bit[82]&~bit[81]&bit[80]&~bit[79]) | (bit[83]&~bit[82]&bit[81]&~bit[80]&bit[79]) | (bit[84]&~bit[83]&bit[82]&bit[81]&bit[80]&bit[79]) | (bit[82]&~bit[81]&bit[80]&bit[79]) | (~bit[83]&bit[82]&bit[81]&~bit[80]&bit[79]) | (bit[83]&bit[82]&~bit[81]&~bit[79]) | (bit[83]&~bit[82]&bit[81]&bit[80]&~bit[79]) | (~bit[82]&bit[81]&~bit[80]&~bit[79]) | (bit[82]&bit[81]&bit[80]&~bit[79]) | (bit[83]&bit[82]&bit[81]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[84]&bit[83]&bit[81]&bit[80]&bit[79]) | (bit[83]&~bit[82]&bit[81]&~bit[80]&bit[79]) | (bit[84]&~bit[83]&bit[82]&bit[81]&bit[80]&bit[79]) | (bit[83]&bit[82]&~bit[81]&~bit[79]) | (bit[83]&bit[82]&~bit[80]&~bit[79]) | (~bit[83]&~bit[81]&bit[79]) | (~bit[83]&~bit[82]&~bit[81]) | (~bit[82]&bit[80]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[84]&~bit[83]&bit[82]&bit[81]&bit[80]&bit[79]) | (~bit[83]&~bit[82]&bit[81]&~bit[80]&~bit[79]) | (bit[84]&~bit[83]&bit[82]&bit[81]&bit[80]&bit[79]) | (bit[83]&~bit[81]&~bit[80]&~bit[79]) | (bit[82]&~bit[81]&~bit[80]&bit[79]) | (~bit[83]&bit[82]&bit[81]&~bit[80]&bit[79]) | (~bit[83]&~bit[82]&bit[80]&bit[79]) | (~bit[83]&bit[82]&bit[80]&~bit[79]) | (bit[83]&~bit[82]&bit[81]&bit[80]&~bit[79]) | (bit[83]&bit[82]&~bit[80]&~bit[79]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[78]);
		control=control & bit[78];

		//-------------79--------------

		out[6] = (~disable_update & ((bit[84]&bit[83]&~bit[82]&bit[81]&bit[80]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((~bit[84]&bit[83]&~bit[82]&bit[81]&~bit[80]) | (bit[84]&bit[83]&~bit[82]&~bit[80]) | (~bit[84]&~bit[83]&bit[82]&~bit[80]) | (~bit[83]&~bit[82]&bit[81]&~bit[80]) | (bit[83]&bit[82]&~bit[81]&~bit[80]) | (~bit[84]&~bit[82]&~bit[81]&~bit[80]) | (~bit[83]&~bit[81]&~bit[80]) | (bit[82]&bit[81]&bit[80]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[84]&~bit[82]&~bit[81]&bit[80]) | (bit[84]&~bit[83]&bit[82]&bit[81]&~bit[80]) | (~bit[84]&~bit[83]&~bit[81]&bit[80]) | (bit[84]&~bit[82]&~bit[81]) | (bit[83]&bit[82]&bit[81]&~bit[80]) | (~bit[84]&~bit[82]&~bit[81]&~bit[80]) | (~bit[83]&~bit[81]&~bit[80]) | (bit[82]&bit[81]&bit[80]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[85]&~bit[83]&bit[82]&bit[81]&bit[80]) | (~bit[85]&~bit[83]&bit[82]&bit[81]&bit[80]) | (bit[84]&~bit[83]&bit[82]&~bit[81]&bit[80]) | (~bit[84]&~bit[82]&~bit[81]&bit[80]) | (~bit[83]&~bit[82]&bit[81]&~bit[80]) | (~bit[84]&~bit[83]&~bit[82]&~bit[81]) | (bit[84]&~bit[83]&bit[82]&bit[81]&~bit[80]) | (~bit[83]&~bit[82]&~bit[81]&bit[80]) | (bit[83]&bit[82]&~bit[81]&~bit[80]) | (bit[83]&bit[82]&bit[81]&~bit[80]) | (~bit[84]&bit[83]&bit[82]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[84]&bit[83]&bit[82]) | (bit[84]&~bit[83]&~bit[82]&~bit[81]&~bit[80]) | (~bit[84]&bit[83]&~bit[82]&bit[81]&~bit[80]) | (~bit[85]&~bit[83]&bit[82]&bit[81]&bit[80]) | (~bit[84]&~bit[83]&bit[81]&bit[80]) | (bit[84]&~bit[83]&bit[82]&~bit[81]&bit[80]) | (~bit[83]&~bit[82]&bit[81]&bit[80]) | (bit[84]&bit[83]&~bit[81]&bit[80]) | (bit[84]&~bit[83]&bit[82]&bit[81]&~bit[80]) | (bit[83]&bit[82]&~bit[81]&~bit[80]) | (bit[83]&bit[82]&bit[81]&~bit[80]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[85]&~bit[84]&bit[82]&bit[81]&bit[80]) | (bit[85]&~bit[83]&bit[82]&bit[81]&bit[80]) | (~bit[85]&bit[84]&bit[83]&bit[80]) | (~bit[84]&bit[83]&bit[82]&~bit[80]) | (bit[84]&bit[83]&~bit[82]&~bit[80]) | (bit[84]&~bit[83]&bit[82]&~bit[81]&bit[80]) | (bit[84]&bit[83]&~bit[81]&bit[80]) | (bit[84]&~bit[83]&bit[82]&bit[81]&~bit[80]) | (~bit[84]&~bit[83]&~bit[81]&bit[80]) | (~bit[83]&~bit[82]&~bit[81]&bit[80]) | (~bit[84]&~bit[83]&~bit[82]) | (~bit[84]&~bit[82]&~bit[81]&~bit[80]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[84]&~bit[83]&~bit[82]&~bit[81]&~bit[80]) | (bit[85]&bit[83]&bit[82]&bit[80]) | (~bit[85]&~bit[83]&bit[82]&bit[81]&bit[80]) | (~bit[84]&bit[83]&~bit[82]&bit[81]) | (~bit[84]&~bit[83]&bit[82]&~bit[80]) | (~bit[84]&~bit[83]&~bit[82]&~bit[81]) | (bit[83]&bit[82]&~bit[81]) | (bit[84]&~bit[83]&bit[81]) | (~bit[84]&~bit[82]&~bit[81]&~bit[80]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[79]);
		control=control & bit[79];

		//-------------80--------------

		out[6] = (~disable_update & ((~bit[86]&bit[85]&bit[84]&bit[83]&bit[82]&bit[81]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((bit[86]&bit[85]&bit[84]&bit[83]&bit[82]&bit[81]) | (~bit[82]&~bit[81]) | (~bit[83]&~bit[81]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[86]&bit[85]&bit[84]&bit[83]&bit[82]&bit[81]) | (bit[83]&bit[82]&~bit[81]) | (~bit[85]&~bit[83]&~bit[82]) | (bit[85]&~bit[84]&~bit[83]&~bit[82]) | (~bit[82]&bit[81]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[86]&bit[85]&bit[84]&bit[83]&bit[82]&bit[81]) | (bit[85]&~bit[84]&~bit[83]&bit[82]&bit[81]) | (~bit[85]&~bit[84]&~bit[83]&bit[81]) | (bit[85]&bit[84]&~bit[83]&~bit[82]&~bit[81]) | (~bit[85]&bit[83]&~bit[81]) | (bit[85]&bit[83]&~bit[81]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[85]&~bit[83]&~bit[82]&bit[81]) | (bit[86]&bit[85]&bit[84]&bit[83]&bit[82]&bit[81]) | (~bit[85]&bit[84]&~bit[83]&bit[81]) | (bit[85]&bit[84]&~bit[83]&~bit[82]&~bit[81]) | (bit[85]&bit[84]&~bit[83]&bit[81]) | (~bit[84]&bit[82]&~bit[81]) | (~bit[85]&~bit[84]&~bit[81]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[86]&bit[85]&bit[84]&bit[83]&bit[82]&bit[81]) | (~bit[85]&bit[84]&~bit[83]&bit[81]) | (bit[85]&bit[84]&~bit[83]&~bit[82]&~bit[81]) | (~bit[84]&bit[83]&~bit[82]) | (~bit[85]&~bit[84]&bit[82]) | (bit[85]&~bit[84]&~bit[83]&~bit[82]) | (~bit[85]&bit[83]&~bit[81]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[85]&bit[84]&~bit[83]&bit[82]&~bit[81]) | (bit[86]&bit[85]&bit[84]&bit[83]&bit[82]&bit[81]) | (bit[85]&~bit[84]&~bit[83]&bit[82]&bit[81]) | (~bit[85]&bit[83]&~bit[82]&bit[81]) | (bit[85]&bit[84]&~bit[83]&~bit[82]&~bit[81]) | (bit[85]&bit[84]&~bit[83]&bit[81]) | (bit[85]&~bit[84]&~bit[83]&~bit[82]) | (~bit[84]&bit[83]&bit[82]) | (bit[85]&bit[83]&~bit[81]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[80]);
		control=control & bit[80];

		//-------------81--------------

		out[6] = (~disable_update & (out[6]&0)) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((bit[86]&~bit[85]&bit[84]&~bit[82]) | (bit[86]&bit[85]&bit[84]&~bit[83]&~bit[82]) | (~bit[86]&~bit[82]) | (~bit[84]&~bit[82]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[86]&bit[85]&bit[84]&bit[83]&~bit[82]) | (~bit[86]&bit[85]&bit[84]&~bit[83]) | (~bit[85]&bit[84]&~bit[83]) | (~bit[83]&bit[82]) | (~bit[84]&~bit[83]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[86]&bit[85]&bit[84]&bit[83]&~bit[82]) | (bit[86]&bit[85]&~bit[84]&bit[83]) | (bit[86]&bit[85]&bit[84]&~bit[83]&~bit[82]) | (bit[86]&~bit[85]&~bit[84]&~bit[83]) | (~bit[85]&~bit[84]&bit[83]) | (~bit[86]&~bit[84]) | (~bit[84]&~bit[82]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[86]&bit[85]&~bit[84]&~bit[83]&bit[82]) | (bit[86]&bit[85]&bit[84]&~bit[83]&~bit[82]) | (~bit[85]&bit[84]&~bit[83]) | (~bit[86]&bit[83]&bit[82]) | (~bit[85]&~bit[84]&bit[83]) | (bit[84]&bit[83]) | (~bit[86]&~bit[85]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[86]&bit[85]&bit[84]&~bit[83]&bit[82]) | (bit[86]&~bit[85]&~bit[84]&~bit[83]&~bit[82]) | (bit[86]&bit[85]&~bit[84]&~bit[83]&bit[82]) | (bit[86]&bit[85]&bit[84]&bit[83]&~bit[82]) | (~bit[86]&bit[85]&~bit[84]&~bit[82]) | (~bit[86]&~bit[85]&bit[84]&~bit[82]) | (~bit[86]&~bit[85]&~bit[84]&bit[82]) | (bit[86]&bit[85]&bit[84]&~bit[83]&~bit[82]) | (~bit[85]&bit[83]&bit[82]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[86]&bit[85]&bit[84]&bit[83]&~bit[82]) | (bit[86]&~bit[85]&bit[84]&~bit[82]) | (~bit[86]&~bit[85]&~bit[84]&bit[83]) | (bit[86]&bit[85]&~bit[84]&bit[83]) | (~bit[86]&bit[85]&bit[84]&~bit[83]) | (bit[86]&bit[85]&bit[84]&~bit[83]&~bit[82]) | (bit[86]&~bit[85]&~bit[84]&~bit[83]) | (~bit[86]&bit[84]&bit[82]) | (~bit[86]&bit[83]&bit[82]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[81]);
		control=control & bit[81];

		//-------------82--------------

		out[6] = (~disable_update & ((~bit[88]&bit[86]&bit[85]&bit[84]&bit[83]) | (~bit[87]&bit[86]&bit[85]&bit[84]&bit[83]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((bit[88]&bit[87]&bit[86]&bit[85]&bit[84]&bit[83]) | (bit[87]&~bit[86]&bit[84]&~bit[83]) | (~bit[87]&bit[85]&~bit[84]&~bit[83]) | (~bit[87]&bit[85]&bit[84]&~bit[83]) | (bit[87]&bit[86]&~bit[85]&~bit[83]) | (~bit[87]&~bit[85]&~bit[83]) | (bit[87]&~bit[84]&~bit[83]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[88]&bit[87]&bit[86]&bit[85]&bit[84]&bit[83]) | (~bit[87]&~bit[86]&bit[85]&~bit[84]) | (~bit[87]&~bit[85]&~bit[84]&bit[83]) | (bit[87]&bit[86]&bit[85]&bit[84]&~bit[83]) | (bit[87]&~bit[85]&~bit[84]) | (~bit[84]&~bit[83]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[88]&bit[87]&bit[86]&bit[85]&bit[84]&bit[83]) | (~bit[87]&bit[86]&bit[85]&~bit[84]&bit[83]) | (bit[87]&bit[85]&~bit[84]&bit[83]) | (bit[87]&bit[86]&bit[85]&bit[84]&~bit[83]) | (~bit[87]&~bit[85]&~bit[83]) | (~bit[86]&~bit[85]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[87]&~bit[86]&bit[85]&~bit[84]) | (bit[88]&bit[87]&bit[86]&bit[85]&bit[84]&bit[83]) | (bit[86]&~bit[85]&bit[84]&bit[83]) | (~bit[87]&bit[86]&bit[85]&~bit[84]&bit[83]) | (~bit[87]&~bit[86]&bit[85]&bit[84]) | (~bit[87]&~bit[85]&~bit[84]&bit[83]) | (~bit[87]&~bit[86]&~bit[83]) | (bit[87]&bit[86]&~bit[85]&~bit[83]) | (bit[87]&bit[86]&bit[85]&bit[84]&~bit[83]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[88]&~bit[87]&bit[86]&bit[84]&bit[83]) | (bit[88]&bit[87]&bit[86]&bit[85]&bit[84]&bit[83]) | (~bit[87]&~bit[86]&bit[85]&~bit[84]) | (bit[87]&~bit[86]&bit[84]&~bit[83]) | (~bit[87]&bit[85]&~bit[84]&~bit[83]) | (bit[87]&bit[85]&~bit[84]&bit[83]) | (~bit[86]&~bit[85]&~bit[83]) | (bit[87]&bit[86]&~bit[85]&~bit[83]) | (bit[87]&bit[86]&bit[85]&bit[84]&~bit[83]) | (~bit[87]&~bit[85]&bit[84]) | (bit[87]&~bit[85]&~bit[84]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[87]&bit[86]&bit[85]&bit[84]&bit[83]) | (bit[88]&bit[87]&bit[86]&bit[85]&bit[84]&bit[83]) | (~bit[87]&bit[86]&bit[85]&~bit[84]&bit[83]) | (~bit[86]&~bit[85]&bit[84]&~bit[83]) | (~bit[87]&bit[85]&bit[84]&~bit[83]) | (~bit[87]&~bit[85]&~bit[84]&bit[83]) | (bit[87]&bit[86]&~bit[85]) | (bit[87]&bit[85]&~bit[84]&bit[83]) | (bit[87]&bit[86]&bit[85]&bit[84]&~bit[83]) | (bit[87]&~bit[84]&~bit[83]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[82]);
		control=control & bit[82];

		//-------------83--------------

		out[6] = (~disable_update & ((bit[88]&~bit[87]&bit[86]&bit[85]&bit[84]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((bit[87]&bit[86]&bit[85]&bit[84]) | (bit[86]&~bit[85]&~bit[84]) | (~bit[86]&~bit[84]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[88]&~bit[86]&~bit[85]) | (bit[88]&~bit[87]&~bit[86]&~bit[85]) | (bit[87]&bit[86]&bit[85]&bit[84]) | (~bit[86]&~bit[85]&bit[84]) | (bit[86]&bit[85]&~bit[84]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[88]&bit[87]&~bit[86]&~bit[85]&~bit[84]) | (~bit[87]&bit[86]&~bit[84]) | (~bit[87]&bit[86]&~bit[85]&bit[84]) | (bit[87]&bit[86]&~bit[85]&bit[84]) | (~bit[88]&bit[87]&bit[86]) | (bit[86]&bit[85]&~bit[84]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[89]&~bit[88]&bit[87]&bit[86]&bit[85]&bit[84]) | (bit[88]&bit[87]&~bit[86]&~bit[85]&~bit[84]) | (bit[88]&bit[87]&bit[86]&bit[85]&bit[84]) | (bit[88]&bit[87]&bit[86]&~bit[85]&~bit[84]) | (~bit[87]&~bit[86]&bit[84]) | (~bit[87]&bit[86]&~bit[85]&bit[84]) | (~bit[87]&bit[85]&~bit[84]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[89]&~bit[88]&bit[87]&bit[86]&bit[85]&bit[84]) | (~bit[88]&~bit[87]&bit[86]&~bit[84]) | (bit[88]&~bit[87]&bit[86]&bit[85]&bit[84]) | (bit[88]&bit[87]&bit[86]&~bit[85]&~bit[84]) | (bit[87]&~bit[86]&bit[85]&bit[84]) | (~bit[88]&bit[87]&~bit[85]&bit[84]) | (~bit[88]&~bit[87]&~bit[85]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[89]&~bit[88]&bit[87]&bit[86]&bit[85]&bit[84]) | (~bit[89]&bit[88]&bit[86]&bit[85]&bit[84]) | (~bit[88]&bit[87]&bit[85]&~bit[84]) | (bit[88]&~bit[87]&~bit[86]&~bit[85]) | (bit[88]&bit[86]&~bit[85]&~bit[84]) | (~bit[88]&bit[86]&~bit[85]&bit[84]) | (~bit[88]&bit[87]&~bit[85]&bit[84]) | (~bit[88]&~bit[86]&bit[85]) | (bit[87]&bit[86]&~bit[85]&bit[84]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[83]);
		control=control & bit[83];

		//-------------84--------------

		out[6] = (~disable_update & ((~bit[90]&~bit[89]&bit[88]&bit[87]&bit[86]&bit[85]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((bit[90]&~bit[89]&bit[88]&bit[87]&bit[86]&bit[85]) | (~bit[89]&~bit[88]&~bit[87]&~bit[85]) | (bit[89]&bit[88]&bit[87]&bit[86]&bit[85]) | (bit[88]&~bit[87]&~bit[86]&~bit[85]) | (bit[87]&~bit[86]&~bit[85]) | (~bit[88]&~bit[86]&~bit[85]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[90]&~bit[89]&bit[88]&bit[87]&bit[86]&bit[85]) | (~bit[88]&bit[87]&bit[86]&~bit[85]) | (~bit[89]&~bit[87]&~bit[86]&bit[85]) | (bit[89]&bit[88]&bit[87]&bit[86]&bit[85]) | (bit[88]&~bit[87]&bit[86]&~bit[85]) | (bit[89]&~bit[88]&bit[86]&~bit[85]) | (bit[88]&bit[87]&bit[86]&~bit[85]) | (~bit[88]&~bit[87]&~bit[86]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[90]&~bit[89]&bit[88]&bit[87]&bit[86]&bit[85]) | (~bit[88]&bit[87]&bit[86]&~bit[85]) | (bit[89]&bit[88]&bit[87]&bit[86]&bit[85]) | (~bit[88]&bit[87]&~bit[86]&bit[85]) | (~bit[89]&bit[87]&~bit[86]&bit[85]) | (bit[88]&~bit[87]&bit[86]&~bit[85]) | (bit[89]&~bit[88]&bit[86]&~bit[85]) | (bit[89]&bit[88]&~bit[87]&~bit[86]) | (bit[88]&~bit[87]&~bit[86]&~bit[85]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[88]&~bit[87]&bit[86]&bit[85]) | (~bit[89]&~bit[88]&bit[87]&~bit[86]) | (bit[89]&bit[88]&bit[87]&bit[86]&bit[85]) | (bit[89]&~bit[88]&~bit[87]&bit[86]) | (bit[89]&bit[88]&~bit[86]&bit[85]) | (~bit[89]&bit[88]&~bit[85]) | (~bit[89]&bit[88]&bit[86]) | (bit[87]&~bit[86]&~bit[85]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[90]&~bit[89]&bit[88]&bit[87]&bit[86]&bit[85]) | (bit[89]&bit[88]&~bit[87]&bit[86]&bit[85]) | (bit[89]&bit[88]&~bit[87]&~bit[85]) | (bit[89]&bit[87]&~bit[86]&bit[85]) | (~bit[89]&~bit[88]&~bit[87]&~bit[86]) | (~bit[89]&~bit[88]&bit[86]&bit[85]) | (bit[88]&~bit[87]&~bit[86]&~bit[85]) | (bit[88]&bit[87]&bit[86]&~bit[85]) | (~bit[88]&~bit[86]&~bit[85]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[89]&bit[88]&~bit[87]&bit[86]&bit[85]) | (~bit[89]&~bit[88]&~bit[86]&~bit[85]) | (bit[89]&~bit[88]&~bit[87]&bit[86]) | (bit[89]&bit[88]&~bit[87]&~bit[86]) | (~bit[90]&bit[89]&bit[85]) | (bit[89]&bit[88]&~bit[86]&bit[85]) | (bit[88]&bit[87]&bit[86]&~bit[85]) | (~bit[89]&bit[87]&~bit[85]) | (bit[89]&~bit[88]&bit[85]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[84]);
		control=control & bit[84];

		//-------------85--------------

		out[6] = (~disable_update & (out[6]&0)) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((bit[90]&~bit[89]&~bit[88]&bit[87]&~bit[86]) | (~bit[90]&~bit[89]&~bit[88]&~bit[87]) | (~bit[90]&~bit[89]&bit[88]&~bit[87]&~bit[86]) | (~bit[89]&bit[88]&bit[87]&~bit[86]) | (~bit[90]&bit[89]&bit[88]&~bit[86]) | (bit[90]&bit[88]&~bit[87]&~bit[86]) | (bit[90]&bit[89]&~bit[88]&~bit[87]&~bit[86]) | (~bit[90]&~bit[89]&~bit[88]&~bit[86]) | (~bit[90]&~bit[88]&~bit[87]&~bit[86]) | (bit[89]&bit[87]&~bit[86]) | (~bit[89]&~bit[88]&~bit[87]&~bit[86]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[89]&~bit[88]&bit[87]&bit[86]) | (bit[90]&~bit[89]&~bit[88]&bit[86]) | (bit[90]&bit[89]&~bit[88]&~bit[87]&~bit[86]) | (~bit[90]&~bit[89]&~bit[88]&~bit[86]) | (bit[89]&~bit[88]&~bit[87]&bit[86]) | (~bit[90]&~bit[88]&~bit[87]&~bit[86]) | (~bit[89]&~bit[88]&~bit[87]&~bit[86]) | (bit[88]&~bit[87]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[90]&bit[89]&bit[88]&~bit[87]&bit[86]) | (bit[90]&~bit[89]&~bit[88]&bit[87]&~bit[86]) | (bit[90]&~bit[89]&~bit[88]&~bit[87]&bit[86]) | (bit[90]&~bit[89]&bit[88]&bit[86]) | (~bit[90]&~bit[89]&bit[88]&bit[86]) | (bit[89]&~bit[88]&bit[87]) | (bit[89]&~bit[88]&~bit[87]&bit[86]) | (~bit[90]&~bit[88]&~bit[87]&~bit[86]) | (~bit[89]&~bit[88]&~bit[87]&~bit[86]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[90]&bit[89]&~bit[87]&bit[86]) | (bit[90]&~bit[89]&~bit[88]&bit[87]&~bit[86]) | (bit[90]&~bit[89]&~bit[88]&~bit[87]&bit[86]) | (~bit[90]&~bit[89]&bit[88]&~bit[87]&~bit[86]) | (~bit[89]&bit[88]&bit[87]&~bit[86]) | (bit[90]&bit[89]&~bit[88]&bit[86]) | (~bit[90]&~bit[89]&bit[88]&bit[86]) | (bit[90]&bit[89]&~bit[88]&~bit[87]&~bit[86]) | (~bit[90]&bit[87]&bit[86]) | (bit[89]&~bit[88]&~bit[87]&bit[86]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[90]&~bit[89]&~bit[88]&~bit[87]&~bit[86]) | (~bit[90]&bit[88]&bit[87]&~bit[86]) | (~bit[90]&bit[89]&~bit[88]&bit[86]) | (bit[90]&~bit[89]&~bit[88]&~bit[87]&bit[86]) | (bit[90]&bit[89]&bit[88]&bit[86]) | (bit[90]&~bit[89]&bit[88]&bit[86]) | (~bit[90]&bit[89]&bit[88]&~bit[86]) | (bit[90]&~bit[89]&bit[88]&~bit[87]) | (bit[90]&bit[89]&~bit[88]&~bit[87]&~bit[86]) | (bit[89]&bit[87]&~bit[86]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[90]&bit[89]&bit[88]&~bit[87]&bit[86]) | (bit[90]&~bit[89]&bit[88]&~bit[86]) | (~bit[90]&~bit[89]&bit[88]&~bit[87]&~bit[86]) | (bit[90]&bit[89]&bit[88]&bit[86]) | (bit[90]&bit[89]&~bit[88]&bit[86]) | (bit[90]&~bit[89]&~bit[88]&bit[86]) | (bit[90]&bit[88]&~bit[87]&~bit[86]) | (bit[90]&~bit[89]&bit[88]&~bit[87]) | (~bit[90]&bit[89]&bit[87]) | (~bit[89]&~bit[88]&~bit[87]&~bit[86]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[85]);
		control=control & bit[85];

		//-------------86--------------
		out[6] = (~disable_update & ((~bit[91]&~bit[90]&~bit[89]&~bit[88]&~bit[87]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((~bit[91]&bit[90]&bit[88]&~bit[87]) | (bit[90]&~bit[88]&~bit[87]) | (~bit[90]&bit[88]&~bit[87]) | (bit[89]&~bit[87]) | (bit[91]&~bit[89]&~bit[87]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[91]&~bit[90]&~bit[89]&~bit[88]) | (bit[90]&~bit[88]&~bit[87]) | (~bit[89]&~bit[88]&bit[87]) | (~bit[91]&bit[89]&~bit[88]) | (~bit[90]&bit[89]&~bit[88]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[91]&~bit[90]&~bit[89]&bit[88]&bit[87]) | (bit[91]&bit[90]&bit[89]&~bit[88]&bit[87]) | (~bit[89]&~bit[88]&bit[87]) | (~bit[91]&bit[90]&~bit[89]) | (~bit[91]&~bit[89]&bit[88]) | (bit[91]&~bit[89]&~bit[87]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[91]&bit[90]&~bit[89]&bit[88]&bit[87]) | (bit[91]&~bit[90]&bit[89]&bit[88]&bit[87]) | (~bit[91]&bit[89]&bit[88]&bit[87]) | (bit[90]&~bit[89]&~bit[88]&~bit[87]) | (bit[91]&~bit[90]&~bit[89]&~bit[88]) | (bit[91]&bit[90]&bit[89]&~bit[88]&bit[87]) | (~bit[90]&bit[88]&~bit[87]) | (~bit[91]&~bit[90]&bit[87]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[91]&~bit[90]&~bit[89]&bit[88]&bit[87]) | (bit[91]&bit[90]&bit[88]&bit[87]) | (bit[91]&bit[90]&bit[89]&~bit[88]&bit[87]) | (~bit[91]&~bit[90]&bit[89]&~bit[87]) | (~bit[90]&~bit[88]&~bit[87]) | (~bit[91]&bit[90]&~bit[89]) | (~bit[90]&bit[89]&~bit[88]) | (~bit[91]&~bit[89]&bit[88]) | (~bit[91]&~bit[90]&bit[87]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[91]&~bit[90]&bit[89]&bit[88]&bit[87]) | (bit[91]&~bit[90]&~bit[89]&bit[88]&bit[87]) | (~bit[91]&~bit[90]&~bit[88]&bit[87]) | (~bit[91]&bit[90]&bit[88]&~bit[87]) | (bit[91]&bit[90]&bit[88]&bit[87]) | (bit[91]&bit[90]&bit[89]&~bit[88]&bit[87]) | (~bit[91]&~bit[90]&bit[89]&~bit[87]) | (~bit[91]&bit[89]&~bit[88]) | (bit[91]&~bit[89]&~bit[87]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[86]);
		control=control & bit[86];

		//-------------87--------------

		out[6] = (~disable_update & ((~bit[92]&bit[91]&~bit[90]&~bit[89]&~bit[88]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((~bit[92]&bit[91]&~bit[90]&bit[89]&~bit[88]) | (~bit[91]&~bit[90]&bit[89]&~bit[88]) | (~bit[91]&bit[90]&bit[89]&~bit[88]) | (bit[92]&bit[91]&~bit[90]&~bit[88]) | (~bit[92]&bit[90]&~bit[88]) | (bit[92]&bit[90]&~bit[89]&~bit[88]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[92]&~bit[91]&~bit[90]&bit[88]) | (~bit[92]&~bit[91]&bit[90]&~bit[89]&bit[88]) | (~bit[92]&~bit[91]&bit[90]&~bit[89]&~bit[88]) | (bit[92]&bit[91]&bit[90]&bit[89]&~bit[88]) | (bit[92]&bit[91]&~bit[90]&~bit[89]&~bit[88]) | (~bit[92]&~bit[90]&~bit[89]&bit[88]) | (bit[92]&~bit[89]&bit[88]) | (~bit[92]&bit[91]&bit[90]&~bit[89]) | (bit[92]&~bit[91]&bit[90]&~bit[89]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[92]&~bit[91]&bit[90]&bit[89]&bit[88]) | (bit[92]&~bit[91]&~bit[90]&~bit[89]&bit[88]) | (~bit[92]&bit[91]&~bit[90]&bit[89]&~bit[88]) | (bit[92]&~bit[90]&bit[89]&bit[88]) | (~bit[92]&bit[91]&~bit[90]&bit[89]&bit[88]) | (~bit[92]&~bit[91]&bit[90]&~bit[89]&~bit[88]) | (bit[92]&bit[91]&~bit[90]&~bit[89]&~bit[88]) | (~bit[91]&~bit[90]&bit[89]&~bit[88]) | (bit[92]&bit[91]&bit[90]&~bit[88]) | (~bit[92]&~bit[90]&~bit[89]&bit[88]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[92]&bit[91]&~bit[90]&~bit[89]&bit[88]) | (~bit[92]&bit[91]&bit[90]&bit[89]&bit[88]) | (~bit[92]&~bit[91]&~bit[90]&~bit[89]&bit[88]) | (~bit[92]&~bit[91]&bit[89]&~bit[88]) | (bit[92]&~bit[91]&bit[89]&bit[88]) | (~bit[92]&~bit[91]&bit[90]&~bit[89]&bit[88]) | (~bit[92]&~bit[91]&bit[90]&~bit[89]&~bit[88]) | (bit[92]&bit[91]&bit[90]&bit[89]&~bit[88]) | (bit[92]&bit[91]&~bit[90]&~bit[88]) | (bit[92]&bit[90]&~bit[89]&~bit[88]) | (bit[92]&~bit[91]&bit[90]&~bit[89]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[92]&bit[91]&~bit[90]&~bit[89]&~bit[88]) | (bit[92]&~bit[91]&~bit[90]&~bit[89]&bit[88]) | (bit[92]&bit[91]&~bit[90]&~bit[89]&bit[88]) | (~bit[92]&~bit[91]&bit[90]&~bit[89]&bit[88]) | (~bit[92]&bit[91]&~bit[90]&bit[89]&bit[88]) | (bit[92]&bit[91]&bit[90]&~bit[88]) | (bit[92]&bit[90]&bit[89]) | (bit[92]&bit[89]&~bit[88]) | (~bit[92]&bit[91]&bit[90]&~bit[89]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[92]&bit[91]&bit[90]&bit[89]&bit[88]) | (~bit[92]&~bit[91]&~bit[90]&~bit[89]&bit[88]) | (~bit[92]&bit[91]&~bit[90]&bit[89]&~bit[88]) | (~bit[92]&bit[91]&~bit[90]&bit[89]&bit[88]) | (~bit[91]&~bit[90]&~bit[89]&~bit[88]) | (bit[92]&bit[91]&bit[90]&bit[89]&~bit[88]) | (bit[92]&bit[91]&~bit[90]&~bit[89]&~bit[88]) | (~bit[91]&bit[90]&bit[89]&~bit[88]) | (~bit[92]&bit[91]&bit[90]&~bit[89]) | (bit[92]&bit[91]&bit[88]) | (bit[92]&bit[90]&~bit[89]&~bit[88]) | (bit[92]&~bit[91]&bit[90]&~bit[89]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[87]);
		control=control & bit[87];

		//-------------88--------------

		out[6] = (~disable_update & ((~bit[93]&bit[91]&~bit[90]&~bit[89]) | (~bit[92]&bit[91]&~bit[90]&~bit[89]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((~bit[93]&~bit[92]&~bit[91]&~bit[90]&bit[89]) | (bit[93]&bit[92]&bit[90]&~bit[89]) | (~bit[93]&~bit[92]&~bit[91]&bit[90]&~bit[89]) | (~bit[93]&bit[92]&bit[90]&~bit[89]) | (bit[93]&bit[92]&bit[91]&~bit[90]&~bit[89]) | (~bit[92]&bit[91]&bit[90]&~bit[89]) | (bit[93]&~bit[92]&~bit[91]&bit[90]&~bit[89]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[93]&~bit[92]&~bit[91]&bit[90]&~bit[89]) | (~bit[92]&~bit[91]&bit[90]&bit[89]) | (bit[93]&~bit[92]&~bit[91]&~bit[90]&bit[89]) | (bit[93]&bit[92]&bit[91]&~bit[90]&~bit[89]) | (~bit[92]&bit[91]&~bit[90]&bit[89]) | (bit[92]&~bit[90]&bit[89]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[93]&bit[92]&~bit[91]&bit[89]) | (bit[93]&~bit[92]&~bit[91]&~bit[90]&bit[89]) | (bit[93]&bit[92]&bit[91]&~bit[90]&~bit[89]) | (~bit[92]&bit[91]&bit[90]&~bit[89]) | (bit[92]&~bit[91]&bit[90]) | (bit[93]&~bit[92]&~bit[91]&bit[90]&~bit[89]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[93]&bit[92]&~bit[91]&~bit[90]&bit[89]) | (~bit[93]&~bit[92]&~bit[91]&bit[90]&~bit[89]) | (~bit[93]&~bit[92]&bit[91]&~bit[90]) | (bit[93]&~bit[92]&~bit[91]&~bit[90]&bit[89]) | (~bit[93]&bit[92]&bit[90]&~bit[89]) | (bit[93]&bit[92]&bit[91]&~bit[90]&~bit[89]) | (~bit[93]&~bit[92]&bit[91]&bit[89]) | (bit[93]&~bit[92]&~bit[91]&bit[90]&~bit[89]) | (~bit[92]&bit[91]&~bit[90]&bit[89]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[92]&~bit[91]&bit[89]) | (bit[93]&bit[92]&bit[90]&~bit[89]) | (~bit[92]&bit[91]&bit[90]&bit[89]) | (bit[93]&~bit[92]&~bit[91]&~bit[90]&bit[89]) | (bit[93]&~bit[92]&~bit[91]&bit[90]&~bit[89]) | (bit[92]&~bit[90]&bit[89]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[93]&bit[92]&~bit[91]&~bit[90]&bit[89]) | (bit[93]&bit[92]&bit[91]&~bit[90]&~bit[89]) | (bit[93]&~bit[92]&~bit[91]&bit[90]&~bit[89]) | (~bit[92]&~bit[90]&~bit[89]) | (~bit[93]&bit[90]&bit[89]) | (~bit[93]&bit[91]&bit[89]) | (~bit[93]&~bit[92]&bit[91]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[88]);
		control=control & bit[88];

		//-------------89--------------

		out[6] = (~disable_update & ((~bit[95]&~bit[94]&bit[93]&bit[92]&bit[91]&bit[90]) | (~bit[94]&~bit[93]&~bit[92]&~bit[91]&~bit[90]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((~bit[95]&bit[94]&bit[93]&bit[92]&bit[91]&bit[90]) | (~bit[94]&~bit[93]&~bit[92]&bit[91]&~bit[90]) | (~bit[94]&~bit[93]&bit[92]&~bit[91]&~bit[90]) | (bit[93]&~bit[92]&bit[91]&~bit[90]) | (bit[95]&bit[93]&bit[92]&bit[91]&bit[90]) | (bit[94]&~bit[93]&~bit[92]&~bit[90]) | (bit[94]&~bit[91]&~bit[90]) | (~bit[94]&bit[93]&~bit[91]&~bit[90]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[95]&bit[94]&bit[93]&bit[92]&bit[91]&bit[90]) | (bit[94]&bit[93]&~bit[92]&~bit[91]&bit[90]) | (~bit[94]&~bit[93]&~bit[92]&bit[91]&~bit[90]) | (bit[95]&bit[93]&bit[92]&bit[91]&bit[90]) | (~bit[93]&~bit[92]&~bit[91]&bit[90]) | (~bit[94]&bit[93]&~bit[92]&~bit[91]) | (bit[94]&~bit[91]&~bit[90]) | (~bit[93]&bit[92]&~bit[90]) | (~bit[94]&bit[93]&~bit[91]&~bit[90]) | (bit[92]&bit[91]&~bit[90]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[95]&bit[94]&bit[93]&bit[92]&bit[91]&bit[90]) | (~bit[94]&~bit[93]&bit[92]&~bit[91]&~bit[90]) | (bit[93]&~bit[92]&~bit[91]&~bit[90]) | (~bit[94]&~bit[93]&~bit[92]&bit[90]) | (~bit[93]&bit[92]&~bit[91]&bit[90]) | (bit[95]&bit[93]&bit[92]&bit[91]&bit[90]) | (~bit[93]&~bit[92]&~bit[91]&bit[90]) | (bit[93]&bit[92]&~bit[91]&bit[90]) | (bit[94]&~bit[93]&~bit[92]&~bit[90]) | (bit[92]&bit[91]&~bit[90]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[94]&~bit[93]&~bit[92]&bit[91]&bit[90]) | (~bit[95]&bit[94]&bit[93]&bit[92]&bit[91]&bit[90]) | (~bit[94]&bit[93]&bit[92]&bit[91]) | (~bit[93]&bit[92]&~bit[91]&bit[90]) | (~bit[94]&bit[93]&~bit[92]&~bit[90]) | (~bit[94]&bit[93]&~bit[92]&~bit[91]) | (bit[94]&~bit[91]&~bit[90]) | (~bit[94]&bit[93]&~bit[91]&~bit[90]) | (bit[92]&bit[91]&~bit[90]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[94]&~bit[93]&~bit[92]&bit[91]&bit[90]) | (~bit[94]&~bit[93]&~bit[92]&~bit[91]&~bit[90]) | (~bit[95]&bit[94]&bit[93]&bit[92]&bit[91]&bit[90]) | (bit[93]&~bit[92]&bit[91]&bit[90]) | (bit[95]&bit[93]&bit[92]&bit[91]&bit[90]) | (~bit[94]&bit[93]&~bit[92]&~bit[90]) | (~bit[94]&bit[92]&~bit[91]&bit[90]) | (bit[93]&bit[92]&~bit[91]&bit[90]) | (bit[94]&~bit[93]&~bit[92]&~bit[90]) | (~bit[93]&bit[92]&~bit[90]) | (~bit[94]&bit[93]&~bit[91]&~bit[90]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[94]&bit[93]&~bit[92]&~bit[91]&bit[90]) | (~bit[94]&~bit[93]&~bit[91]&bit[90]) | (bit[93]&~bit[92]&bit[91]&~bit[90]) | (bit[95]&bit[93]&bit[92]&bit[91]&bit[90]) | (~bit[94]&bit[92]&~bit[91]&bit[90]) | (~bit[94]&bit[93]&bit[91]) | (~bit[93]&bit[92]&~bit[91]) | (~bit[94]&~bit[93]&bit[92]) | (bit[94]&~bit[93]&~bit[92]&~bit[90]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[89]);
		control=control & bit[89];

		//-------------90--------------

		out[6] = (~disable_update & ((bit[96]&bit[95]&~bit[94]&bit[93]&bit[92]&bit[91]) | (~bit[96]&bit[95]&~bit[94]&bit[93]&bit[92]&bit[91]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((bit[95]&bit[94]&~bit[93]&~bit[91]) | (bit[95]&~bit[94]&~bit[92]&~bit[91]) | (~bit[94]&~bit[93]&bit[92]&~bit[91]) | (bit[94]&bit[93]&bit[92]&bit[91]) | (~bit[94]&bit[93]&~bit[92]&~bit[91]) | (~bit[95]&~bit[93]&~bit[91]) | (bit[94]&bit[93]&~bit[92]&~bit[91]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[94]&bit[93]&bit[92]&bit[91]) | (~bit[95]&~bit[94]&~bit[92]) | (bit[93]&bit[92]&~bit[91]) | (~bit[93]&~bit[92]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[95]&~bit[94]&~bit[92]&~bit[91]) | (~bit[95]&~bit[94]&~bit[93]&~bit[92]) | (bit[95]&bit[93]&~bit[92]&bit[91]) | (bit[94]&bit[93]&bit[92]&bit[91]) | (bit[94]&bit[93]&~bit[92]&~bit[91]) | (~bit[94]&~bit[93]&bit[91]) | (bit[93]&bit[92]&~bit[91]) | (~bit[95]&bit[94]&bit[93]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[96]&bit[95]&~bit[94]&bit[93]&bit[92]&bit[91]) | (bit[95]&~bit[94]&bit[93]&~bit[92]) | (~bit[94]&~bit[93]&bit[92]&~bit[91]) | (~bit[95]&~bit[94]&bit[92]&~bit[91]) | (bit[95]&~bit[94]&bit[93]&~bit[91]) | (~bit[94]&bit[93]&~bit[92]&~bit[91]) | (bit[94]&~bit[93]&bit[91]) | (bit[94]&~bit[93]&~bit[92]) | (~bit[95]&bit[94]&bit[93]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[95]&bit[94]&~bit[92]&bit[91]) | (bit[95]&~bit[94]&bit[93]&~bit[91]) | (~bit[95]&bit[94]&~bit[93]) | (~bit[95]&bit[92]&bit[91]) | (bit[95]&bit[93]&bit[92]) | (~bit[94]&bit[93]&bit[92]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[96]&bit[95]&~bit[94]&bit[93]&bit[92]&bit[91]) | (~bit[96]&bit[94]&bit[93]&bit[92]&bit[91]) | (~bit[95]&~bit[93]&~bit[92]&bit[91]) | (bit[95]&bit[94]&~bit[93]&~bit[91]) | (~bit[95]&~bit[94]&bit[93]&bit[91]) | (~bit[95]&~bit[94]&bit[92]&~bit[91]) | (~bit[95]&~bit[94]&~bit[93]&~bit[92]) | (bit[95]&bit[93]&~bit[92]&bit[91]) | (bit[94]&bit[93]&~bit[92]&~bit[91]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[90]);
		control=control & bit[90];

		//-------------91--------------


		out[6] = (~disable_update & ((~bit[97]&~bit[96]&~bit[95]&bit[94]&bit[93]&bit[92]) | (bit[95]&~bit[94]&bit[93]&bit[92]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((bit[96]&bit[95]&~bit[94]&bit[93]&~bit[92]) | (~bit[96]&bit[95]&~bit[94]&~bit[92]) | (~bit[95]&~bit[94]&bit[93]&~bit[92]) | (bit[96]&~bit[95]&bit[94]&bit[93]&bit[92]) | (bit[95]&bit[94]&bit[93]&bit[92]) | (bit[97]&~bit[95]&bit[94]&bit[93]&bit[92]) | (bit[95]&bit[94]&~bit[93]&~bit[92]) | (~bit[95]&~bit[93]&~bit[92]) | (~bit[94]&~bit[93]&~bit[92]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[96]&~bit[95]&bit[94]&bit[93]&~bit[92]) | (~bit[96]&bit[95]&bit[94]&bit[93]) | (bit[96]&~bit[95]&bit[94]&bit[93]&bit[92]) | (bit[95]&bit[94]&bit[93]&bit[92]) | (~bit[96]&~bit[94]&~bit[93]) | (bit[97]&~bit[95]&bit[94]&bit[93]&bit[92]) | (~bit[95]&~bit[94]&~bit[93]) | (bit[96]&bit[94]&bit[93]&~bit[92]) | (~bit[95]&~bit[93]&~bit[92]) | (~bit[94]&~bit[93]&~bit[92]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[96]&bit[95]&~bit[94]&~bit[93]&bit[92]) | (~bit[95]&bit[94]&~bit[93]&bit[92]) | (~bit[96]&~bit[95]&bit[94]&bit[93]&~bit[92]) | (~bit[95]&~bit[94]&bit[93]&~bit[92]) | (~bit[96]&bit[95]&bit[94]&bit[93]) | (bit[96]&~bit[95]&bit[94]&bit[93]&bit[92]) | (bit[97]&~bit[95]&bit[94]&bit[93]&bit[92]) | (bit[96]&bit[94]&bit[93]&~bit[92]) | (bit[95]&bit[94]&~bit[93]&~bit[92]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[96]&bit[95]&~bit[94]&~bit[93]&bit[92]) | (~bit[97]&bit[95]&bit[93]&bit[92]) | (bit[96]&bit[95]&bit[94]&bit[92]) | (~bit[96]&~bit[95]&~bit[93]&bit[92]) | (~bit[96]&~bit[95]&bit[94]&bit[93]&~bit[92]) | (~bit[96]&bit[95]&~bit[94]&~bit[92]) | (~bit[96]&~bit[94]&bit[93]&bit[92]) | (bit[96]&~bit[95]&bit[94]&bit[93]&bit[92]) | (bit[97]&~bit[95]&bit[94]&bit[93]&bit[92]) | (~bit[96]&bit[95]&bit[94]&~bit[93]) | (bit[95]&bit[94]&~bit[93]&~bit[92]) | (~bit[94]&~bit[93]&~bit[92]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[97]&~bit[96]&bit[95]&~bit[94]&bit[93]&bit[92]) | (~bit[97]&bit[96]&bit[94]&bit[93]&bit[92]) | (bit[97]&~bit[96]&~bit[95]&bit[94]&bit[93]) | (bit[96]&~bit[95]&~bit[94]&bit[93]&bit[92]) | (bit[96]&bit[95]&~bit[94]&~bit[93]&bit[92]) | (bit[96]&bit[95]&~bit[94]&bit[93]&~bit[92]) | (~bit[95]&~bit[94]&~bit[93]&~bit[92]) | (~bit[95]&bit[94]&bit[93]&~bit[92]) | (~bit[96]&~bit[95]&bit[93]&~bit[92]) | (~bit[96]&bit[95]&bit[94]&~bit[93]) | (~bit[96]&bit[94]&~bit[92]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[97]&bit[95]&bit[93]&bit[92]) | (~bit[96]&~bit[94]&~bit[93]&~bit[92]) | (bit[96]&bit[95]&~bit[94]&bit[93]&~bit[92]) | (bit[96]&~bit[93]&bit[92]) | (~bit[96]&~bit[95]&bit[93]&~bit[92]) | (bit[97]&~bit[95]&bit[94]&bit[93]&bit[92]) | (bit[96]&bit[94]&bit[93]&~bit[92]) | (~bit[96]&bit[95]&bit[94]&~bit[93]) | (bit[95]&bit[94]&~bit[93]&~bit[92]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[91]);
		control=control & bit[91];

		//-------------92--------------

		out[6] = (~disable_update & ((bit[97]&bit[96]&~bit[95]&bit[94]&bit[93]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((bit[97]&bit[96]&~bit[95]&~bit[93]) | (bit[97]&~bit[96]&~bit[94]&~bit[93]) | (~bit[96]&~bit[95]&bit[94]&~bit[93]) | (~bit[97]&~bit[96]&~bit[93]) | (bit[95]&~bit[94]&~bit[93]) | (bit[95]&bit[94]&bit[93]) | (~bit[97]&~bit[95]&~bit[93]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[97]&bit[96]&bit[95]&bit[94]) | (~bit[96]&~bit[95]&~bit[94]) | (~bit[95]&~bit[94]&bit[93]) | (bit[97]&bit[95]&bit[94]&~bit[93]) | (bit[95]&bit[94]&bit[93]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[97]&bit[96]&bit[95]&~bit[93]) | (bit[96]&~bit[95]&~bit[94]&~bit[93]) | (~bit[97]&bit[95]&~bit[94]) | (~bit[96]&bit[95]&bit[93]) | (bit[97]&bit[95]&bit[94]&~bit[93]) | (bit[95]&~bit[94]&~bit[93]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[97]&bit[96]&~bit[95]&bit[94]&bit[93]) | (bit[97]&bit[96]&bit[95]&~bit[94]&bit[93]) | (~bit[98]&~bit[96]&bit[94]&bit[93]) | (bit[97]&~bit[96]&bit[94]&~bit[93]) | (~bit[97]&bit[96]&bit[95]&bit[94]) | (bit[96]&~bit[95]&~bit[94]&~bit[93]) | (~bit[97]&bit[95]&bit[94]&bit[93]) | (~bit[97]&~bit[96]&~bit[94]) | (~bit[96]&~bit[95]&bit[93]) | (~bit[97]&~bit[95]&~bit[93]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[98]&bit[97]&bit[96]&~bit[95]&bit[94]&bit[93]) | (~bit[97]&~bit[96]&~bit[95]&bit[94]&~bit[93]) | (bit[97]&bit[96]&bit[95]&~bit[94]&bit[93]) | (~bit[97]&bit[96]&~bit[95]&~bit[94]) | (~bit[97]&bit[96]&bit[95]&~bit[93]) | (bit[97]&~bit[96]&~bit[94]&~bit[93]) | (bit[97]&~bit[96]&bit[95]&~bit[94]) | (~bit[97]&bit[95]&bit[94]&bit[93]) | (~bit[97]&~bit[96]&~bit[94]&bit[93]) | (bit[97]&bit[95]&bit[94]&~bit[93]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[98]&bit[97]&bit[96]&~bit[95]&bit[94]&bit[93]) | (~bit[98]&bit[95]&bit[94]&bit[93]) | (bit[97]&bit[96]&~bit[95]&~bit[93]) | (~bit[97]&~bit[95]&bit[94]&bit[93]) | (~bit[96]&~bit[95]&bit[94]&~bit[93]) | (bit[97]&~bit[96]&bit[95]&~bit[94]) | (~bit[97]&~bit[96]&~bit[94]&bit[93]) | (bit[97]&~bit[94]&bit[93]) | (bit[97]&bit[95]&bit[94]&~bit[93]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[92]);
		control=control & bit[92];

		//-------------93--------------

		out[6] = (~disable_update & ((~bit[99]&~bit[98]&~bit[97]&bit[96]&bit[95]&bit[94]) | (bit[99]&~bit[98]&bit[97]&~bit[96]&bit[95]&bit[94]) | (~bit[99]&~bit[98]&bit[97]&~bit[96]&bit[95]&bit[94]) | (bit[98]&bit[97]&~bit[96]&bit[95]&bit[94]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((~bit[99]&~bit[98]&bit[97]&bit[96]&bit[95]&bit[94]) | (~bit[99]&bit[98]&~bit[97]&bit[96]&bit[95]&bit[94]) | (bit[99]&~bit[97]&bit[96]&bit[95]&bit[94]) | (bit[98]&bit[97]&bit[96]&bit[95]&bit[94]) | (bit[99]&~bit[98]&bit[96]&bit[95]&bit[94]) | (~bit[95]&~bit[94]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[99]&~bit[98]&bit[97]&bit[96]&bit[95]&bit[94]) | (~bit[99]&bit[98]&~bit[97]&bit[96]&bit[95]&bit[94]) | (bit[99]&~bit[97]&bit[96]&bit[95]&bit[94]) | (bit[98]&bit[97]&bit[96]&bit[95]&bit[94]) | (bit[99]&~bit[98]&bit[96]&bit[95]&bit[94]) | (~bit[96]&bit[95]&~bit[94]) | (~bit[98]&bit[95]&~bit[94]) | (~bit[97]&bit[95]&~bit[94]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[99]&~bit[98]&bit[97]&bit[96]&bit[95]&bit[94]) | (~bit[99]&bit[98]&~bit[97]&bit[96]&bit[95]&bit[94]) | (bit[99]&~bit[97]&bit[96]&bit[95]&bit[94]) | (~bit[98]&bit[97]&~bit[96]&~bit[95]&bit[94]) | (bit[98]&bit[97]&bit[96]&bit[95]&~bit[94]) | (~bit[98]&~bit[97]&~bit[95]&bit[94]) | (bit[98]&bit[97]&~bit[96]&~bit[95]) | (~bit[97]&~bit[96]&~bit[95]) | (~bit[96]&~bit[94]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[99]&~bit[98]&bit[97]&~bit[96]&bit[95]&bit[94]) | (~bit[99]&~bit[98]&bit[97]&~bit[96]&bit[95]&bit[94]) | (~bit[99]&bit[98]&~bit[97]&bit[96]&bit[95]&bit[94]) | (~bit[98]&bit[97]&bit[96]&~bit[95]&bit[94]) | (bit[98]&bit[97]&bit[96]&bit[95]&bit[94]) | (bit[98]&bit[97]&bit[96]&bit[95]&~bit[94]) | (bit[98]&bit[96]&~bit[95]&bit[94]) | (bit[98]&~bit[97]&~bit[95]&~bit[94]) | (bit[99]&~bit[98]&bit[96]&bit[95]&bit[94]) | (~bit[98]&~bit[97]&bit[96]&~bit[94]) | (~bit[98]&~bit[95]&~bit[94]) | (~bit[97]&~bit[96]&~bit[94]) | (~bit[98]&~bit[97]&~bit[96]) | (~bit[97]&~bit[96]&~bit[95]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[98]&bit[97]&~bit[96]&~bit[95]&~bit[94]) | (~bit[99]&~bit[98]&bit[97]&~bit[96]&bit[95]&bit[94]) | (bit[98]&bit[97]&~bit[96]&bit[95]&bit[94]) | (~bit[98]&bit[97]&~bit[96]&~bit[95]&bit[94]) | (~bit[98]&bit[97]&bit[96]&~bit[95]&bit[94]) | (bit[98]&bit[97]&bit[96]&bit[95]&~bit[94]) | (bit[98]&~bit[97]&bit[96]&bit[94]) | (~bit[98]&~bit[96]&bit[95]&~bit[94]) | (bit[99]&~bit[98]&bit[96]&bit[95]&bit[94]) | (~bit[98]&~bit[97]&~bit[94]) | (~bit[97]&bit[95]&~bit[94]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[99]&~bit[98]&~bit[97]&bit[96]&bit[95]&bit[94]) | (bit[99]&~bit[98]&bit[97]&~bit[96]&bit[95]&bit[94]) | (~bit[99]&~bit[98]&bit[97]&bit[96]&bit[95]&bit[94]) | (~bit[98]&~bit[97]&~bit[96]&~bit[95]&bit[94]) | (bit[98]&~bit[97]&~bit[96]&bit[95]&bit[94]) | (~bit[99]&bit[98]&bit[97]&bit[94]) | (bit[98]&bit[97]&~bit[96]&~bit[95]) | (~bit[98]&~bit[97]&bit[95]&~bit[94]) | (bit[98]&bit[96]&~bit[95]&bit[94]) | (bit[98]&~bit[97]&~bit[95]&~bit[94]) | (bit[99]&~bit[98]&bit[96]&bit[95]&bit[94]) | (~bit[98]&~bit[97]&bit[96]&~bit[94]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[93]);
		control=control & bit[93];

		//-------------94--------------


		out[6] = (~disable_update & ((~bit[100]&~bit[99]&~bit[97]&bit[96]&bit[95]) | (bit[99]&~bit[98]&~bit[97]&bit[96]&bit[95]) | (~bit[99]&~bit[98]&~bit[97]&bit[96]&bit[95]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((bit[100]&bit[99]&bit[98]&bit[97]&bit[96]&bit[95]) | (~bit[100]&~bit[99]&bit[97]&bit[96]&bit[95]) | (bit[99]&bit[98]&~bit[97]&bit[96]&bit[95]) | (~bit[100]&bit[99]&bit[98]&bit[96]&bit[95]) | (bit[100]&~bit[99]&bit[98]&bit[96]&bit[95]) | (~bit[98]&bit[97]&bit[96]&bit[95]) | (~bit[96]&~bit[95]) | (~bit[97]&~bit[95]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[100]&~bit[99]&bit[98]&~bit[97]&bit[96]&bit[95]) | (~bit[100]&~bit[99]&bit[97]&bit[96]&bit[95]) | (bit[99]&bit[98]&~bit[97]&bit[96]&bit[95]) | (~bit[98]&~bit[97]&~bit[96]&~bit[95]) | (~bit[99]&~bit[97]&~bit[96]&bit[95]) | (~bit[98]&bit[97]&bit[96]&bit[95]) | (~bit[98]&~bit[97]&~bit[96]&bit[95]) | (bit[97]&bit[96]&~bit[95]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[100]&bit[99]&bit[98]&bit[97]&bit[96]&bit[95]) | (bit[99]&bit[98]&~bit[97]&~bit[96]&~bit[95]) | (~bit[99]&bit[98]&~bit[97]&~bit[96]&~bit[95]) | (~bit[100]&bit[99]&bit[98]&bit[96]&bit[95]) | (~bit[100]&~bit[99]&~bit[98]&bit[97]) | (~bit[99]&~bit[98]&bit[97]&~bit[96]) | (bit[100]&~bit[99]&bit[98]&bit[96]&bit[95]) | (bit[99]&bit[98]&~bit[97]&bit[95]) | (bit[99]&~bit[98]&bit[97]&~bit[95]) | (~bit[99]&bit[97]&~bit[95]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[99]&bit[98]&~bit[97]&~bit[96]&~bit[95]) | (~bit[99]&bit[98]&bit[97]&~bit[96]&bit[95]) | (~bit[100]&bit[99]&bit[98]&bit[96]&bit[95]) | (~bit[99]&~bit[98]&~bit[97]&bit[96]&bit[95]) | (bit[100]&~bit[99]&~bit[98]&bit[97]&bit[96]) | (bit[100]&~bit[99]&bit[98]&bit[96]&bit[95]) | (bit[99]&bit[98]&~bit[97]&bit[95]) | (~bit[98]&~bit[97]&~bit[96]&bit[95]) | (bit[99]&~bit[98]&~bit[96]&bit[95]) | (bit[99]&bit[98]&bit[97]&~bit[95]) | (~bit[98]&bit[96]&~bit[95]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[100]&bit[99]&~bit[97]&bit[96]&bit[95]) | (bit[99]&bit[98]&~bit[97]&~bit[96]&~bit[95]) | (bit[99]&~bit[98]&~bit[97]&bit[96]&bit[95]) | (bit[100]&~bit[99]&~bit[98]&bit[97]&bit[96]) | (~bit[100]&~bit[99]&~bit[98]&~bit[97]) | (bit[100]&~bit[99]&bit[98]&bit[96]&bit[95]) | (bit[99]&~bit[98]&bit[97]&bit[95]) | (~bit[99]&~bit[97]&bit[96]&~bit[95]) | (~bit[98]&~bit[97]&~bit[96]&~bit[95]) | (~bit[99]&~bit[97]&~bit[96]&bit[95]) | (bit[99]&bit[98]&bit[97]&~bit[95]) | (~bit[99]&~bit[98]&~bit[95]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[100]&~bit[99]&bit[98]&~bit[97]&bit[96]&bit[95]) | (bit[100]&bit[99]&bit[98]&bit[97]&bit[96]&bit[95]) | (~bit[99]&~bit[98]&~bit[97]&~bit[96]&~bit[95]) | (~bit[99]&bit[98]&bit[97]&~bit[96]&bit[95]) | (bit[99]&bit[98]&~bit[96]&~bit[95]) | (bit[100]&~bit[99]&~bit[98]&bit[97]&bit[96]) | (~bit[100]&bit[99]&~bit[98]&bit[95]) | (~bit[98]&bit[97]&bit[96]&~bit[95]) | (bit[99]&~bit[98]&bit[96]&~bit[95]) | (bit[99]&~bit[98]&bit[97]&~bit[95]) | (bit[99]&~bit[98]&~bit[96]&bit[95]) | (bit[99]&bit[98]&bit[97]&~bit[95]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[94]);
		control=control & bit[94];

		//-------------95--------------

		out[6] = (~disable_update & ((~bit[101]&bit[100]&bit[99]&~bit[98]&bit[97]&bit[96]) | (bit[101]&bit[100]&bit[99]&~bit[98]&bit[97]&bit[96]) | (~bit[99]&bit[98]&bit[97]&bit[96]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((bit[100]&~bit[99]&~bit[97]&~bit[96]) | (bit[99]&~bit[98]&~bit[97]&~bit[96]) | (bit[99]&bit[98]&bit[97]&bit[96]) | (~bit[100]&~bit[97]&~bit[96]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[100]&bit[99]&bit[98]&bit[97]&~bit[96]) | (~bit[100]&~bit[98]&bit[97]&~bit[96]) | (bit[99]&bit[98]&bit[97]&bit[96]) | (bit[100]&bit[99]&bit[98]&~bit[97]&~bit[96]) | (bit[100]&bit[99]&bit[97]&~bit[96]) | (~bit[99]&bit[97]&~bit[96]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[101]&bit[100]&bit[99]&~bit[98]&bit[97]&bit[96]) | (~bit[100]&~bit[99]&~bit[98]&~bit[96]) | (~bit[100]&~bit[99]&~bit[97]&bit[96]) | (bit[100]&~bit[99]&~bit[98]&~bit[97]&bit[96]) | (bit[99]&bit[98]&bit[97]&bit[96]) | (bit[99]&~bit[98]&~bit[97]&bit[96]) | (bit[100]&bit[99]&bit[98]&~bit[97]&~bit[96]) | (~bit[98]&bit[97]&~bit[96]) | (~bit[99]&bit[97]&~bit[96]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[101]&bit[100]&bit[99]&~bit[98]&bit[97]&bit[96]) | (~bit[101]&bit[99]&bit[98]&bit[96]) | (~bit[100]&bit[99]&bit[98]&bit[97]&~bit[96]) | (bit[100]&~bit[99]&~bit[98]&~bit[97]&bit[96]) | (~bit[100]&~bit[99]&~bit[98]&bit[96]) | (bit[99]&~bit[98]&~bit[97]&~bit[96]) | (~bit[100]&~bit[98]&bit[97]&~bit[96]) | (bit[100]&bit[98]&~bit[97]&bit[96]) | (bit[100]&~bit[99]&~bit[98]&~bit[96]) | (~bit[100]&bit[99]&bit[98]&bit[96]) | (bit[100]&bit[99]&bit[98]&~bit[97]&~bit[96]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[101]&bit[100]&bit[99]&bit[98]&bit[97]) | (bit[100]&~bit[99]&~bit[98]&~bit[97]&~bit[96]) | (bit[100]&~bit[99]&bit[98]&~bit[97]&bit[96]) | (bit[100]&~bit[99]&~bit[98]&bit[97]&bit[96]) | (~bit[100]&~bit[99]&bit[98]&~bit[96]) | (~bit[100]&bit[99]&~bit[98]&~bit[97]) | (~bit[100]&~bit[99]&bit[97]&~bit[96]) | (~bit[100]&bit[98]&bit[97]&bit[96]) | (~bit[100]&~bit[98]&~bit[97]&bit[96]) | (bit[99]&~bit[98]&~bit[97]&bit[96]) | (~bit[100]&bit[99]&bit[98]&bit[96]) | (bit[100]&bit[99]&bit[98]&~bit[97]&~bit[96]) | (bit[100]&bit[99]&bit[97]&~bit[96]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[101]&bit[100]&bit[99]&bit[98]&bit[97]) | (~bit[101]&bit[98]&bit[97]&bit[96]) | (~bit[100]&bit[99]&bit[98]&bit[97]&~bit[96]) | (bit[100]&~bit[99]&~bit[98]&bit[97]&bit[96]) | (bit[100]&~bit[99]&~bit[97]&~bit[96]) | (bit[100]&~bit[99]&~bit[98]&~bit[97]&bit[96]) | (~bit[100]&bit[99]&~bit[98]&bit[96]) | (bit[100]&bit[98]&~bit[97]&bit[96]) | (bit[100]&~bit[99]&~bit[98]&~bit[96]) | (bit[100]&bit[99]&bit[98]&~bit[97]&~bit[96]) | (bit[100]&bit[99]&bit[97]&~bit[96]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[95]);
		control=control & bit[95];

		//-------------96--------------


		out[6] = (~disable_update & ((bit[100]&bit[99]&bit[98]&bit[97]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((~bit[100]&bit[99]&~bit[98]&~bit[97]) | (bit[100]&bit[99]&~bit[98]&~bit[97]) | (~bit[99]&~bit[97]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[100]&~bit[99]&~bit[98]&bit[97]) | (~bit[99]&~bit[98]&~bit[97]) | (~bit[101]&~bit[99]&~bit[98]) | (bit[99]&bit[98]&~bit[97]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[101]&~bit[100]&~bit[99]&bit[98]&bit[97]) | (~bit[100]&~bit[98]&~bit[97]) | (~bit[101]&bit[100]&bit[99]&~bit[98]&bit[97]) | (bit[101]&bit[100]&~bit[98]&bit[97]) | (bit[100]&bit[99]&~bit[98]&~bit[97]) | (bit[99]&bit[98]&~bit[97]) | (~bit[100]&bit[99]&~bit[98]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[102]&~bit[101]&bit[100]&bit[98]&bit[97]) | (~bit[101]&bit[100]&~bit[99]&bit[98]&bit[97]) | (~bit[101]&bit[100]&~bit[98]&~bit[97]) | (~bit[100]&~bit[99]&~bit[98]&bit[97]) | (bit[101]&~bit[99]&bit[97]) | (~bit[100]&bit[98]&~bit[97]) | (~bit[100]&bit[99]&~bit[98]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[102]&bit[101]&bit[100]&bit[99]&bit[97]) | (bit[101]&~bit[100]&~bit[99]&bit[98]&bit[97]) | (bit[102]&~bit[101]&bit[100]&bit[99]&bit[97]) | (~bit[101]&~bit[99]&bit[98]&~bit[97]) | (~bit[101]&bit[100]&bit[99]&~bit[98]&bit[97]) | (~bit[100]&bit[99]&~bit[98]&~bit[97]) | (bit[101]&bit[100]&~bit[98]&bit[97]) | (~bit[101]&~bit[100]&bit[99]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[101]&~bit[100]&bit[99]&bit[98]&bit[97]) | (bit[102]&~bit[101]&bit[100]&bit[99]&bit[97]) | (~bit[101]&bit[100]&~bit[99]&bit[98]&bit[97]) | (~bit[101]&bit[100]&bit[99]&~bit[97]) | (~bit[101]&bit[100]&bit[99]&~bit[98]&bit[97]) | (bit[100]&bit[99]&~bit[98]&~bit[97]) | (~bit[101]&~bit[100]&~bit[99]) | (~bit[101]&~bit[98]&~bit[97]) | (bit[100]&~bit[99]&~bit[98]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[96]);
		control=control & bit[96];

		//-------------97--------------

		out[6] = (~disable_update & ((~bit[103]&~bit[102]&bit[101]&bit[100]&bit[99]&bit[98]) | (~bit[103]&bit[102]&bit[101]&bit[100]&bit[99]&bit[98]) | (bit[103]&bit[101]&bit[100]&bit[99]&bit[98]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((bit[102]&bit[100]&~bit[99]&~bit[98]) | (bit[101]&~bit[100]&~bit[99]&~bit[98]) | (~bit[100]&bit[99]&~bit[98]) | (~bit[101]&~bit[98]) | (~bit[102]&~bit[98]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[102]&~bit[100]&~bit[99]&bit[98]) | (bit[102]&bit[101]&bit[100]&bit[99]&~bit[98]) | (bit[101]&~bit[100]&~bit[99]&~bit[98]) | (~bit[101]&~bit[99]) | (~bit[102]&~bit[99]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[103]&~bit[102]&bit[101]&bit[100]&bit[99]&bit[98]) | (bit[102]&bit[101]&bit[100]&bit[99]&~bit[98]) | (~bit[102]&~bit[100]&bit[98]) | (~bit[100]&bit[99]&~bit[98]) | (bit[102]&bit[101]&bit[100]&~bit[99]) | (~bit[101]&~bit[100]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[103]&bit[102]&bit[101]&bit[100]&bit[99]&bit[98]) | (bit[103]&bit[101]&bit[100]&bit[99]&bit[98]) | (bit[101]&~bit[100]&~bit[99]&~bit[98]) | (bit[102]&bit[101]&~bit[100]&bit[98]) | (bit[100]&bit[99]&~bit[98]) | (~bit[101]&~bit[100]&bit[99]) | (~bit[102]&~bit[101]&bit[99]) | (~bit[102]&~bit[101]&~bit[100]) | (bit[102]&bit[101]&bit[100]&~bit[99]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[103]&~bit[102]&bit[101]&bit[100]&bit[99]&bit[98]) | (~bit[103]&bit[102]&bit[101]&bit[100]&bit[99]&bit[98]) | (bit[102]&~bit[101]&~bit[100]&~bit[99]&~bit[98]) | (~bit[102]&~bit[101]&~bit[100]&bit[99]&bit[98]) | (bit[102]&~bit[101]&bit[100]&bit[98]) | (bit[102]&bit[101]&bit[100]&bit[99]&~bit[98]) | (~bit[102]&~bit[101]&bit[100]&~bit[99]) | (~bit[102]&bit[101]&~bit[100]&~bit[98]) | (~bit[102]&~bit[101]&bit[100]&~bit[98]) | (bit[102]&bit[101]&~bit[100]&bit[98]) | (bit[102]&bit[101]&bit[100]&~bit[99]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[103]&bit[101]&bit[100]&bit[99]&bit[98]) | (~bit[102]&bit[100]&~bit[99]&bit[98]) | (bit[102]&~bit[100]&~bit[99]&bit[98]) | (bit[102]&~bit[101]&bit[100]&bit[99]) | (bit[102]&bit[101]&bit[100]&bit[99]&~bit[98]) | (bit[102]&bit[100]&~bit[99]&~bit[98]) | (~bit[102]&~bit[100]&bit[99]&~bit[98]) | (~bit[102]&bit[101]&~bit[100]&~bit[98]) | (~bit[102]&~bit[101]&bit[100]&~bit[98]) | (bit[102]&bit[101]&~bit[100]&bit[98]) | (bit[102]&bit[101]&bit[100]&~bit[99]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[97]);
		control=control & bit[97];

		//-------------98--------------

		out[6] = (~disable_update & ((~bit[104]&~bit[103]&bit[102]&bit[101]&bit[100]&bit[99]) | (~bit[104]&bit[103]&bit[102]&bit[101]&bit[100]&bit[99]) | (bit[104]&bit[102]&bit[101]&bit[100]&bit[99]) | (bit[103]&~bit[102]&~bit[101]&~bit[100]&~bit[99]) | (~bit[103]&~bit[101]&~bit[100]&~bit[99]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((~bit[103]&~bit[102]&~bit[101]&~bit[100]&bit[99]) | (~bit[103]&bit[102]&bit[101]&bit[100]&~bit[99]) | (bit[103]&bit[102]&bit[101]&bit[100]&~bit[99]) | (~bit[103]&bit[102]&~bit[101]&bit[100]&~bit[99]) | (~bit[103]&~bit[102]&bit[100]&~bit[99]) | (bit[103]&~bit[102]&bit[101]&~bit[99]) | (bit[101]&~bit[100]&~bit[99]) | (bit[103]&~bit[102]&~bit[101]&bit[100]&~bit[99]) | (bit[103]&bit[102]&~bit[101]&~bit[99]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[103]&bit[102]&~bit[101]&bit[100]&~bit[99]) | (~bit[103]&bit[102]&~bit[101]&~bit[100]&bit[99]) | (~bit[103]&~bit[102]&bit[100]&~bit[99]) | (~bit[103]&bit[102]&bit[101]&~bit[100]) | (~bit[102]&bit[101]&~bit[100]) | (bit[103]&~bit[102]&~bit[100]&bit[99]) | (bit[103]&bit[102]&~bit[101]&~bit[100]) | (bit[103]&~bit[102]&~bit[101]&bit[100]&~bit[99]) | (bit[103]&bit[101]&~bit[100]) | (bit[103]&bit[102]&~bit[101]&~bit[99]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[104]&~bit[103]&bit[102]&bit[101]&bit[100]&bit[99]) | (bit[103]&bit[102]&~bit[100]&~bit[99]) | (~bit[103]&bit[102]&bit[101]&bit[100]&~bit[99]) | (bit[103]&~bit[102]&~bit[101]&bit[99]) | (~bit[103]&~bit[102]&~bit[101]&bit[100]) | (~bit[103]&bit[102]&~bit[101]&~bit[100]&bit[99]) | (~bit[103]&~bit[101]&bit[100]&bit[99]) | (bit[103]&~bit[102]&bit[101]&~bit[99]) | (bit[101]&~bit[100]&~bit[99]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[104]&bit[103]&bit[102]&bit[101]&bit[100]&bit[99]) | (bit[104]&bit[102]&bit[101]&bit[100]&bit[99]) | (bit[103]&bit[102]&~bit[101]&bit[100]&bit[99]) | (bit[103]&bit[102]&bit[101]&bit[100]&~bit[99]) | (~bit[103]&bit[102]&~bit[101]&bit[100]&~bit[99]) | (~bit[103]&bit[102]&~bit[101]&~bit[100]&bit[99]) | (~bit[102]&~bit[101]&bit[99]) | (~bit[103]&~bit[102]&~bit[100]) | (bit[103]&bit[102]&~bit[101]&~bit[100]) | (bit[103]&~bit[102]&~bit[101]&bit[100]&~bit[99]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[104]&bit[103]&bit[102]&bit[101]&bit[100]&bit[99]) | (bit[103]&~bit[102]&~bit[101]&~bit[100]&~bit[99]) | (bit[104]&~bit[103]&bit[100]&bit[99]) | (bit[103]&bit[102]&bit[101]&bit[100]&~bit[99]) | (~bit[103]&~bit[102]&bit[100]&bit[99]) | (~bit[103]&bit[102]&bit[101]&~bit[100]) | (~bit[103]&~bit[101]&bit[100]&bit[99]) | (bit[103]&~bit[102]&bit[101]&~bit[99]) | (bit[103]&~bit[102]&~bit[100]&bit[99]) | (bit[103]&bit[102]&~bit[101]&~bit[100]) | (bit[103]&~bit[102]&~bit[101]&bit[100]&~bit[99]) | (bit[103]&bit[102]&~bit[101]&~bit[99]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[104]&~bit[103]&bit[102]&bit[101]&bit[100]&bit[99]) | (bit[104]&bit[102]&bit[101]&bit[100]&bit[99]) | (~bit[103]&~bit[102]&~bit[101]&~bit[100]&bit[99]) | (bit[103]&bit[102]&~bit[101]&bit[100]&bit[99]) | (~bit[103]&bit[102]&bit[101]&bit[100]&~bit[99]) | (bit[102]&~bit[101]&~bit[100]&~bit[99]) | (bit[103]&~bit[102]&bit[101]) | (bit[103]&~bit[102]&~bit[100]&bit[99]) | (bit[103]&~bit[102]&~bit[101]&bit[100]&~bit[99]) | (bit[103]&bit[101]&~bit[100]) | (bit[103]&bit[102]&~bit[101]&~bit[99]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[98]);
		control=control & bit[98];

		//-------------99--------------

		out[6] = (~disable_update & ((~bit[105]&bit[104]&bit[103]&bit[102]&bit[101]&bit[100]) | (~bit[105]&~bit[104]&bit[103]&bit[102]&bit[101]&bit[100]) | (bit[105]&~bit[104]&bit[103]&bit[102]&bit[101]&bit[100]) | (~bit[103]&~bit[102]&~bit[101]&~bit[100]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((~bit[104]&~bit[103]&~bit[102]&~bit[101]&bit[100]) | (bit[104]&~bit[103]&bit[102]&bit[101]&~bit[100]) | (bit[104]&bit[103]&bit[102]&bit[101]&~bit[100]) | (bit[105]&bit[104]&bit[103]&bit[102]&bit[101]&bit[100]) | (~bit[103]&~bit[102]&bit[101]&~bit[100]) | (~bit[104]&bit[102]&bit[101]&~bit[100]) | (~bit[103]&bit[102]&~bit[101]&~bit[100]) | (bit[103]&bit[102]&~bit[101]&~bit[100]) | (bit[103]&~bit[102]&~bit[100]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[105]&bit[104]&bit[103]&bit[102]&bit[101]&bit[100]) | (~bit[103]&~bit[102]&bit[101]&~bit[100]) | (~bit[103]&bit[102]&~bit[101]&bit[100]) | (bit[104]&~bit[103]&~bit[102]&~bit[101]&bit[100]) | (~bit[104]&bit[103]&~bit[101]&bit[100]) | (bit[103]&~bit[102]&~bit[101]) | (~bit[103]&bit[102]&~bit[101]&~bit[100]) | (bit[103]&bit[102]&~bit[101]&~bit[100]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[105]&~bit[104]&bit[103]&bit[102]&bit[101]&bit[100]) | (bit[104]&bit[103]&bit[102]&~bit[101]&bit[100]) | (bit[104]&~bit[103]&bit[102]&bit[101]&~bit[100]) | (bit[105]&bit[104]&bit[103]&bit[102]&bit[101]&bit[100]) | (bit[104]&~bit[103]&~bit[102]&~bit[101]&bit[100]) | (~bit[103]&~bit[102]&bit[101]&bit[100]) | (~bit[104]&bit[102]&bit[101]&~bit[100]) | (~bit[104]&bit[103]&~bit[102]) | (bit[103]&~bit[102]&~bit[101]) | (~bit[103]&bit[102]&~bit[101]&~bit[100]) | (bit[103]&~bit[102]&~bit[100]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[105]&~bit[104]&bit[103]&bit[102]&bit[101]&bit[100]) | (bit[104]&bit[103]&bit[102]&~bit[101]&bit[100]) | (bit[104]&bit[103]&~bit[102]&bit[101]) | (bit[104]&bit[103]&bit[102]&bit[101]&~bit[100]) | (~bit[104]&bit[103]&~bit[101]&~bit[100]) | (bit[105]&bit[104]&bit[103]&bit[102]&bit[101]&bit[100]) | (~bit[103]&bit[102]&~bit[101]&bit[100]) | (bit[104]&~bit[103]&~bit[102]&~bit[101]&bit[100]) | (bit[103]&~bit[102]&~bit[100]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[105]&bit[104]&bit[103]&bit[102]&bit[101]&bit[100]) | (bit[105]&~bit[104]&bit[103]&bit[102]&bit[101]&bit[100]) | (bit[104]&bit[103]&bit[102]&bit[101]&~bit[100]) | (bit[105]&bit[104]&bit[103]&bit[102]&bit[101]&bit[100]) | (~bit[104]&bit[103]&~bit[102]&~bit[101]) | (bit[104]&~bit[103]&~bit[102]&~bit[101]&bit[100]) | (~bit[103]&~bit[102]&bit[101]&bit[100]) | (~bit[104]&~bit[103]&~bit[100]) | (~bit[104]&~bit[103]&bit[102]) | (~bit[103]&bit[102]&~bit[101]&~bit[100]) | (bit[103]&bit[102]&~bit[101]&~bit[100]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[105]&bit[104]&bit[103]&bit[102]&bit[101]&bit[100]) | (~bit[105]&~bit[104]&bit[103]&bit[102]&bit[101]&bit[100]) | (~bit[104]&~bit[103]&~bit[102]&bit[101]&bit[100]) | (bit[105]&~bit[104]&bit[103]&bit[102]&bit[101]&bit[100]) | (~bit[104]&bit[103]&~bit[102]&bit[101]&~bit[100]) | (bit[104]&~bit[103]&bit[102]&bit[101]&~bit[100]) | (~bit[104]&bit[102]&~bit[101]&~bit[100]) | (bit[105]&bit[104]&bit[103]&bit[102]&bit[101]&bit[100]) | (~bit[104]&bit[103]&~bit[101]&bit[100]) | (bit[103]&bit[102]&~bit[101]&~bit[100]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[99]);
		control=control & bit[99];

		//-------------100--------------

		out[6] = (~disable_update & ((bit[104]&~bit[103]&bit[102]&bit[101]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((bit[103]&bit[102]&bit[101]) | (~bit[102]&~bit[101]) | (~bit[103]&~bit[101]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[105]&~bit[104]&bit[103]&~bit[101]) | (bit[104]&~bit[103]&~bit[102]&bit[101]) | (~bit[103]&~bit[102]&~bit[101]) | (~bit[104]&~bit[103]&~bit[102]) | (bit[103]&bit[102]&bit[101]) | (bit[103]&bit[102]&~bit[101]) | (~bit[105]&bit[103]&~bit[101]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[106]&~bit[105]&bit[104]&~bit[103]&bit[102]&bit[101]) | (bit[105]&bit[104]&bit[103]&~bit[102]&~bit[101]) | (~bit[105]&~bit[104]&~bit[103]&~bit[101]) | (~bit[104]&bit[103]&bit[101]) | (~bit[103]&~bit[102]&~bit[101]) | (bit[103]&bit[102]&~bit[101]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[106]&~bit[105]&bit[103]&bit[102]&bit[101]) | (bit[105]&bit[104]&bit[103]&~bit[102]&~bit[101]) | (bit[105]&~bit[104]&~bit[103]&bit[102]&~bit[101]) | (bit[106]&~bit[105]&bit[104]&~bit[103]&bit[102]) | (~bit[105]&bit[104]&~bit[103]&bit[102]&~bit[101]) | (~bit[105]&bit[104]&bit[103]&bit[101]) | (~bit[104]&bit[103]&~bit[101]) | (~bit[104]&~bit[103]&~bit[102]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[105]&~bit[104]&~bit[103]&bit[102]&~bit[101]) | (bit[105]&bit[104]&bit[103]&~bit[102]&bit[101]) | (~bit[105]&~bit[104]&bit[102]&bit[101]) | (~bit[106]&bit[104]&bit[103]&bit[101]) | (~bit[105]&bit[104]&~bit[103]&~bit[102]) | (~bit[105]&bit[104]&bit[103]&bit[101]) | (~bit[105]&bit[103]&~bit[101]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[106]&~bit[105]&bit[104]&bit[103]&bit[102]&bit[101]) | (~bit[106]&bit[105]&~bit[104]&bit[103]&bit[102]) | (~bit[106]&bit[105]&bit[104]&~bit[103]&bit[101]) | (bit[106]&~bit[105]&bit[104]&~bit[103]&bit[102]) | (bit[105]&bit[104]&bit[103]&~bit[102]&bit[101]) | (~bit[105]&bit[104]&~bit[103]&bit[102]&~bit[101]) | (~bit[105]&~bit[104]&~bit[102]&bit[101]) | (bit[105]&~bit[104]&bit[103]&~bit[101]) | (~bit[105]&~bit[104]&~bit[103]&~bit[101]) | (~bit[105]&bit[104]&~bit[103]&~bit[102]) | (bit[104]&~bit[103]&~bit[102]&bit[101]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[100]);
		control=control & bit[100];

		//-------------101--------------

		out[6] = (~disable_update & ((~bit[107]&bit[106]&bit[105]&~bit[104]&bit[103]&bit[102]) | (bit[107]&bit[106]&bit[105]&~bit[104]&bit[103]&bit[102]) | (~bit[105]&bit[104]&bit[103]&bit[102]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((~bit[107]&bit[105]&bit[104]&bit[103]&bit[102]) | (bit[106]&~bit[105]&~bit[104]&~bit[102]) | (bit[107]&bit[105]&bit[104]&bit[103]&bit[102]) | (~bit[106]&~bit[104]&~bit[102]) | (~bit[103]&~bit[102]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[107]&bit[105]&bit[104]&bit[103]&bit[102]) | (~bit[106]&~bit[104]&~bit[103]&bit[102]) | (bit[107]&bit[105]&bit[104]&bit[103]&bit[102]) | (~bit[105]&~bit[104]&~bit[103]&bit[102]) | (bit[106]&~bit[105]&bit[104]&bit[103]&~bit[102]) | (bit[106]&bit[105]&bit[103]&~bit[102]) | (~bit[106]&bit[104]&bit[103]&~bit[102]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[107]&~bit[106]&bit[105]&bit[104]&bit[103]) | (~bit[107]&bit[106]&bit[105]&~bit[104]&bit[103]&bit[102]) | (bit[106]&bit[105]&~bit[104]&~bit[103]&bit[102]) | (~bit[106]&~bit[104]&~bit[103]&~bit[102]) | (bit[106]&~bit[105]&bit[104]&bit[103]&~bit[102]) | (~bit[105]&bit[104]&~bit[103]&bit[102]) | (~bit[105]&~bit[103]&~bit[102]) | (bit[106]&bit[105]&~bit[104]&~bit[102]) | (~bit[106]&bit[104]&bit[103]&~bit[102]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[107]&bit[106]&bit[105]&~bit[104]&bit[103]&bit[102]) | (~bit[107]&bit[105]&bit[104]&bit[103]&bit[102]) | (bit[106]&bit[105]&~bit[104]&~bit[103]&bit[102]) | (~bit[106]&bit[104]&bit[103]&bit[102]) | (~bit[106]&~bit[104]&~bit[103]&~bit[102]) | (bit[106]&~bit[105]&bit[104]&bit[103]&~bit[102]) | (~bit[105]&~bit[104]&~bit[103]&~bit[102]) | (bit[105]&bit[104]&~bit[103]&~bit[102]) | (bit[105]&bit[104]&~bit[103]&bit[102]) | (~bit[106]&~bit[105]&bit[103]) | (bit[106]&bit[105]&bit[103]&~bit[102]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[107]&bit[106]&~bit[105]&bit[104]&bit[102]) | (~bit[107]&bit[106]&bit[105]&~bit[104]&bit[103]&bit[102]) | (bit[107]&bit[106]&bit[105]&~bit[104]&bit[103]&bit[102]) | (bit[106]&~bit[105]&~bit[104]&~bit[102]) | (~bit[106]&bit[105]&bit[104]&~bit[103]) | (bit[107]&bit[105]&bit[104]&bit[103]&bit[102]) | (~bit[105]&~bit[104]&~bit[103]&~bit[102]) | (~bit[105]&bit[104]&~bit[103]&bit[102]) | (bit[105]&bit[104]&~bit[103]&~bit[102]) | (~bit[106]&~bit[105]&~bit[103]&bit[102]) | (bit[106]&bit[105]&~bit[104]&~bit[102]) | (bit[106]&bit[105]&bit[103]&~bit[102]) | (~bit[106]&bit[104]&bit[103]&~bit[102]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[107]&bit[106]&bit[105]&~bit[104]&bit[103]&bit[102]) | (~bit[106]&bit[105]&bit[104]) | (~bit[107]&~bit[106]&bit[104]&bit[102]) | (bit[106]&bit[105]&~bit[104]&~bit[103]&bit[102]) | (bit[106]&~bit[105]&bit[103]&bit[102]) | (bit[107]&bit[105]&bit[104]&bit[103]&bit[102]) | (~bit[105]&~bit[104]&~bit[103]&bit[102]) | (bit[106]&~bit[105]&bit[104]&bit[103]&~bit[102]) | (~bit[106]&~bit[105]&~bit[103]&bit[102]) | (bit[105]&bit[104]&~bit[103]&bit[102]) | (bit[106]&bit[105]&~bit[104]&~bit[102]) | (~bit[106]&~bit[103]&~bit[102]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[101]);
		control=control & bit[101];

		//-------------102--------------

		out[6] = (~disable_update & ((bit[108]&bit[107]&bit[106]&~bit[105]&bit[104]&bit[103]) | (~bit[108]&bit[107]&bit[106]&~bit[105]&bit[104]&bit[103]) | (~bit[107]&~bit[106]&bit[105]&bit[104]&bit[103]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((~bit[108]&bit[106]&bit[105]&bit[104]&bit[103]) | (~bit[107]&~bit[106]&~bit[105]&bit[104]&~bit[103]) | (bit[108]&bit[106]&bit[105]&bit[104]&bit[103]) | (~bit[107]&bit[106]&~bit[105]&~bit[103]) | (bit[107]&~bit[105]&bit[104]&~bit[103]) | (bit[107]&bit[105]&bit[104]&bit[103]) | (~bit[104]&~bit[103]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[106]&~bit[105]&~bit[104]) | (bit[107]&bit[105]&bit[104]&bit[103]) | (~bit[107]&bit[106]&bit[105]&bit[104]) | (bit[106]&~bit[105]&~bit[104]&bit[103]) | (~bit[106]&~bit[104]&bit[103]) | (bit[105]&bit[104]&~bit[103]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[108]&bit[107]&bit[106]&~bit[105]&bit[104]&bit[103]) | (~bit[107]&~bit[106]&~bit[105]&bit[104]&~bit[103]) | (~bit[106]&~bit[105]&~bit[104]&bit[103]) | (bit[107]&~bit[106]&bit[105]&bit[104]) | (~bit[106]&bit[105]&~bit[103]) | (~bit[107]&bit[106]&bit[105]&bit[104]) | (bit[106]&bit[105]&~bit[104]) | (bit[106]&~bit[105]&~bit[104]&~bit[103]) | (bit[105]&bit[104]&~bit[103]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[108]&bit[107]&bit[106]&~bit[105]&bit[104]&bit[103]) | (~bit[108]&bit[106]&bit[105]&bit[104]&bit[103]) | (~bit[107]&~bit[106]&~bit[105]&bit[104]&bit[103]) | (bit[107]&~bit[106]&bit[104]&~bit[103]) | (~bit[107]&bit[106]&~bit[105]&~bit[103]) | (bit[107]&~bit[106]&bit[105]&bit[104]) | (~bit[106]&bit[105]&~bit[103]) | (bit[106]&~bit[105]&~bit[104]&bit[103]) | (bit[106]&~bit[105]&~bit[104]&~bit[103]) | (bit[105]&bit[104]&~bit[103]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[108]&bit[107]&bit[106]&~bit[105]&bit[104]&bit[103]) | (~bit[108]&~bit[106]&bit[105]&bit[104]&bit[103]) | (bit[108]&bit[106]&bit[105]&bit[104]&bit[103]) | (~bit[107]&~bit[106]&~bit[105]&bit[104]&bit[103]) | (~bit[107]&~bit[105]&~bit[104]&~bit[103]) | (~bit[106]&bit[105]&bit[104]&~bit[103]) | (bit[107]&~bit[105]&bit[104]&~bit[103]) | (bit[107]&bit[105]&bit[104]&bit[103]) | (~bit[107]&bit[106]&~bit[104]) | (bit[106]&~bit[105]&~bit[104]&~bit[103]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[107]&~bit[106]&~bit[105]&bit[104]&bit[103]) | (~bit[108]&~bit[106]&bit[105]&bit[104]&bit[103]) | (bit[108]&bit[106]&bit[105]&bit[104]&bit[103]) | (bit[107]&bit[106]&bit[105]&~bit[104]) | (~bit[107]&bit[106]&~bit[104]&~bit[103]) | (~bit[107]&~bit[106]&~bit[104]&bit[103]) | (~bit[107]&bit[106]&bit[105]&bit[104]) | (bit[106]&~bit[105]&~bit[104]&bit[103]) | (~bit[107]&bit[105]&~bit[103]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[102]);
		control=control & bit[102];

		//-------------103--------------

		out[6] = (~disable_update & ((bit[108]&~bit[107]&~bit[106]&bit[105]&bit[104]) | (bit[107]&~bit[106]&bit[105]&bit[104]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((bit[109]&bit[108]&bit[107]&bit[106]&bit[105]&bit[104]) | (~bit[108]&~bit[107]&~bit[106]&~bit[104]) | (~bit[109]&bit[107]&bit[106]&bit[105]&bit[104]) | (~bit[108]&bit[107]&bit[106]&bit[105]&bit[104]) | (~bit[107]&bit[106]&bit[105]&bit[104]) | (~bit[105]&~bit[104]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[108]&~bit[107]&~bit[106]&~bit[105]&~bit[104]) | (bit[108]&bit[107]&~bit[106]&bit[105]&~bit[104]) | (~bit[108]&~bit[107]&bit[106]&bit[105]&~bit[104]) | (bit[108]&~bit[107]&bit[105]&~bit[104]) | (~bit[107]&bit[106]&bit[105]&bit[104]) | (~bit[108]&bit[107]&bit[105]&~bit[104]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[109]&bit[108]&bit[107]&bit[106]&bit[105]&bit[104]) | (~bit[109]&~bit[108]&bit[106]&bit[105]&bit[104]) | (~bit[109]&bit[107]&bit[106]&bit[105]&bit[104]) | (bit[108]&bit[107]&bit[106]&bit[105]&~bit[104]) | (~bit[108]&~bit[107]&~bit[105]&bit[104]) | (~bit[108]&bit[107]&bit[106]&bit[105]&bit[104]) | (bit[108]&~bit[107]&~bit[106]&~bit[104]) | (~bit[108]&bit[107]&~bit[106]&~bit[104]) | (bit[108]&~bit[106]&~bit[105]&bit[104]) | (bit[107]&~bit[106]&~bit[105]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[109]&~bit[108]&~bit[107]&bit[106]&bit[105]&bit[104]) | (bit[108]&~bit[107]&~bit[106]&bit[105]&bit[104]) | (~bit[109]&bit[107]&bit[106]&bit[105]&bit[104]) | (bit[108]&bit[107]&~bit[106]&bit[105]&~bit[104]) | (~bit[108]&bit[106]&~bit[105]&~bit[104]) | (bit[108]&bit[107]&bit[106]&bit[105]&~bit[104]) | (~bit[107]&~bit[106]&~bit[105]&bit[104]) | (bit[107]&bit[106]&~bit[105]&bit[104]) | (~bit[108]&~bit[107]&bit[106]&bit[105]&~bit[104]) | (~bit[108]&bit[107]&bit[106]&bit[105]&bit[104]) | (bit[108]&~bit[107]&~bit[105]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[108]&~bit[107]&bit[106]&~bit[105]&~bit[104]) | (bit[108]&~bit[107]&bit[106]&~bit[105]&bit[104]) | (bit[109]&bit[108]&bit[107]&bit[106]&bit[105]&bit[104]) | (bit[109]&~bit[108]&~bit[107]&bit[106]&bit[105]&bit[104]) | (~bit[109]&~bit[108]&bit[106]&bit[105]&bit[104]) | (~bit[109]&bit[108]&~bit[107]&bit[105]&bit[104]) | (bit[108]&bit[107]&~bit[106]&bit[105]&~bit[104]) | (bit[108]&bit[107]&bit[106]&bit[105]&~bit[104]) | (bit[108]&~bit[107]&~bit[106]&~bit[104]) | (bit[108]&~bit[107]&bit[105]&~bit[104]) | (~bit[108]&bit[107]&bit[105]&bit[104]) | (~bit[108]&~bit[106]&bit[104]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[109]&bit[108]&bit[107]&bit[106]&bit[105]&bit[104]) | (bit[109]&~bit[108]&~bit[107]&bit[106]&bit[105]&bit[104]) | (~bit[108]&~bit[107]&~bit[106]&~bit[105]&~bit[104]) | (~bit[108]&bit[107]&bit[106]&~bit[105]&bit[104]) | (~bit[108]&~bit[107]&~bit[106]&bit[105]&bit[104]) | (~bit[109]&bit[108]&~bit[107]&bit[105]&bit[104]) | (bit[108]&bit[107]&bit[106]&~bit[104]) | (~bit[109]&bit[107]&bit[105]&bit[104]) | (~bit[108]&~bit[107]&bit[106]&bit[105]&~bit[104]) | (~bit[108]&bit[107]&~bit[106]&~bit[104]) | (bit[108]&~bit[106]&~bit[105]&bit[104]) | (~bit[108]&bit[107]&bit[105]&~bit[104]) | (bit[108]&~bit[107]&~bit[105]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[103]);
		control=control & bit[103];

		//-------------104--------------

		out[6] = (~disable_update & ((~bit[108]&~bit[107]&bit[106]&bit[105]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((~bit[109]&~bit[108]&bit[107]&~bit[106]&~bit[105]) | (~bit[109]&~bit[108]&~bit[107]&~bit[106]&~bit[105]) | (bit[110]&bit[109]&bit[108]&bit[106]&bit[105]) | (bit[110]&~bit[109]&bit[108]&bit[107]&bit[106]&bit[105]) | (~bit[110]&bit[109]&bit[108]&bit[106]&bit[105]) | (~bit[110]&~bit[109]&bit[108]&bit[106]&bit[105]) | (~bit[108]&bit[107]&bit[106]&bit[105]) | (bit[109]&~bit[108]&~bit[107]&~bit[106]&~bit[105]) | (bit[108]&~bit[107]&~bit[106]&~bit[105]) | (bit[108]&~bit[107]&bit[106]&bit[105]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[109]&bit[108]&~bit[107]&bit[106]&~bit[105]) | (bit[109]&bit[108]&~bit[107]&bit[106]&~bit[105]) | (bit[108]&bit[107]&~bit[106]&~bit[105]) | (~bit[108]&bit[107]&bit[106]&~bit[105]) | (~bit[108]&bit[107]&bit[106]&bit[105]) | (~bit[108]&~bit[107]&bit[106]&~bit[105]) | (bit[108]&~bit[107]&bit[106]&bit[105]) | (bit[109]&bit[107]&~bit[106]&~bit[105]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[109]&~bit[108]&~bit[107]&~bit[106]&~bit[105]) | (bit[110]&~bit[109]&bit[108]&bit[107]&bit[106]&bit[105]) | (bit[109]&bit[108]&~bit[107]&~bit[106]&bit[105]) | (~bit[110]&bit[109]&bit[108]&bit[106]&bit[105]) | (~bit[109]&bit[108]&~bit[107]&~bit[106]&bit[105]) | (bit[108]&bit[107]&~bit[106]&~bit[105]) | (~bit[110]&~bit[109]&bit[108]&bit[106]&bit[105]) | (~bit[109]&~bit[108]&~bit[106]&bit[105]) | (~bit[109]&bit[108]&~bit[107]&bit[106]&bit[105]) | (~bit[109]&~bit[108]&~bit[107]&bit[106]&bit[105]) | (~bit[108]&~bit[107]&bit[106]&~bit[105]) | (~bit[108]&~bit[107]&~bit[106]&bit[105]) | (bit[109]&bit[107]&~bit[106]&~bit[105]) | (bit[108]&bit[107]&bit[106]&~bit[105]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[110]&~bit[109]&~bit[108]&bit[107]&bit[106]&bit[105]) | (~bit[110]&bit[109]&~bit[108]&bit[105]) | (~bit[110]&~bit[109]&bit[107]&bit[106]&bit[105]) | (bit[110]&bit[109]&bit[108]&bit[106]&bit[105]) | (bit[109]&bit[108]&~bit[107]&bit[106]&~bit[105]) | (~bit[109]&~bit[107]&bit[106]&~bit[105]) | (~bit[109]&bit[108]&~bit[107]&bit[106]&bit[105]) | (bit[109]&~bit[108]&~bit[107]&~bit[106]&~bit[105]) | (~bit[109]&bit[108]&bit[107]&~bit[106]) | (bit[109]&~bit[108]&bit[107]&~bit[106]) | (~bit[108]&~bit[107]&~bit[106]&bit[105]) | (bit[109]&bit[107]&~bit[106]&~bit[105]) | (bit[108]&bit[107]&bit[106]&~bit[105]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[110]&~bit[109]&bit[107]&bit[106]&bit[105]) | (bit[110]&bit[109]&bit[108]&bit[106]&bit[105]) | (bit[110]&~bit[109]&bit[108]&bit[107]&bit[106]&bit[105]) | (~bit[109]&bit[108]&~bit[107]&bit[106]&~bit[105]) | (bit[108]&bit[107]&~bit[106]&bit[105]) | (~bit[109]&bit[108]&~bit[107]&~bit[106]&bit[105]) | (~bit[110]&~bit[109]&bit[108]&bit[106]&bit[105]) | (~bit[109]&~bit[108]&~bit[107]&bit[106]&bit[105]) | (~bit[108]&bit[107]&bit[106]&~bit[105]) | (bit[109]&~bit[108]&~bit[107]&~bit[106]&~bit[105]) | (bit[108]&~bit[107]&~bit[106]&~bit[105]) | (~bit[109]&bit[108]&bit[107]&~bit[106]) | (bit[109]&~bit[108]&bit[107]&~bit[106]) | (bit[108]&bit[107]&bit[106]&~bit[105]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[110]&~bit[108]&~bit[107]&bit[106]&bit[105]) | (bit[110]&~bit[109]&~bit[108]&bit[107]&bit[106]&bit[105]) | (~bit[109]&bit[108]&~bit[107]&~bit[106]&~bit[105]) | (~bit[109]&~bit[108]&bit[107]&~bit[106]&~bit[105]) | (~bit[109]&~bit[108]&~bit[107]&~bit[106]&bit[105]) | (bit[110]&~bit[109]&bit[108]&bit[107]&bit[106]&bit[105]) | (bit[109]&bit[108]&~bit[107]&~bit[106]&bit[105]) | (~bit[110]&bit[109]&bit[108]&bit[106]&bit[105]) | (bit[109]&bit[108]&~bit[107]&bit[106]&~bit[105]) | (~bit[109]&bit[107]&bit[106]&~bit[105]) | (bit[109]&~bit[107]&bit[106]&bit[105]) | (bit[109]&~bit[108]&~bit[107]&~bit[106]&~bit[105]) | (bit[109]&~bit[108]&bit[107]&~bit[106]) | (bit[109]&bit[107]&~bit[106]&~bit[105]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[104]);
		control=control & bit[104];

		//-------------105--------------

		out[6] = (~disable_update & ((bit[110]&~bit[108]&bit[107]&bit[106]) | (bit[109]&~bit[108]&bit[107]&bit[106]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((bit[111]&bit[110]&~bit[109]&bit[108]&bit[107]&bit[106]) | (~bit[111]&~bit[110]&bit[108]&bit[107]&bit[106]) | (bit[111]&~bit[110]&bit[108]&bit[107]&bit[106]) | (~bit[111]&bit[110]&bit[108]&bit[107]&bit[106]) | (~bit[109]&~bit[108]&~bit[107]&~bit[106]) | (bit[110]&bit[109]&bit[108]&bit[107]&bit[106]) | (~bit[110]&~bit[108]&~bit[107]&~bit[106]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[111]&~bit[110]&bit[108]&bit[107]&bit[106]) | (~bit[110]&~bit[109]&~bit[108]&bit[107]&~bit[106]) | (bit[110]&bit[109]&bit[108]&~bit[107]&~bit[106]) | (bit[109]&~bit[108]&bit[107]&~bit[106]) | (bit[110]&bit[109]&~bit[108]&~bit[106]) | (~bit[110]&bit[109]&bit[108]&~bit[107]&~bit[106]) | (bit[110]&~bit[109]&~bit[108]&bit[107]&~bit[106]) | (~bit[109]&bit[108]&~bit[107]&~bit[106]) | (~bit[109]&bit[108]&bit[107]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[111]&~bit[110]&~bit[109]&bit[108]&bit[107]&bit[106]) | (~bit[111]&~bit[110]&bit[109]&~bit[108]&bit[106]) | (bit[111]&~bit[110]&bit[108]&bit[107]&bit[106]) | (~bit[110]&~bit[109]&~bit[108]&bit[107]&~bit[106]) | (~bit[110]&~bit[109]&~bit[108]&~bit[107]&bit[106]) | (~bit[110]&bit[109]&~bit[108]&~bit[107]&bit[106]) | (bit[110]&bit[109]&~bit[107]&~bit[106]) | (~bit[110]&bit[109]&bit[108]&~bit[107]&~bit[106]) | (bit[110]&bit[109]&bit[108]&bit[107]&bit[106]) | (bit[110]&~bit[109]&~bit[108]&bit[106]) | (~bit[109]&bit[108]&~bit[107]&~bit[106]) | (bit[109]&bit[108]&bit[107]&~bit[106]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[111]&bit[110]&bit[108]&bit[107]&bit[106]) | (~bit[110]&~bit[109]&~bit[108]&~bit[107]&bit[106]) | (bit[110]&~bit[109]&bit[108]&~bit[107]&bit[106]) | (bit[109]&~bit[108]&bit[107]&~bit[106]) | (bit[111]&~bit[110]&bit[109]&bit[107]&bit[106]) | (~bit[109]&~bit[108]&~bit[107]&~bit[106]) | (~bit[110]&bit[108]&~bit[107]&bit[106]) | (bit[110]&bit[109]&bit[108]&bit[107]&bit[106]) | (bit[110]&~bit[109]&~bit[108]&bit[107]&~bit[106]) | (~bit[109]&bit[108]&~bit[107]&~bit[106]) | (bit[110]&bit[109]&~bit[108]) | (bit[109]&bit[108]&bit[107]&~bit[106]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[111]&bit[110]&~bit[109]&bit[108]&bit[107]&bit[106]) | (~bit[111]&bit[110]&bit[109]&bit[108]&bit[106]) | (~bit[110]&~bit[109]&bit[108]&~bit[107]&bit[106]) | (bit[109]&~bit[108]&~bit[107]&~bit[106]) | (bit[111]&~bit[110]&bit[109]&bit[107]&bit[106]) | (~bit[110]&bit[109]&bit[108]&~bit[107]&~bit[106]) | (~bit[110]&bit[108]&bit[107]&~bit[106]) | (bit[110]&~bit[109]&~bit[108]&bit[106]) | (bit[110]&~bit[109]&~bit[108]&bit[107]&~bit[106]) | (bit[110]&bit[109]&~bit[107]&bit[106]) | (bit[109]&bit[108]&bit[107]&~bit[106]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[111]&~bit[110]&~bit[109]&bit[108]&bit[107]&bit[106]) | (bit[111]&bit[110]&~bit[109]&bit[108]&bit[107]&bit[106]) | (~bit[111]&~bit[110]&bit[109]&~bit[108]&bit[106]) | (~bit[111]&bit[110]&~bit[108]&bit[107]&bit[106]) | (bit[110]&bit[109]&bit[108]&~bit[107]&~bit[106]) | (bit[110]&~bit[109]&bit[108]&~bit[107]&bit[106]) | (~bit[110]&bit[109]&~bit[108]&~bit[107]&bit[106]) | (~bit[110]&~bit[109]&~bit[107]&~bit[106]) | (bit[111]&~bit[110]&bit[109]&bit[107]&bit[106]) | (~bit[110]&bit[109]&bit[107]&~bit[106]) | (~bit[110]&~bit[108]&~bit[107]&~bit[106]) | (bit[110]&~bit[109]&~bit[108]&bit[107]&~bit[106]) | (bit[110]&bit[109]&~bit[107]&bit[106]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[105]);
		control=control & bit[105];

		//-------------106--------------

		out[6] = (~disable_update & ((bit[111]&bit[110]&bit[109]&~bit[108]&bit[107]) | (~bit[110]&~bit[109]&bit[108]&bit[107]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((bit[111]&~bit[110]&bit[109]&~bit[108]&~bit[107]) | (~bit[112]&bit[111]&bit[110]&bit[108]&bit[107]) | (~bit[112]&~bit[111]&bit[110]&bit[108]&bit[107]) | (~bit[110]&bit[109]&bit[108]&bit[107]) | (bit[111]&bit[110]&~bit[109]&~bit[108]&~bit[107]) | (~bit[110]&~bit[109]&~bit[108]&~bit[107]) | (bit[112]&bit[110]&bit[108]&bit[107]) | (~bit[111]&~bit[108]&~bit[107]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[111]&bit[110]&bit[109]&~bit[108]&~bit[107]) | (~bit[112]&~bit[111]&bit[110]&bit[108]&bit[107]) | (~bit[110]&bit[109]&bit[108]&bit[107]) | (~bit[110]&bit[108]&~bit[107]) | (~bit[111]&bit[108]&~bit[107]) | (bit[110]&~bit[109]&bit[108]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[111]&bit[110]&bit[109]&~bit[108]&bit[107]) | (~bit[112]&bit[111]&bit[110]&bit[108]&bit[107]) | (bit[111]&bit[110]&bit[108]&~bit[107]) | (~bit[112]&~bit[111]&~bit[110]&bit[108]) | (~bit[111]&~bit[110]&bit[108]&~bit[107]) | (~bit[111]&~bit[109]&bit[108]&bit[107]) | (bit[112]&bit[110]&bit[108]&bit[107]) | (~bit[110]&~bit[109]&bit[108]&~bit[107]) | (~bit[110]&~bit[109]&~bit[108]) | (~bit[111]&~bit[109]&~bit[107]) | (bit[111]&bit[110]&bit[109]&~bit[107]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[111]&~bit[110]&bit[109]&~bit[108]&~bit[107]) | (bit[112]&~bit[111]&bit[109]&bit[108]&bit[107]) | (~bit[112]&bit[111]&~bit[110]&bit[108]&bit[107]) | (~bit[112]&bit[111]&bit[110]&~bit[108]) | (~bit[111]&bit[110]&~bit[109]&bit[107]) | (bit[111]&bit[110]&~bit[109]&~bit[108]&~bit[107]) | (~bit[110]&~bit[109]&bit[108]&~bit[107]) | (bit[111]&bit[109]&bit[108]) | (~bit[109]&~bit[108]&bit[107]) | (bit[111]&bit[110]&bit[109]&~bit[107]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[112]&bit[111]&bit[110]&bit[109]&~bit[108]) | (bit[112]&~bit[111]&bit[109]&bit[108]&bit[107]) | (~bit[111]&bit[110]&~bit[109]&bit[108]&~bit[107]) | (bit[111]&~bit[110]&bit[109]&~bit[108]&~bit[107]) | (~bit[112]&bit[111]&bit[110]&bit[108]&bit[107]) | (~bit[112]&~bit[111]&bit[110]&bit[108]&bit[107]) | (~bit[112]&~bit[109]&bit[108]&bit[107]) | (~bit[111]&~bit[110]&~bit[108]&bit[107]) | (~bit[111]&~bit[109]&~bit[108]&bit[107]) | (bit[111]&bit[110]&~bit[109]&~bit[108]&~bit[107]) | (~bit[110]&~bit[109]&~bit[108]&~bit[107]) | (bit[111]&bit[110]&bit[109]&~bit[107]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[112]&bit[111]&bit[110]&bit[109]&~bit[108]) | (~bit[112]&bit[111]&~bit[110]&bit[108]&bit[107]) | (bit[111]&~bit[110]&bit[109]&~bit[108]&~bit[107]) | (bit[111]&bit[110]&bit[108]&~bit[107]) | (~bit[110]&bit[109]&~bit[108]&bit[107]) | (bit[112]&~bit[109]&bit[108]&bit[107]) | (~bit[112]&~bit[111]&bit[110]&bit[108]&bit[107]) | (~bit[111]&~bit[110]&~bit[109]&~bit[107]) | (bit[111]&bit[110]&~bit[109]&~bit[108]&~bit[107]) | (bit[112]&bit[110]&bit[108]&bit[107]) | (bit[111]&bit[110]&bit[109]&~bit[107]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[106]);
		control=control & bit[106];

		//-------------107--------------

		out[6] = (~disable_update & ((~bit[110]&bit[109]&bit[108]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((bit[113]&bit[111]&bit[110]&bit[109]&bit[108]) | (~bit[113]&bit[112]&bit[110]&bit[109]&bit[108]) | (~bit[113]&~bit[112]&bit[110]&bit[109]&bit[108]) | (~bit[110]&~bit[109]&~bit[108]) | (~bit[111]&bit[110]&bit[109]&bit[108]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[112]&bit[111]&bit[110]&~bit[109]&~bit[108]) | (bit[113]&bit[111]&bit[110]&bit[109]&bit[108]) | (~bit[113]&bit[112]&bit[110]&bit[109]&bit[108]) | (~bit[113]&~bit[112]&bit[110]&bit[109]&bit[108]) | (~bit[111]&bit[110]&~bit[109]&~bit[108]) | (~bit[112]&bit[110]&~bit[109]&~bit[108]) | (~bit[111]&bit[110]&bit[109]&bit[108]) | (~bit[110]&bit[109]&~bit[108]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[112]&~bit[111]&~bit[110]&bit[109]&bit[108]) | (~bit[113]&~bit[111]&bit[109]&bit[108]) | (~bit[112]&~bit[111]&~bit[110]&~bit[109]) | (~bit[113]&~bit[112]&bit[110]&bit[109]&bit[108]) | (~bit[111]&bit[110]&~bit[109]&~bit[108]) | (~bit[112]&bit[110]&~bit[109]&~bit[108]) | (bit[112]&bit[110]&bit[109]&~bit[108]) | (~bit[112]&bit[110]&bit[109]&~bit[108]) | (~bit[111]&bit[110]&bit[109]&bit[108]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[113]&bit[112]&~bit[111]&~bit[110]) | (bit[112]&~bit[111]&~bit[110]&~bit[108]) | (bit[112]&bit[111]&bit[110]&~bit[109]&~bit[108]) | (bit[113]&~bit[112]&bit[110]&bit[109]&bit[108]) | (~bit[113]&bit[112]&bit[110]&bit[109]&bit[108]) | (~bit[112]&bit[111]&~bit[110]&bit[108]) | (bit[112]&~bit[110]&~bit[109]&bit[108]) | (~bit[112]&bit[110]&bit[109]&~bit[108]) | (~bit[112]&~bit[111]&bit[110]) | (~bit[111]&bit[109]&~bit[108]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[113]&bit[112]&bit[111]&~bit[110]&bit[109]&bit[108]) | (~bit[112]&~bit[111]&bit[110]&bit[109]&~bit[108]) | (bit[113]&bit[111]&bit[110]&bit[109]&bit[108]) | (bit[111]&~bit[109]&~bit[108]) | (~bit[112]&~bit[111]&~bit[110]&bit[109]&bit[108]) | (bit[113]&bit[112]&~bit[111]&bit[108]) | (~bit[113]&~bit[112]&bit[110]&bit[109]&bit[108]) | (bit[112]&~bit[111]&~bit[109]&bit[108]) | (bit[112]&bit[110]&~bit[109]&~bit[108]) | (~bit[112]&bit[111]&~bit[109]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[113]&bit[112]&bit[111]&~bit[110]&bit[109]&bit[108]) | (bit[113]&~bit[112]&bit[110]&bit[109]&bit[108]) | (~bit[113]&~bit[112]&~bit[110]&bit[109]) | (bit[112]&bit[110]&~bit[109]&~bit[108]) | (bit[112]&bit[110]&bit[109]&~bit[108]) | (bit[113]&bit[112]&bit[108]) | (~bit[112]&~bit[110]&~bit[108]) | (bit[112]&~bit[109]&bit[108]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[107]);
		control=control & bit[107];

		//-------------108--------------

		out[6] = (~disable_update & ((~bit[114]&~bit[113]&~bit[112]&bit[111]&bit[110]&bit[109]) | (bit[112]&~bit[111]&bit[110]&bit[109]) | (bit[113]&~bit[112]&~bit[111]&bit[110]&bit[109]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((~bit[114]&bit[112]&bit[111]&bit[110]&bit[109]) | (~bit[114]&bit[113]&~bit[112]&bit[111]&bit[110]&bit[109]) | (bit[114]&~bit[113]&bit[111]&bit[110]&bit[109]) | (bit[113]&bit[112]&bit[111]&bit[110]&bit[109]) | (bit[114]&~bit[112]&bit[111]&bit[110]&bit[109]) | (~bit[110]&~bit[109]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[114]&bit[112]&bit[111]&bit[110]&bit[109]) | (~bit[114]&bit[113]&~bit[112]&bit[111]&bit[110]&bit[109]) | (bit[114]&~bit[113]&bit[111]&bit[110]&bit[109]) | (~bit[112]&bit[111]&bit[110]&~bit[109]) | (bit[112]&bit[111]&bit[110]&~bit[109]) | (bit[113]&bit[112]&bit[111]&bit[110]&bit[109]) | (bit[114]&~bit[112]&bit[111]&bit[110]&bit[109]) | (~bit[111]&bit[110]&~bit[109]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[114]&~bit[113]&bit[112]&bit[110]&bit[109]) | (~bit[114]&bit[113]&~bit[112]&bit[111]&bit[110]&bit[109]) | (bit[113]&bit[112]&~bit[111]&~bit[110]) | (bit[113]&~bit[112]&~bit[111]&bit[110]&bit[109]) | (~bit[113]&~bit[112]&bit[111]&~bit[110]) | (bit[114]&~bit[113]&bit[111]&bit[110]&bit[109]) | (~bit[112]&~bit[110]&~bit[109]) | (bit[113]&~bit[112]&~bit[111]&~bit[110]) | (bit[114]&~bit[112]&bit[111]&bit[110]&bit[109]) | (~bit[113]&~bit[111]&~bit[110]) | (~bit[111]&bit[110]&~bit[109]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[114]&~bit[113]&bit[112]&~bit[111]&bit[109]) | (bit[114]&~bit[113]&~bit[112]&bit[111]&bit[110]) | (~bit[113]&~bit[112]&bit[110]&~bit[109]) | (~bit[113]&bit[112]&bit[111]&~bit[110]&~bit[109]) | (bit[113]&~bit[112]&bit[111]&~bit[110]&bit[109]) | (~bit[114]&bit[113]&bit[112]&bit[110]&bit[109]) | (~bit[113]&bit[112]&bit[111]&~bit[110]&bit[109]) | (~bit[114]&bit[113]&~bit[112]&bit[111]&bit[110]&bit[109]) | (~bit[112]&bit[111]&bit[110]&~bit[109]) | (bit[113]&bit[112]&bit[111]&bit[110]&bit[109]) | (bit[113]&~bit[112]&~bit[111]&~bit[110]) | (~bit[113]&~bit[111]&~bit[110]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[114]&bit[113]&~bit[112]&~bit[111]&bit[110]) | (bit[113]&~bit[112]&~bit[111]&bit[110]&~bit[109]) | (~bit[113]&bit[112]&bit[111]&~bit[110]&~bit[109]) | (bit[113]&~bit[112]&bit[111]&~bit[110]&bit[109]) | (bit[113]&bit[112]&bit[111]&~bit[110]&bit[109]) | (bit[114]&bit[112]&~bit[111]&bit[110]&bit[109]) | (~bit[113]&~bit[112]&~bit[111]&~bit[110]) | (~bit[114]&bit[112]&bit[111]&bit[110]&bit[109]) | (bit[113]&bit[112]&~bit[111]&~bit[110]) | (bit[113]&bit[112]&bit[111]&bit[110]&bit[109]) | (~bit[113]&bit[111]&bit[110]&~bit[109]) | (bit[114]&~bit[112]&bit[111]&bit[110]&bit[109]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[113]&bit[112]&bit[110]&~bit[109]) | (bit[113]&bit[112]&bit[111]&~bit[110]&bit[109]) | (~bit[114]&bit[113]&bit[112]&bit[110]&bit[109]) | (~bit[113]&bit[112]&bit[111]&~bit[110]&bit[109]) | (bit[114]&bit[112]&~bit[111]&bit[110]&bit[109]) | (~bit[114]&bit[113]&~bit[112]&bit[111]&bit[110]&bit[109]) | (bit[113]&~bit[112]&~bit[111]&bit[110]&bit[109]) | (~bit[113]&~bit[112]&bit[111]&~bit[110]) | (bit[114]&~bit[113]&bit[111]&bit[110]&bit[109]) | (bit[112]&bit[111]&bit[110]&~bit[109]) | (bit[113]&~bit[112]&~bit[111]&~bit[110]) | (~bit[113]&bit[111]&bit[110]&~bit[109]) | (bit[114]&~bit[112]&bit[111]&bit[110]&bit[109]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[108]);
		control=control & bit[108];

		//-------------109--------------

		out[6] = (~disable_update & ((~bit[115]&~bit[114]&~bit[113]&bit[112]&bit[111]&bit[110]) | (~bit[114]&bit[113]&~bit[112]&bit[111]&bit[110]) | (bit[114]&bit[113]&~bit[112]&bit[111]&bit[110]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((bit[115]&bit[114]&bit[113]&bit[112]&bit[111]&bit[110]) | (~bit[115]&bit[114]&~bit[113]&bit[112]&bit[111]&bit[110]) | (bit[115]&~bit[114]&bit[112]&bit[111]&bit[110]) | (~bit[115]&bit[113]&bit[112]&bit[111]&bit[110]) | (~bit[114]&~bit[113]&~bit[112]&bit[111]&~bit[110]) | (~bit[114]&bit[113]&~bit[112]&~bit[111]&~bit[110]) | (~bit[114]&bit[113]&~bit[112]&bit[111]&~bit[110]) | (~bit[114]&~bit[113]&~bit[111]&~bit[110]) | (bit[115]&~bit[113]&bit[112]&bit[111]&bit[110]) | (bit[114]&~bit[112]&~bit[110]) | (bit[112]&~bit[111]&~bit[110]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[115]&bit[114]&~bit[113]&bit[112]&bit[111]&bit[110]) | (bit[115]&~bit[114]&bit[112]&bit[111]&bit[110]) | (~bit[115]&bit[113]&bit[112]&bit[111]&bit[110]) | (~bit[114]&bit[113]&~bit[112]&~bit[111]&~bit[110]) | (~bit[113]&bit[112]&bit[111]&~bit[110]) | (bit[115]&~bit[113]&bit[112]&bit[111]&bit[110]) | (~bit[113]&~bit[112]&~bit[111]) | (bit[113]&bit[112]&bit[111]&~bit[110]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[115]&bit[114]&bit[113]&bit[112]&bit[111]&bit[110]) | (~bit[115]&bit[114]&~bit[113]&bit[112]&bit[111]&bit[110]) | (~bit[114]&bit[113]&~bit[112]&bit[111]&bit[110]) | (~bit[114]&~bit[113]&~bit[112]&bit[111]&~bit[110]) | (bit[114]&~bit[113]&bit[112]&~bit[111]&bit[110]) | (bit[114]&bit[113]&~bit[112]&~bit[111]) | (~bit[113]&bit[112]&bit[111]&~bit[110]) | (bit[115]&~bit[113]&bit[112]&bit[111]&bit[110]) | (~bit[114]&bit[112]&~bit[111]) | (bit[113]&~bit[112]&~bit[111]&bit[110]) | (bit[112]&~bit[111]&~bit[110]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[115]&bit[114]&bit[113]&bit[112]&bit[111]&bit[110]) | (~bit[115]&bit[114]&~bit[113]&bit[112]&bit[111]&bit[110]) | (bit[114]&~bit[113]&~bit[112]&bit[111]&~bit[110]) | (bit[114]&bit[113]&~bit[112]&bit[111]&bit[110]) | (~bit[115]&~bit[114]&bit[113]&bit[111]) | (bit[114]&bit[113]&~bit[111]&bit[110]) | (~bit[113]&bit[112]&~bit[111]&~bit[110]) | (~bit[114]&bit[113]&~bit[112]&bit[111]&~bit[110]) | (~bit[114]&~bit[113]&bit[112]&bit[110]) | (bit[114]&bit[113]&~bit[112]&~bit[111]) | (~bit[114]&~bit[113]&~bit[111]&~bit[110]) | (bit[113]&bit[112]&bit[111]&~bit[110]) | (bit[113]&~bit[112]&~bit[111]&bit[110]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[115]&bit[113]&bit[111]&bit[110]) | (bit[114]&bit[113]&~bit[112]&bit[111]&bit[110]) | (bit[114]&~bit[113]&bit[112]&~bit[111]&bit[110]) | (~bit[114]&~bit[112]&~bit[111]&bit[110]) | (~bit[114]&bit[113]&bit[112]&~bit[110]) | (~bit[114]&bit[112]&bit[111]&~bit[110]) | (bit[115]&~bit[113]&bit[112]&bit[111]&bit[110]) | (bit[114]&~bit[112]&~bit[110]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[114]&~bit[113]&~bit[112]&bit[111]&~bit[110]) | (~bit[115]&bit[113]&~bit[112]&bit[110]) | (bit[114]&~bit[113]&bit[112]&~bit[111]&bit[110]) | (~bit[114]&bit[113]&~bit[112]&~bit[111]&~bit[110]) | (~bit[114]&~bit[113]&bit[112]&~bit[110]) | (~bit[114]&~bit[113]&~bit[112]&bit[110]) | (~bit[114]&bit[113]&~bit[112]&bit[111]&~bit[110]) | (bit[115]&~bit[113]&bit[112]&bit[111]&bit[110]) | (bit[114]&~bit[112]&~bit[111]) | (bit[114]&bit[113]&bit[112]) | (bit[113]&~bit[112]&~bit[111]&bit[110]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[109]);
		control=control & bit[109];

		//-------------110--------------

		out[6] = (~disable_update & ((~bit[116]&~bit[115]&bit[113]&bit[112]&bit[111]) | (bit[115]&bit[114]&~bit[113]&bit[112]&bit[111]) | (bit[116]&~bit[114]&bit[113]&bit[112]&bit[111]) | (~bit[116]&~bit[114]&bit[113]&bit[112]&bit[111]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((bit[116]&~bit[115]&bit[114]&bit[113]&bit[112]&bit[111]) | (bit[115]&bit[114]&bit[113]&bit[112]&bit[111]) | (~bit[112]&~bit[111]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[116]&~bit[115]&bit[114]&bit[113]&bit[112]&bit[111]) | (bit[115]&bit[114]&bit[113]&bit[112]&bit[111]) | (bit[112]&~bit[111]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[116]&~bit[115]&bit[114]&bit[113]&bit[112]&bit[111]) | (bit[115]&bit[114]&~bit[113]&bit[112]&bit[111]) | (bit[115]&bit[114]&bit[113]&bit[112]&bit[111]) | (~bit[114]&~bit[113]&bit[112]&~bit[111]) | (~bit[114]&bit[113]&~bit[112]&bit[111]) | (~bit[115]&~bit[113]&~bit[111]) | (~bit[114]&~bit[113]&~bit[112]) | (~bit[113]&~bit[112]&bit[111]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[116]&~bit[115]&bit[114]&bit[113]&bit[112]&bit[111]) | (~bit[116]&~bit[114]&bit[113]&bit[112]&bit[111]) | (bit[115]&bit[114]&bit[113]&bit[112]&bit[111]) | (bit[115]&bit[114]&~bit[113]&~bit[111]) | (~bit[115]&~bit[114]&bit[113]&bit[112]) | (~bit[115]&bit[114]&~bit[112]&bit[111]) | (~bit[114]&bit[113]&~bit[111]) | (~bit[115]&~bit[114]&~bit[111]) | (~bit[114]&~bit[113]&~bit[112]) | (~bit[113]&~bit[112]&bit[111]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[116]&~bit[115]&bit[114]&bit[113]&bit[112]&bit[111]) | (~bit[115]&bit[114]&~bit[112]&~bit[111]) | (~bit[116]&~bit[115]&~bit[114]&bit[111]) | (bit[115]&bit[114]&~bit[113]&~bit[111]) | (~bit[114]&~bit[113]&~bit[112]&bit[111]) | (bit[114]&bit[113]&~bit[112]&bit[111]) | (~bit[115]&~bit[114]&~bit[112]&bit[111]) | (~bit[115]&~bit[114]&~bit[113]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[116]&bit[115]&bit[114]&bit[112]&bit[111]) | (bit[116]&~bit[114]&bit[113]&bit[112]&bit[111]) | (bit[115]&~bit[113]&~bit[112]&~bit[111]) | (bit[115]&~bit[114]&~bit[113]&bit[112]) | (bit[114]&bit[113]&~bit[112]&bit[111]) | (~bit[114]&~bit[113]&bit[112]&~bit[111]) | (~bit[115]&bit[114]&~bit[112]&bit[111]) | (~bit[114]&bit[113]&~bit[112]&bit[111]) | (~bit[115]&~bit[114]&~bit[112]&bit[111]) | (~bit[115]&bit[113]&~bit[111]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[110]);
		control=control & bit[110];

		//-------------111--------------

		out[6] = (~disable_update & ((~bit[117]&bit[116]&bit[115]&~bit[114]&bit[113]&bit[112]) | (~bit[117]&bit[116]&~bit[115]&bit[114]&bit[113]&bit[112]) | (~bit[117]&~bit[116]&bit[114]&bit[113]&bit[112]) | (bit[117]&bit[116]&bit[115]&~bit[114]&bit[113]&bit[112]) | (bit[117]&~bit[115]&bit[114]&bit[113]&bit[112]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((~bit[117]&bit[116]&bit[115]&bit[114]&bit[113]&bit[112]) | (bit[116]&bit[115]&~bit[114]&bit[113]&~bit[112]) | (bit[117]&bit[115]&bit[114]&bit[113]&bit[112]) | (~bit[116]&bit[115]&~bit[114]&~bit[112]) | (bit[116]&~bit[115]&~bit[114]&~bit[112]) | (~bit[116]&~bit[115]&bit[113]&~bit[112]) | (bit[115]&~bit[114]&~bit[113]&~bit[112]) | (bit[116]&~bit[115]&~bit[113]&~bit[112]) | (bit[116]&bit[115]&bit[114]&~bit[113]&~bit[112]) | (~bit[116]&~bit[113]&~bit[112]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[117]&bit[116]&bit[115]&bit[114]&bit[113]&bit[112]) | (~bit[116]&~bit[115]&~bit[113]) | (~bit[116]&bit[115]&bit[114]&bit[113]&~bit[112]) | (bit[117]&bit[115]&bit[114]&bit[113]&bit[112]) | (~bit[115]&~bit[114]&~bit[113]&bit[112]) | (bit[115]&~bit[114]&~bit[113]&bit[112]) | (bit[115]&~bit[114]&~bit[113]&~bit[112]) | (bit[116]&~bit[115]&~bit[113]&~bit[112]) | (bit[116]&bit[114]&bit[113]&~bit[112]) | (~bit[116]&~bit[113]&~bit[112]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[117]&bit[116]&bit[115]&~bit[114]&bit[113]&bit[112]) | (~bit[117]&~bit[116]&~bit[115]&bit[113]&bit[112]) | (bit[117]&bit[116]&bit[115]&~bit[114]&bit[113]&bit[112]) | (~bit[117]&bit[116]&bit[115]&bit[114]&bit[113]&bit[112]) | (bit[116]&~bit[115]&bit[114]&bit[113]&~bit[112]) | (~bit[116]&~bit[115]&~bit[114]&~bit[112]) | (~bit[116]&~bit[115]&~bit[114]&bit[113]) | (~bit[116]&bit[115]&bit[114]&bit[113]&~bit[112]) | (bit[117]&bit[115]&bit[114]&bit[113]&bit[112]) | (bit[115]&bit[114]&~bit[113]&bit[112]) | (bit[116]&~bit[115]&bit[114]&~bit[113]&bit[112]) | (bit[116]&bit[115]&bit[114]&~bit[113]&~bit[112]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[117]&bit[116]&bit[115]&~bit[114]&bit[113]&bit[112]) | (bit[117]&~bit[116]&bit[114]&bit[113]&bit[112]) | (~bit[117]&bit[116]&~bit[115]&bit[114]&bit[113]&bit[112]) | (~bit[116]&bit[115]&bit[114]&bit[113]&~bit[112]) | (~bit[116]&bit[115]&~bit[114]&~bit[112]) | (bit[116]&~bit[115]&~bit[114]&~bit[112]) | (~bit[115]&~bit[114]&~bit[113]&bit[112]) | (bit[115]&~bit[114]&~bit[113]&~bit[112]) | (bit[116]&~bit[115]&bit[114]&~bit[113]&bit[112]) | (bit[116]&bit[114]&bit[113]&~bit[112]) | (bit[116]&bit[115]&bit[114]&~bit[113]&~bit[112]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[117]&bit[116]&~bit[115]&bit[114]&bit[113]&bit[112]) | (bit[116]&~bit[115]&~bit[114]&~bit[113]&~bit[112]) | (~bit[117]&~bit[116]&bit[114]&bit[113]&bit[112]) | (~bit[116]&~bit[115]&bit[114]&~bit[113]&~bit[112]) | (bit[117]&bit[116]&bit[115]&~bit[114]&bit[113]&bit[112]) | (~bit[117]&bit[116]&bit[115]&bit[114]&bit[113]&bit[112]) | (bit[117]&~bit[115]&bit[114]&bit[113]&bit[112]) | (~bit[116]&bit[115]&~bit[114]&~bit[113]) | (bit[116]&bit[115]&~bit[114]&bit[113]&~bit[112]) | (bit[115]&~bit[114]&~bit[113]&bit[112]) | (bit[115]&bit[114]&~bit[113]&bit[112]) | (bit[116]&~bit[115]&bit[114]&~bit[113]&bit[112]) | (bit[116]&bit[114]&bit[113]&~bit[112]) | (bit[116]&bit[115]&bit[114]&~bit[113]&~bit[112]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[117]&bit[116]&bit[115]&~bit[114]&bit[113]&bit[112]) | (bit[116]&~bit[115]&bit[114]&bit[113]&~bit[112]) | (bit[117]&~bit[115]&bit[114]&bit[113]&bit[112]) | (bit[116]&bit[115]&~bit[114]&bit[113]&~bit[112]) | (bit[117]&bit[115]&bit[114]&bit[113]&bit[112]) | (~bit[116]&~bit[115]&bit[113]&~bit[112]) | (~bit[116]&~bit[113]&bit[112]) | (bit[116]&~bit[115]&bit[114]&~bit[113]&bit[112]) | (bit[116]&~bit[115]&~bit[113]&~bit[112]) | (~bit[115]&~bit[114]&bit[113]) | (bit[116]&bit[115]&bit[114]&~bit[113]&~bit[112]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[111]);
		control=control & bit[111];

		//-------------112--------------

		out[6] = (~disable_update & ((bit[118]&~bit[116]&~bit[115]&bit[114]&bit[113]) | (~bit[118]&~bit[115]&bit[114]&bit[113]) | (~bit[117]&~bit[115]&bit[114]&bit[113]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((bit[118]&~bit[117]&bit[116]&bit[115]&bit[114]&bit[113]) | (~bit[118]&~bit[117]&bit[115]&bit[114]&bit[113]) | (bit[118]&bit[117]&bit[116]&~bit[115]&bit[114]&bit[113]) | (~bit[117]&~bit[116]&bit[115]&bit[114]&~bit[113]) | (~bit[117]&~bit[116]&bit[115]&~bit[114]&~bit[113]) | (bit[117]&bit[116]&bit[115]&bit[114]&bit[113]) | (~bit[116]&bit[115]&bit[114]&bit[113]) | (bit[116]&bit[115]&~bit[114]&~bit[113]) | (bit[117]&~bit[114]&~bit[113]) | (~bit[115]&~bit[113]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[118]&~bit[117]&bit[115]&bit[114]&bit[113]) | (bit[118]&bit[117]&bit[116]&~bit[115]&bit[114]&bit[113]) | (~bit[117]&~bit[116]&bit[115]&~bit[114]&~bit[113]) | (~bit[117]&bit[116]&bit[115]&bit[114]&~bit[113]) | (~bit[116]&~bit[115]&~bit[114]&bit[113]) | (~bit[116]&bit[115]&bit[114]&bit[113]) | (~bit[115]&~bit[114]&~bit[113]) | (bit[117]&bit[115]&bit[114]&~bit[113]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[118]&~bit[117]&bit[116]&bit[115]&bit[114]&bit[113]) | (~bit[118]&bit[117]&bit[115]&bit[114]&bit[113]) | (bit[118]&bit[117]&bit[116]&~bit[115]&bit[114]&bit[113]) | (~bit[117]&bit[116]&bit[115]&bit[114]&~bit[113]) | (~bit[117]&~bit[116]&~bit[115]&~bit[113]) | (bit[117]&bit[116]&bit[115]&bit[114]&bit[113]) | (bit[117]&~bit[116]&bit[115]&~bit[113]) | (~bit[117]&~bit[116]&bit[114]&bit[113]) | (bit[116]&~bit[115]&~bit[114]&bit[113]) | (bit[116]&bit[115]&~bit[114]&~bit[113]) | (~bit[116]&bit[115]&~bit[114]&bit[113]) | (~bit[116]&~bit[115]&bit[114]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[118]&~bit[117]&~bit[116]&bit[115]&bit[114]&bit[113]) | (bit[118]&~bit[117]&bit[116]&bit[115]&bit[114]&bit[113]) | (bit[118]&bit[117]&bit[116]&~bit[115]&bit[114]&bit[113]) | (~bit[118]&~bit[115]&bit[114]&bit[113]) | (bit[117]&~bit[116]&~bit[114]&~bit[113]) | (~bit[117]&bit[116]&bit[115]&~bit[114]) | (~bit[117]&~bit[115]&bit[114]&bit[113]) | (bit[116]&~bit[115]&~bit[114]&bit[113]) | (bit[116]&bit[115]&~bit[114]&~bit[113]) | (bit[117]&bit[115]&bit[114]&~bit[113]) | (~bit[117]&bit[116]&~bit[113]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[117]&~bit[116]&~bit[115]&bit[114]&~bit[113]) | (bit[117]&bit[116]&bit[115]&~bit[114]&bit[113]) | (bit[118]&bit[117]&bit[116]&~bit[115]&bit[114]&bit[113]) | (~bit[117]&~bit[116]&bit[115]&~bit[114]&~bit[113]) | (~bit[118]&~bit[117]&~bit[115]&bit[113]) | (bit[117]&bit[116]&bit[115]&bit[114]&bit[113]) | (bit[117]&~bit[116]&~bit[114]&~bit[113]) | (bit[117]&bit[116]&~bit[115]&~bit[113]) | (~bit[117]&bit[116]&~bit[115]&bit[113]) | (~bit[117]&bit[116]&bit[115]&~bit[114]) | (~bit[116]&~bit[115]&~bit[114]&bit[113]) | (~bit[116]&bit[115]&~bit[114]&bit[113]) | (bit[117]&bit[115]&bit[114]&~bit[113]) | (bit[118]&bit[115]&bit[113]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[118]&~bit[117]&~bit[116]&bit[115]&bit[114]&bit[113]) | (~bit[118]&bit[116]&~bit[115]&bit[114]&bit[113]) | (bit[118]&~bit[116]&~bit[115]&bit[114]&bit[113]) | (~bit[118]&bit[117]&bit[115]&bit[114]&bit[113]) | (bit[117]&bit[116]&bit[115]&~bit[114]&bit[113]) | (~bit[117]&~bit[116]&bit[115]&bit[114]&~bit[113]) | (~bit[117]&bit[116]&~bit[115]&~bit[114]) | (~bit[117]&bit[116]&bit[115]&bit[114]&~bit[113]) | (~bit[117]&~bit[116]&~bit[114]&bit[113]) | (bit[117]&~bit[116]&bit[115]&~bit[113]) | (bit[117]&bit[116]&~bit[115]&~bit[113]) | (bit[117]&~bit[114]&~bit[113]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[112]);
		control=control & bit[112];

		//-------------113--------------

		out[6] = (~disable_update & ((~bit[119]&bit[118]&~bit[117]&~bit[116]&bit[115]&bit[114]) | (bit[119]&~bit[117]&~bit[116]&bit[115]&bit[114]) | (~bit[118]&~bit[116]&bit[115]&bit[114]) | (bit[117]&bit[116]&~bit[115]&bit[114]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((bit[119]&bit[118]&~bit[117]&bit[116]&bit[115]&bit[114]) | (~bit[119]&~bit[117]&bit[116]&bit[115]&bit[114]) | (~bit[118]&bit[117]&~bit[116]&~bit[115]&~bit[114]) | (bit[118]&~bit[116]&~bit[115]&~bit[114]) | (~bit[118]&~bit[117]&~bit[115]&~bit[114]) | (bit[118]&bit[117]&bit[115]&bit[114]) | (~bit[118]&bit[116]&bit[115]&bit[114]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[119]&bit[118]&~bit[117]&bit[116]&bit[115]&bit[114]) | (bit[118]&bit[117]&~bit[116]&bit[115]&bit[114]) | (~bit[119]&~bit[117]&bit[116]&bit[115]&bit[114]) | (~bit[118]&~bit[117]&bit[115]&~bit[114]) | (~bit[118]&bit[117]&bit[116]&~bit[115]&~bit[114]) | (bit[118]&bit[116]&~bit[115]&~bit[114]) | (~bit[116]&bit[115]&~bit[114]) | (~bit[118]&bit[116]&bit[115]&bit[114]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[119]&~bit[118]&~bit[117]&bit[115]&bit[114]) | (~bit[118]&~bit[117]&~bit[116]&~bit[115]) | (~bit[118]&~bit[117]&bit[116]&bit[115]&bit[114]) | (~bit[119]&~bit[117]&bit[116]&bit[115]&bit[114]) | (bit[118]&~bit[117]&~bit[116]&~bit[115]&bit[114]) | (~bit[118]&bit[117]&bit[116]&bit[115]&~bit[114]) | (bit[117]&bit[116]&~bit[115]&bit[114]) | (~bit[118]&~bit[116]&~bit[115]&bit[114]) | (~bit[118]&bit[117]&bit[116]&~bit[115]&~bit[114]) | (bit[118]&bit[116]&~bit[115]&~bit[114]) | (bit[118]&bit[117]&bit[115]&bit[114]) | (bit[118]&bit[116]&bit[115]&~bit[114]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[119]&bit[118]&~bit[117]&~bit[116]&bit[115]&bit[114]) | (bit[119]&~bit[118]&~bit[117]&~bit[116]&bit[115]) | (bit[119]&bit[118]&~bit[117]&bit[116]&bit[115]&bit[114]) | (~bit[118]&~bit[117]&bit[116]&~bit[115]&bit[114]) | (bit[118]&bit[117]&~bit[116]&~bit[115]&bit[114]) | (~bit[118]&bit[117]&~bit[116]&~bit[115]&~bit[114]) | (~bit[119]&~bit[118]&bit[116]&bit[114]) | (bit[118]&~bit[117]&~bit[114]) | (~bit[118]&bit[117]&bit[116]&bit[115]&~bit[114]) | (~bit[117]&~bit[116]&bit[115]&~bit[114]) | (bit[118]&bit[117]&bit[115]&bit[114]) | (bit[118]&bit[116]&bit[115]&~bit[114]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[119]&~bit[118]&bit[117]&bit[116]&~bit[115]) | (~bit[119]&bit[118]&bit[117]&bit[116]&bit[114]) | (bit[119]&bit[118]&~bit[117]&bit[116]&bit[115]&bit[114]) | (bit[119]&~bit[117]&~bit[116]&bit[115]&bit[114]) | (~bit[118]&~bit[117]&~bit[116]&~bit[115]&bit[114]) | (bit[118]&bit[117]&~bit[116]&bit[115]&bit[114]) | (bit[118]&~bit[117]&bit[116]&~bit[115]) | (~bit[118]&bit[117]&bit[116]&bit[115]&~bit[114]) | (~bit[118]&~bit[116]&bit[115]&~bit[114]) | (bit[118]&~bit[116]&~bit[115]&~bit[114]) | (~bit[118]&bit[117]&bit[116]&~bit[115]&~bit[114]) | (bit[118]&~bit[117]&bit[116]&~bit[114]) | (bit[118]&bit[116]&~bit[115]&~bit[114]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[119]&~bit[118]&~bit[117]&~bit[116]&bit[115]) | (bit[119]&bit[117]&bit[116]&~bit[115]&bit[114]) | (bit[118]&bit[117]&~bit[116]&~bit[115]&bit[114]) | (bit[118]&bit[117]&~bit[116]&~bit[114]) | (~bit[119]&bit[117]&bit[115]&bit[114]) | (~bit[118]&bit[117]&~bit[116]&~bit[115]&~bit[114]) | (bit[118]&~bit[117]&~bit[116]&~bit[115]&bit[114]) | (~bit[118]&bit[117]&bit[116]&~bit[115]&~bit[114]) | (~bit[118]&~bit[117]&~bit[115]&~bit[114]) | (bit[118]&~bit[117]&bit[116]&~bit[114]) | (~bit[117]&~bit[116]&bit[115]&~bit[114]) | (~bit[118]&bit[116]&bit[115]&bit[114]) | (bit[118]&bit[116]&bit[115]&~bit[114]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[113]);
		control=control & bit[113];

		//-------------114--------------

		out[6] = (~disable_update & ((~bit[120]&bit[119]&bit[118]&bit[117]&~bit[116]&bit[115]) | (bit[120]&~bit[119]&bit[118]&~bit[117]&bit[116]&bit[115]) | (bit[120]&bit[119]&bit[118]&bit[117]&~bit[116]&bit[115]) | (~bit[120]&~bit[117]&bit[116]&bit[115]) | (~bit[118]&~bit[117]&bit[116]&bit[115]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((~bit[120]&bit[119]&bit[118]&bit[117]&bit[116]&bit[115]) | (~bit[119]&bit[118]&~bit[117]&~bit[116]&~bit[115]) | (bit[119]&bit[118]&~bit[117]&~bit[116]&~bit[115]) | (bit[120]&~bit[119]&bit[118]&bit[117]&bit[116]&bit[115]) | (~bit[120]&bit[119]&~bit[118]&bit[117]&bit[116]&bit[115]) | (~bit[120]&~bit[119]&bit[117]&bit[116]&bit[115]) | (bit[120]&~bit[118]&bit[117]&bit[116]&bit[115]) | (~bit[118]&~bit[116]&~bit[115]) | (bit[120]&bit[119]&bit[118]&bit[116]&bit[115]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[120]&bit[119]&bit[118]&bit[117]&bit[116]&bit[115]) | (bit[120]&bit[119]&bit[118]&~bit[117]&bit[116]) | (bit[120]&~bit[119]&bit[118]&bit[117]&bit[116]&bit[115]) | (~bit[120]&bit[119]&~bit[118]&bit[117]&bit[116]&bit[115]) | (~bit[120]&~bit[119]&bit[117]&bit[116]&bit[115]) | (bit[120]&~bit[118]&bit[117]&bit[116]&bit[115]) | (bit[118]&bit[117]&~bit[116]&~bit[115]) | (bit[116]&~bit[115]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[120]&bit[119]&bit[118]&bit[117]&~bit[116]&bit[115]) | (bit[120]&bit[119]&bit[118]&bit[117]&~bit[116]&bit[115]) | (~bit[120]&bit[119]&~bit[118]&bit[117]&bit[116]&bit[115]) | (bit[119]&~bit[118]&~bit[117]&bit[116]&~bit[115]) | (~bit[119]&~bit[117]&bit[116]&~bit[115]) | (~bit[119]&~bit[118]&bit[116]&bit[115]) | (~bit[119]&~bit[118]&~bit[117]&~bit[115]) | (bit[118]&bit[117]&~bit[116]&~bit[115]) | (bit[120]&bit[119]&bit[118]&bit[116]&bit[115]) | (~bit[117]&~bit[116]&bit[115]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[120]&bit[119]&bit[118]&bit[117]&~bit[116]&bit[115]) | (~bit[120]&~bit[119]&bit[118]&bit[116]&bit[115]) | (~bit[119]&bit[118]&~bit[117]&~bit[116]&~bit[115]) | (bit[120]&~bit[119]&bit[118]&bit[117]&bit[116]&bit[115]) | (bit[119]&~bit[118]&~bit[117]&~bit[116]&~bit[115]) | (~bit[120]&bit[119]&~bit[118]&~bit[117]&bit[115]) | (~bit[119]&~bit[118]&bit[117]&bit[116]&~bit[115]) | (bit[120]&bit[119]&~bit[118]&bit[116]&bit[115]) | (~bit[120]&~bit[119]&bit[117]&bit[116]&bit[115]) | (bit[119]&bit[118]&~bit[117]&bit[116]&~bit[115]) | (bit[118]&bit[117]&~bit[116]&~bit[115]) | (bit[120]&bit[119]&bit[118]&bit[116]&bit[115]) | (~bit[118]&~bit[116]&bit[115]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[120]&~bit[119]&bit[118]&~bit[117]&bit[116]&bit[115]) | (bit[120]&bit[119]&bit[118]&bit[117]&~bit[116]&bit[115]) | (~bit[120]&bit[118]&bit[117]&bit[116]&bit[115]) | (bit[119]&bit[118]&~bit[117]&~bit[116]&~bit[115]) | (~bit[120]&bit[119]&~bit[118]&bit[117]&bit[116]&bit[115]) | (~bit[120]&bit[119]&~bit[118]&~bit[117]&bit[115]) | (~bit[119]&~bit[118]&bit[117]&bit[116]&~bit[115]) | (bit[119]&~bit[118]&bit[117]&bit[116]&~bit[115]) | (~bit[119]&bit[118]&bit[117]&~bit[115]) | (~bit[119]&~bit[118]&~bit[117]&~bit[115]) | (bit[120]&~bit[118]&bit[117]&bit[116]&bit[115]) | (bit[119]&bit[118]&~bit[117]&bit[116]&~bit[115]) | (bit[120]&bit[119]&bit[118]&bit[116]&bit[115]) | (~bit[117]&~bit[116]&bit[115]) | (~bit[118]&~bit[116]&bit[115]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[120]&bit[119]&bit[118]&bit[117]&bit[116]&bit[115]) | (~bit[120]&~bit[119]&~bit[118]&~bit[117]&bit[115]) | (bit[120]&bit[119]&bit[118]&~bit[117]&bit[116]) | (bit[120]&~bit[119]&bit[118]&bit[117]&bit[116]&bit[115]) | (bit[119]&~bit[118]&~bit[117]&~bit[116]&~bit[115]) | (bit[119]&~bit[118]&~bit[117]&bit[116]&~bit[115]) | (bit[120]&bit[119]&~bit[118]&bit[116]&bit[115]) | (bit[119]&~bit[118]&bit[117]&bit[116]&~bit[115]) | (bit[119]&bit[118]&~bit[116]&~bit[115]) | (~bit[119]&~bit[117]&~bit[116]&bit[115]) | (bit[120]&~bit[118]&bit[117]&bit[116]&bit[115]) | (bit[119]&bit[118]&~bit[117]&bit[116]&~bit[115]) | (~bit[119]&~bit[118]&~bit[116]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[114]);
		control=control & bit[114];

		//-------------115--------------

		out[6] = (~disable_update & ((~bit[121]&~bit[120]&~bit[119]&bit[118]&bit[117]&bit[116]) | (bit[121]&~bit[120]&~bit[119]&bit[117]&bit[116]) | (bit[120]&bit[119]&bit[118]&~bit[117]&bit[116]) | (~bit[119]&~bit[118]&bit[117]&bit[116]) | (bit[119]&~bit[118]&bit[117]&bit[116]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((bit[121]&bit[120]&bit[119]&bit[118]&bit[117]&bit[116]) | (~bit[121]&~bit[120]&bit[119]&bit[118]&bit[117]&bit[116]) | (bit[121]&~bit[120]&bit[119]&bit[118]&bit[117]&bit[116]) | (~bit[120]&~bit[119]&~bit[117]&~bit[116]) | (~bit[119]&~bit[118]&~bit[117]&~bit[116]) | (~bit[121]&bit[120]&bit[118]&bit[117]&bit[116]) | (bit[120]&~bit[119]&bit[118]&bit[117]&bit[116]) | (bit[119]&~bit[118]&~bit[117]&~bit[116]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[121]&bit[120]&bit[119]&bit[118]&bit[117]&bit[116]) | (~bit[121]&~bit[120]&bit[119]&bit[118]&bit[117]&bit[116]) | (bit[121]&~bit[120]&bit[119]&bit[118]&bit[117]&bit[116]) | (~bit[121]&bit[120]&bit[118]&bit[117]&bit[116]) | (bit[120]&bit[119]&~bit[118]&bit[117]&~bit[116]) | (bit[120]&~bit[119]&bit[118]&bit[117]&bit[116]) | (bit[119]&bit[118]&~bit[117]&~bit[116]) | (~bit[120]&~bit[118]&bit[117]&~bit[116]) | (~bit[119]&bit[117]&~bit[116]) | (bit[120]&~bit[119]&bit[118]&~bit[117]&~bit[116]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[121]&~bit[120]&bit[119]&bit[118]&bit[117]&bit[116]) | (~bit[120]&~bit[119]&~bit[118]&~bit[117]) | (bit[120]&bit[119]&bit[118]&bit[117]&~bit[116]) | (~bit[119]&~bit[118]&~bit[117]&~bit[116]) | (bit[120]&bit[119]&bit[118]&~bit[117]&bit[116]) | (~bit[119]&~bit[118]&bit[117]&bit[116]) | (bit[120]&~bit[119]&bit[118]&bit[117]&bit[116]) | (bit[119]&bit[118]&~bit[117]&~bit[116]) | (bit[120]&~bit[119]&bit[118]&~bit[117]&~bit[116]) | (~bit[120]&bit[119]&bit[118]&~bit[116]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[120]&~bit[119]&~bit[118]&~bit[117]&bit[116]) | (~bit[121]&~bit[120]&~bit[118]&bit[117]) | (bit[121]&~bit[120]&bit[119]&bit[118]&bit[117]&bit[116]) | (bit[120]&bit[119]&bit[118]&bit[117]&~bit[116]) | (bit[120]&bit[119]&bit[118]&~bit[117]&bit[116]) | (~bit[121]&bit[120]&bit[118]&bit[117]&bit[116]) | (bit[120]&~bit[119]&bit[118]&bit[117]&bit[116]) | (bit[119]&bit[118]&~bit[117]&~bit[116]) | (bit[119]&~bit[118]&bit[117]&bit[116]) | (bit[119]&~bit[118]&~bit[117]&~bit[116]) | (bit[120]&~bit[119]&bit[118]&~bit[117]&~bit[116]) | (~bit[120]&bit[119]&~bit[118]) | (~bit[119]&~bit[118]&bit[117]&~bit[116]) | (~bit[120]&bit[119]&bit[118]&~bit[116]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[121]&bit[120]&~bit[119]&~bit[118]&bit[117]) | (~bit[121]&~bit[120]&~bit[119]&bit[118]&bit[117]&bit[116]) | (~bit[121]&~bit[120]&bit[119]&~bit[118]&bit[117]&bit[116]) | (bit[121]&~bit[120]&~bit[118]&bit[117]&bit[116]) | (bit[121]&bit[120]&bit[119]&bit[118]&bit[117]&bit[116]) | (~bit[121]&~bit[120]&bit[119]&bit[118]&bit[117]&bit[116]) | (bit[121]&~bit[120]&bit[119]&bit[118]&bit[117]&bit[116]) | (bit[120]&bit[119]&~bit[118]&~bit[117]&bit[116]) | (~bit[120]&~bit[119]&~bit[116]) | (bit[120]&bit[119]&bit[118]&~bit[117]&bit[116]) | (bit[120]&bit[119]&~bit[118]&bit[117]&~bit[116]) | (~bit[120]&~bit[118]&~bit[117]&~bit[116]) | (~bit[120]&~bit[119]&~bit[117]) | (bit[120]&~bit[119]&bit[118]&~bit[117]&~bit[116]) | (~bit[119]&~bit[118]&bit[117]&~bit[116]) | (~bit[120]&bit[119]&bit[118]&~bit[116]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[121]&~bit[120]&bit[119]&~bit[118]&bit[117]&bit[116]) | (bit[121]&~bit[120]&~bit[119]&bit[117]&bit[116]) | (bit[121]&bit[120]&bit[119]&bit[118]&bit[117]&bit[116]) | (~bit[121]&bit[120]&bit[119]&bit[116]) | (bit[120]&bit[119]&~bit[118]&~bit[117]&bit[116]) | (bit[120]&~bit[119]&~bit[117]&bit[116]) | (bit[120]&bit[119]&bit[118]&bit[117]&~bit[116]) | (~bit[121]&bit[120]&bit[118]&bit[117]&bit[116]) | (bit[120]&bit[119]&~bit[118]&bit[117]&~bit[116]) | (~bit[120]&~bit[118]&~bit[117]&~bit[116]) | (~bit[120]&~bit[118]&bit[117]&~bit[116]) | (bit[119]&~bit[118]&~bit[117]&~bit[116]) | (bit[120]&~bit[119]&bit[118]&~bit[117]&~bit[116]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[115]);
		control=control & bit[115];

		//-------------116--------------

		out[6] = (~disable_update & ((bit[122]&bit[121]&~bit[120]&~bit[119]&bit[118]&bit[117]) | (~bit[122]&~bit[121]&~bit[119]&bit[118]&bit[117]) | (bit[122]&~bit[121]&~bit[120]&~bit[119]&bit[118]&bit[117]) | (bit[121]&bit[120]&bit[119]&~bit[118]&bit[117]) | (~bit[122]&~bit[120]&~bit[119]&bit[118]&bit[117]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((bit[122]&bit[121]&bit[119]&bit[118]&bit[117]) | (~bit[121]&bit[120]&bit[119]&~bit[118]&~bit[117]) | (~bit[122]&bit[121]&bit[119]&bit[118]&bit[117]) | (~bit[122]&bit[121]&bit[120]&~bit[119]&bit[118]&bit[117]) | (~bit[121]&~bit[120]&bit[119]&bit[118]&bit[117]) | (~bit[122]&~bit[121]&bit[119]&bit[118]&bit[117]) | (bit[122]&bit[120]&~bit[119]&bit[118]&bit[117]) | (bit[121]&bit[119]&~bit[118]&~bit[117]) | (bit[120]&~bit[119]&~bit[118]&~bit[117]) | (~bit[121]&~bit[120]&~bit[118]&~bit[117]) | (~bit[120]&~bit[119]&~bit[117]) | (bit[122]&~bit[121]&bit[120]&bit[118]&bit[117]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[122]&bit[121]&bit[120]&~bit[119]&bit[118]&bit[117]) | (~bit[122]&~bit[121]&bit[119]&bit[118]&bit[117]) | (bit[122]&bit[120]&~bit[119]&bit[118]&bit[117]) | (~bit[121]&bit[119]&bit[118]&~bit[117]) | (bit[120]&~bit[119]&bit[118]&~bit[117]) | (~bit[120]&bit[119]&bit[118]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[121]&bit[120]&bit[119]&~bit[118]&bit[117]) | (~bit[122]&bit[121]&bit[120]&bit[119]&bit[117]) | (~bit[122]&bit[121]&bit[120]&~bit[119]&bit[118]&bit[117]) | (~bit[121]&~bit[120]&bit[119]&bit[118]&bit[117]) | (bit[121]&bit[120]&bit[119]&bit[118]&~bit[117]) | (bit[122]&bit[120]&~bit[119]&bit[118]&bit[117]) | (~bit[121]&~bit[120]&~bit[118]&~bit[117]) | (bit[120]&~bit[119]&bit[118]&~bit[117]) | (bit[122]&~bit[121]&bit[120]&bit[118]&bit[117]) | (~bit[119]&~bit[118]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[122]&~bit[121]&~bit[120]&~bit[119]&bit[118]&bit[117]) | (~bit[122]&bit[121]&bit[120]&bit[119]&bit[117]) | (~bit[121]&~bit[120]&bit[119]&bit[118]&~bit[117]) | (~bit[122]&~bit[120]&~bit[119]&bit[118]&bit[117]) | (~bit[121]&bit[120]&bit[119]&~bit[118]&~bit[117]) | (~bit[122]&bit[121]&bit[119]&bit[118]&bit[117]) | (~bit[122]&bit[121]&bit[120]&~bit[119]&bit[118]&bit[117]) | (bit[121]&bit[120]&bit[119]&bit[118]&bit[117]) | (bit[121]&bit[120]&bit[119]&bit[118]&~bit[117]) | (~bit[120]&~bit[119]&~bit[118]&~bit[117]) | (bit[122]&bit[120]&~bit[119]&bit[118]&bit[117]) | (~bit[121]&~bit[120]&~bit[118]&bit[117]) | (bit[121]&~bit[120]&~bit[118]&~bit[117]) | (bit[120]&~bit[119]&bit[118]&~bit[117]) | (bit[122]&~bit[121]&bit[120]&bit[118]&bit[117]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[122]&bit[121]&~bit[120]&~bit[119]&bit[118]&bit[117]) | (~bit[122]&~bit[121]&~bit[120]&bit[118]&bit[117]) | (bit[122]&bit[121]&bit[119]&~bit[118]&bit[117]) | (~bit[122]&bit[121]&bit[120]&~bit[119]&bit[118]&bit[117]) | (~bit[121]&bit[120]&~bit[119]&~bit[117]) | (~bit[121]&~bit[119]&~bit[118]&~bit[117]) | (bit[121]&bit[120]&bit[119]&bit[118]&~bit[117]) | (bit[121]&~bit[120]&~bit[118]&bit[117]) | (bit[121]&bit[119]&~bit[118]&~bit[117]) | (bit[122]&~bit[121]&bit[120]&bit[118]&bit[117]) | (~bit[120]&bit[119]&bit[118]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[122]&~bit[121]&~bit[120]&~bit[119]&bit[118]&bit[117]) | (bit[122]&bit[121]&bit[119]&~bit[118]&bit[117]) | (bit[122]&bit[121]&bit[119]&bit[118]&bit[117]) | (bit[121]&bit[120]&bit[119]&bit[118]&bit[117]) | (~bit[122]&~bit[121]&bit[119]&bit[118]&bit[117]) | (bit[121]&~bit[120]&bit[119]&~bit[118]) | (~bit[121]&~bit[120]&bit[118]&~bit[117]) | (~bit[121]&~bit[119]&~bit[118]&bit[117]) | (bit[120]&~bit[119]&~bit[118]&~bit[117]) | (~bit[121]&bit[119]&bit[118]&~bit[117]) | (bit[121]&~bit[120]&~bit[118]&~bit[117]) | (~bit[121]&bit[120]&~bit[118]) | (bit[122]&~bit[121]&bit[120]&bit[118]&bit[117]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[116]);
		control=control & bit[116];

		//-------------117--------------

		out[6] = (~disable_update & ((bit[123]&bit[122]&~bit[121]&~bit[120]&bit[119]&bit[118]) | (~bit[123]&~bit[122]&bit[121]&~bit[120]&bit[119]&bit[118]) | (bit[123]&bit[121]&~bit[120]&bit[119]&bit[118]) | (~bit[123]&bit[122]&~bit[120]&bit[119]&bit[118]) | (~bit[122]&~bit[121]&~bit[120]&bit[119]&bit[118]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((~bit[123]&~bit[122]&bit[121]&bit[120]&bit[119]&bit[118]) | (bit[123]&~bit[122]&bit[120]&bit[119]&bit[118]) | (~bit[122]&~bit[121]&bit[120]&bit[119]&bit[118]) | (~bit[121]&~bit[120]&~bit[119]&~bit[118]) | (bit[122]&bit[120]&bit[119]&bit[118]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[123]&~bit[122]&bit[121]&bit[120]&bit[119]&bit[118]) | (bit[123]&~bit[122]&bit[120]&bit[119]&bit[118]) | (bit[122]&bit[121]&~bit[120]&~bit[118]) | (bit[122]&~bit[121]&~bit[120]&bit[119]&~bit[118]) | (~bit[122]&bit[120]&~bit[119]&~bit[118]) | (~bit[122]&~bit[120]&bit[119]&~bit[118]) | (bit[121]&~bit[120]&~bit[119]&~bit[118]) | (bit[122]&bit[120]&~bit[119]&~bit[118]) | (~bit[121]&bit[120]&bit[119]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[123]&~bit[121]&~bit[120]&bit[118]) | (~bit[122]&~bit[121]&bit[120]&bit[119]&bit[118]) | (bit[122]&~bit[121]&~bit[120]&~bit[119]&bit[118]) | (~bit[121]&bit[120]&~bit[119]&~bit[118]) | (~bit[122]&bit[121]&bit[120]&bit[119]&~bit[118]) | (~bit[122]&~bit[120]&~bit[119]&bit[118]) | (~bit[122]&~bit[121]&~bit[120]&bit[119]&bit[118]) | (~bit[122]&bit[120]&~bit[119]&~bit[118]) | (bit[122]&bit[121]&bit[120]&bit[119]) | (bit[121]&~bit[120]&~bit[119]&~bit[118]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[123]&~bit[122]&bit[121]&bit[120]&bit[119]&bit[118]) | (bit[123]&bit[122]&~bit[121]&~bit[120]&bit[119]&bit[118]) | (~bit[123]&~bit[122]&bit[121]&~bit[120]&bit[119]&bit[118]) | (bit[122]&bit[121]&~bit[119]&~bit[118]) | (~bit[122]&bit[121]&bit[120]&bit[119]&~bit[118]) | (bit[122]&~bit[121]&~bit[120]&bit[119]&~bit[118]) | (~bit[122]&~bit[121]&bit[120]&~bit[119]) | (~bit[122]&~bit[121]&~bit[120]&bit[119]&bit[118]) | (~bit[121]&bit[120]&~bit[119]&bit[118]) | (bit[122]&bit[121]&bit[120]&bit[119]) | (bit[122]&bit[121]&~bit[120]&~bit[119]) | (bit[122]&bit[120]&bit[119]&bit[118]) | (~bit[122]&~bit[120]&bit[119]&~bit[118]) | (bit[121]&~bit[120]&~bit[119]&~bit[118]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[123]&~bit[122]&bit[121]&bit[119]&bit[118]) | (bit[123]&bit[122]&~bit[121]&~bit[120]&bit[119]&bit[118]) | (~bit[123]&bit[122]&~bit[120]&bit[119]&bit[118]) | (~bit[123]&bit[122]&bit[120]&bit[119]&bit[118]) | (~bit[123]&~bit[121]&bit[119]&bit[118]) | (bit[121]&bit[120]&~bit[119]&bit[118]) | (~bit[122]&~bit[121]&~bit[120]&~bit[118]) | (bit[122]&bit[121]&~bit[120]&~bit[118]) | (~bit[122]&~bit[121]&~bit[120]&~bit[119]) | (bit[122]&bit[121]&~bit[120]&~bit[119]) | (~bit[122]&~bit[120]&~bit[119]&~bit[118]) | (bit[122]&bit[120]&~bit[119]&~bit[118]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[123]&~bit[122]&bit[121]&~bit[120]&bit[119]&bit[118]) | (bit[123]&bit[121]&~bit[120]&bit[119]&bit[118]) | (~bit[123]&bit[122]&bit[120]&bit[119]&bit[118]) | (bit[123]&~bit[122]&bit[120]&bit[119]&bit[118]) | (bit[122]&~bit[121]&~bit[120]&~bit[119]&bit[118]) | (~bit[122]&~bit[121]&bit[120]&~bit[118]) | (~bit[122]&bit[121]&bit[120]&bit[119]&~bit[118]) | (bit[122]&~bit[121]&~bit[120]&bit[119]&~bit[118]) | (~bit[122]&bit[120]&~bit[119]&bit[118]) | (bit[122]&~bit[121]&bit[119]&bit[118]) | (~bit[122]&~bit[120]&~bit[119]&~bit[118]) | (bit[122]&bit[120]&~bit[119]&~bit[118]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[117]);
		control=control & bit[117];

		//-------------118--------------

		out[6] = (~disable_update & ((bit[124]&~bit[123]&~bit[121]&bit[120]&bit[119]) | (~bit[124]&~bit[123]&~bit[121]&bit[120]&bit[119]) | (bit[122]&bit[121]&~bit[120]&bit[119]) | (~bit[122]&~bit[121]&bit[120]&bit[119]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((bit[124]&bit[123]&bit[122]&bit[121]&bit[120]&bit[119]) | (~bit[123]&~bit[122]&~bit[120]&~bit[119]) | (bit[123]&bit[122]&~bit[121]&bit[120]&bit[119]) | (~bit[123]&bit[122]&bit[121]&bit[120]&bit[119]) | (~bit[124]&bit[121]&bit[120]&bit[119]) | (~bit[122]&bit[121]&bit[120]&bit[119]) | (~bit[121]&~bit[120]&~bit[119]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[124]&~bit[123]&bit[122]&bit[121]&bit[119]) | (bit[123]&bit[122]&~bit[121]&bit[120]&bit[119]) | (~bit[123]&bit[122]&~bit[121]&bit[120]&~bit[119]) | (bit[122]&bit[121]&~bit[120]&~bit[119]) | (bit[123]&bit[122]&~bit[121]&bit[120]&~bit[119]) | (~bit[122]&bit[121]&bit[120]&~bit[119]) | (~bit[123]&bit[122]&bit[121]&bit[120]&bit[119]) | (~bit[124]&bit[121]&bit[120]&bit[119]) | (bit[123]&~bit[122]&bit[121]&~bit[119]) | (~bit[122]&~bit[121]&bit[120]&~bit[119]) | (~bit[122]&bit[121]&bit[120]&bit[119]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[124]&bit[123]&bit[122]&bit[121]&bit[120]&bit[119]) | (bit[124]&~bit[123]&bit[122]&bit[121]&~bit[120]&bit[119]) | (~bit[124]&~bit[122]&~bit[121]&bit[119]) | (bit[123]&bit[122]&~bit[121]&bit[120]&bit[119]) | (~bit[122]&~bit[121]&~bit[120]&bit[119]) | (~bit[123]&~bit[122]&bit[120]&bit[119]) | (bit[123]&bit[122]&bit[121]&~bit[120]) | (bit[123]&bit[121]&~bit[120]&~bit[119]) | (~bit[123]&~bit[122]&~bit[121]&~bit[119]) | (~bit[122]&~bit[121]&bit[120]&~bit[119]) | (bit[122]&bit[121]&~bit[119]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[124]&bit[123]&bit[122]&~bit[121]&bit[120]&bit[119]) | (~bit[124]&bit[123]&~bit[122]&bit[121]&bit[120]&bit[119]) | (bit[124]&~bit[123]&bit[122]&bit[121]&~bit[120]&bit[119]) | (bit[124]&bit[123]&bit[120]&bit[119]) | (bit[122]&~bit[120]&~bit[119]) | (~bit[123]&bit[122]&~bit[121]&bit[120]&~bit[119]) | (bit[123]&~bit[122]&~bit[120]&~bit[119]) | (bit[123]&bit[122]&bit[121]&~bit[120]) | (~bit[123]&bit[122]&~bit[121]&~bit[120]) | (bit[122]&bit[121]&~bit[119]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[124]&bit[123]&bit[122]&~bit[121]&bit[120]&bit[119]) | (~bit[124]&bit[123]&~bit[122]&bit[121]&bit[120]&bit[119]) | (bit[124]&bit[123]&bit[122]&bit[121]&bit[120]&bit[119]) | (~bit[124]&~bit[123]&~bit[121]&bit[120]&bit[119]) | (bit[124]&~bit[123]&bit[122]&bit[121]&~bit[120]&bit[119]) | (~bit[124]&~bit[123]&bit[122]&bit[120]) | (~bit[123]&bit[122]&~bit[121]&bit[120]&~bit[119]) | (bit[123]&bit[122]&~bit[121]&bit[120]&~bit[119]) | (bit[123]&~bit[122]&~bit[120]&~bit[119]) | (bit[122]&~bit[121]&~bit[120]&bit[119]) | (~bit[123]&~bit[122]&~bit[120]) | (~bit[123]&bit[121]&~bit[119]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[124]&~bit[123]&~bit[121]&bit[120]&bit[119]) | (bit[124]&bit[123]&bit[122]&bit[121]&bit[120]&bit[119]) | (~bit[124]&~bit[123]&bit[121]&bit[120]&bit[119]) | (bit[124]&~bit[123]&bit[122]&bit[121]&~bit[120]&bit[119]) | (~bit[124]&bit[123]&bit[121]&~bit[120]) | (bit[123]&~bit[122]&bit[121]&~bit[120]) | (bit[123]&bit[122]&~bit[121]&bit[120]&~bit[119]) | (~bit[123]&~bit[122]&~bit[121]&bit[120]) | (~bit[123]&bit[122]&bit[121]&bit[120]&bit[119]) | (bit[123]&~bit[122]&bit[121]&~bit[119]) | (bit[123]&bit[121]&~bit[120]&~bit[119]) | (~bit[123]&~bit[122]&~bit[121]&~bit[119]) | (~bit[123]&bit[122]&~bit[121]&~bit[120]) | (bit[122]&~bit[121]&~bit[120]&bit[119]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[118]);
		control=control & bit[118];

		//-------------119--------------

		out[6] = (~disable_update & ((~bit[125]&bit[124]&bit[123]&~bit[122]&bit[121]&bit[120]) | (~bit[124]&bit[123]&~bit[122]&bit[121]&bit[120]) | (bit[124]&bit[123]&bit[122]&~bit[121]&bit[120]) | (~bit[123]&~bit[122]&bit[121]&bit[120]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((bit[125]&bit[124]&bit[123]&~bit[122]&bit[121]&bit[120]) | (bit[124]&~bit[123]&bit[122]&~bit[121]&~bit[120]) | (bit[123]&bit[122]&~bit[121]&~bit[120]) | (~bit[124]&~bit[123]&~bit[121]&~bit[120]) | (~bit[123]&~bit[122]&~bit[120]) | (bit[122]&bit[121]&bit[120]) | (~bit[122]&~bit[121]&~bit[120]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[125]&bit[124]&bit[123]&~bit[122]&bit[121]&bit[120]) | (~bit[123]&~bit[122]&~bit[121]&bit[120]) | (~bit[124]&~bit[122]&~bit[121]&bit[120]) | (~bit[123]&bit[122]&bit[121]&~bit[120]) | (bit[122]&bit[121]&bit[120]) | (bit[123]&bit[121]&~bit[120]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[125]&bit[124]&bit[123]&~bit[122]&bit[121]&bit[120]) | (bit[125]&~bit[123]&bit[122]&bit[121]&bit[120]) | (bit[125]&~bit[124]&~bit[123]&bit[121]&bit[120]) | (~bit[124]&~bit[123]&bit[122]&~bit[121]&bit[120]) | (bit[124]&bit[123]&bit[122]&~bit[121]&bit[120]) | (bit[124]&~bit[123]&bit[122]&~bit[121]&bit[120]) | (~bit[125]&~bit[123]&bit[121]&bit[120]) | (bit[123]&~bit[122]&~bit[120]) | (~bit[124]&~bit[123]&~bit[121]&~bit[120]) | (bit[124]&bit[123]&~bit[122]&~bit[121]&bit[120]) | (~bit[123]&bit[122]&bit[121]&~bit[120]) | (~bit[122]&~bit[121]&~bit[120]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[124]&~bit[123]&bit[122]&~bit[121]&~bit[120]) | (~bit[124]&~bit[123]&bit[122]&~bit[121]&bit[120]) | (~bit[125]&~bit[124]&bit[121]&bit[120]) | (bit[125]&bit[124]&~bit[122]&bit[121]&bit[120]) | (bit[124]&bit[123]&bit[122]&~bit[121]&bit[120]) | (~bit[124]&~bit[123]&~bit[122]&~bit[121]) | (bit[124]&bit[123]&~bit[122]&~bit[121]&bit[120]) | (~bit[124]&bit[123]&bit[122]) | (bit[123]&bit[121]&~bit[120]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[125]&bit[124]&bit[123]&~bit[121]&bit[120]) | (~bit[124]&bit[123]&bit[122]&~bit[121]&bit[120]) | (bit[125]&~bit[124]&~bit[123]&bit[121]&bit[120]) | (~bit[124]&bit[123]&~bit[122]&bit[121]&bit[120]) | (bit[125]&bit[124]&~bit[122]&bit[121]&bit[120]) | (bit[124]&~bit[123]&bit[122]&~bit[121]&bit[120]) | (~bit[124]&bit[123]&~bit[122]&~bit[120]) | (~bit[124]&bit[122]&bit[121]&~bit[120]) | (~bit[125]&bit[122]&bit[121]&bit[120]) | (bit[124]&bit[122]&~bit[121]&~bit[120]) | (~bit[123]&~bit[122]&~bit[121]&~bit[120]) | (bit[124]&bit[123]&~bit[122]&~bit[121]&bit[120]) | (~bit[123]&bit[122]&bit[121]&~bit[120]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[125]&bit[124]&bit[123]&~bit[122]&bit[121]&bit[120]) | (bit[125]&bit[124]&bit[123]&~bit[121]) | (bit[125]&~bit[123]&bit[122]&bit[121]&bit[120]) | (bit[125]&bit[124]&~bit[122]&bit[121]&bit[120]) | (bit[123]&bit[122]&~bit[121]&~bit[120]) | (~bit[124]&bit[121]&~bit[120]) | (bit[124]&~bit[122]&~bit[121]) | (bit[123]&bit[121]&~bit[120]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[119]);
		control=control & bit[119];

		//-------------120--------------

		out[6] = (~disable_update & ((~bit[126]&bit[125]&~bit[124]&bit[123]&~bit[122]&bit[121]) | (~bit[126]&~bit[125]&bit[123]&~bit[122]&bit[121]) | (bit[126]&~bit[124]&bit[123]&~bit[122]&bit[121]) | (bit[126]&bit[124]&~bit[123]&~bit[122]&bit[121]) | (~bit[126]&bit[124]&~bit[123]&~bit[122]&bit[121]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((~bit[126]&~bit[124]&~bit[123]&bit[122]&bit[121]) | (~bit[125]&bit[124]&~bit[123]&~bit[122]&~bit[121]) | (bit[125]&~bit[124]&bit[123]&bit[122]&bit[121]) | (bit[126]&bit[124]&bit[123]&~bit[122]&bit[121]) | (~bit[125]&~bit[124]&~bit[123]&bit[122]&bit[121]) | (~bit[126]&bit[125]&bit[124]&bit[123]&~bit[122]&bit[121]) | (~bit[125]&~bit[124]&bit[123]&bit[122]&bit[121]) | (bit[126]&bit[125]&~bit[124]&~bit[123]&bit[122]&bit[121]) | (~bit[126]&~bit[125]&bit[122]&bit[121]) | (bit[124]&~bit[123]&bit[122]&bit[121]) | (bit[125]&~bit[123]&~bit[122]&~bit[121]) | (~bit[124]&~bit[122]&~bit[121]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[126]&~bit[124]&~bit[123]&bit[122]&bit[121]) | (bit[126]&bit[124]&bit[123]&~bit[122]&bit[121]) | (~bit[125]&~bit[124]&~bit[123]&bit[122]&bit[121]) | (~bit[126]&bit[125]&bit[124]&bit[123]&~bit[122]&bit[121]) | (bit[126]&~bit[125]&bit[124]&bit[123]&bit[121]) | (bit[125]&bit[124]&bit[123]&bit[122]&bit[121]) | (bit[124]&bit[123]&~bit[122]&~bit[121]) | (~bit[123]&bit[122]&~bit[121]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[126]&bit[124]&bit[123]&~bit[122]&bit[121]) | (bit[126]&bit[124]&~bit[123]&~bit[122]&bit[121]) | (~bit[126]&bit[125]&bit[124]&bit[123]&~bit[122]&bit[121]) | (~bit[126]&bit[124]&~bit[123]&~bit[122]&bit[121]) | (bit[126]&~bit[125]&bit[124]&bit[123]&bit[121]) | (bit[125]&bit[124]&bit[123]&bit[122]&bit[121]) | (bit[126]&bit[125]&~bit[124]&~bit[123]&bit[122]&bit[121]) | (~bit[125]&~bit[124]&~bit[123]&~bit[121]) | (bit[124]&bit[123]&~bit[122]&~bit[121]) | (bit[124]&~bit[123]&bit[122]&bit[121]) | (~bit[124]&bit[122]&~bit[121]) | (~bit[125]&bit[122]&~bit[121]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[126]&~bit[125]&~bit[124]&bit[123]&~bit[122]) | (~bit[126]&~bit[125]&~bit[123]&bit[122]&bit[121]) | (~bit[125]&~bit[124]&bit[123]&bit[122]&bit[121]) | (bit[125]&bit[124]&bit[123]&bit[122]&bit[121]) | (~bit[125]&bit[124]&~bit[123]&~bit[122]&bit[121]) | (bit[126]&bit[125]&~bit[124]&~bit[123]&bit[122]&bit[121]) | (bit[125]&bit[124]&bit[122]&~bit[121]) | (~bit[124]&bit[123]&bit[122]&~bit[121]) | (bit[126]&~bit[125]&bit[124]&bit[121]) | (bit[124]&bit[123]&~bit[122]&~bit[121]) | (bit[125]&~bit[123]&~bit[122]&~bit[121]) | (~bit[125]&~bit[122]&~bit[121]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[126]&bit[125]&~bit[124]&bit[123]&~bit[122]&bit[121]) | (bit[126]&~bit[125]&~bit[124]&~bit[122]&bit[121]) | (~bit[125]&bit[124]&~bit[123]&~bit[122]&~bit[121]) | (~bit[125]&bit[124]&bit[123]&~bit[122]&~bit[121]) | (bit[125]&~bit[124]&bit[123]&bit[122]&bit[121]) | (~bit[126]&~bit[125]&~bit[123]&bit[122]&bit[121]) | (bit[125]&~bit[124]&~bit[123]&~bit[121]) | (~bit[126]&bit[125]&bit[124]&bit[123]&~bit[122]&bit[121]) | (~bit[126]&bit[124]&~bit[123]&~bit[122]&bit[121]) | (bit[126]&~bit[125]&bit[124]&bit[123]&bit[121]) | (bit[126]&bit[125]&~bit[124]&~bit[123]&bit[122]&bit[121]) | (bit[125]&bit[124]&bit[122]&~bit[121]) | (~bit[125]&~bit[124]&bit[122]&~bit[121]) | (~bit[125]&~bit[124]&~bit[123]&bit[121]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[126]&bit[125]&bit[124]&bit[122]&bit[121]) | (bit[126]&~bit[124]&bit[123]&~bit[122]&bit[121]) | (~bit[125]&bit[124]&bit[123]&~bit[122]&~bit[121]) | (bit[126]&bit[124]&~bit[123]&~bit[122]&bit[121]) | (bit[125]&bit[124]&~bit[123]&~bit[121]) | (~bit[126]&bit[125]&bit[124]&bit[123]&~bit[122]&bit[121]) | (~bit[125]&bit[124]&~bit[123]&~bit[122]&bit[121]) | (bit[126]&bit[125]&~bit[124]&~bit[123]&bit[122]&bit[121]) | (~bit[126]&bit[123]&bit[122]&bit[121]) | (~bit[124]&bit[123]&bit[122]&~bit[121]) | (bit[126]&~bit[125]&bit[124]&bit[121]) | (~bit[125]&~bit[124]&bit[122]&~bit[121]) | (bit[125]&~bit[123]&~bit[122]&~bit[121]) | (~bit[125]&~bit[124]&~bit[123]&bit[121]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[120]);
		control=control & bit[120];

		//-------------121--------------

		out[6] = (~disable_update & ((~bit[127]&bit[126]&bit[125]&bit[124]&~bit[123]&bit[122]) | (~bit[126]&~bit[125]&~bit[124]&bit[123]&bit[122]) | (bit[126]&bit[125]&~bit[124]&~bit[123]&bit[122]) | (bit[127]&~bit[125]&bit[124]&~bit[123]&bit[122]) | (bit[127]&bit[125]&bit[124]&~bit[123]&bit[122]) | (~bit[127]&~bit[126]&bit[124]&~bit[123]&bit[122]) | (~bit[127]&~bit[125]&bit[124]&~bit[123]&bit[122]))) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((~bit[126]&bit[125]&~bit[124]&bit[123]&bit[122]) | (~bit[126]&~bit[125]&bit[124]&bit[123]&bit[122]) | (bit[126]&bit[125]&~bit[124]&bit[123]&bit[122]) | (bit[126]&~bit[125]&bit[123]&bit[122]) | (bit[125]&bit[124]&bit[123]&bit[122]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((bit[126]&bit[125]&~bit[124]&~bit[123]&bit[122]) | (~bit[126]&bit[125]&~bit[124]&bit[123]&bit[122]) | (bit[126]&bit[125]&bit[124]&~bit[123]&~bit[122]) | (~bit[126]&~bit[125]&bit[124]&bit[123]&bit[122]) | (~bit[126]&bit[124]&~bit[123]&~bit[122]) | (~bit[126]&~bit[125]&~bit[124]&~bit[122]) | (bit[126]&bit[125]&~bit[124]&bit[123]&bit[122]) | (bit[126]&~bit[125]&bit[123]&bit[122]) | (~bit[125]&bit[124]&~bit[123]&~bit[122]) | (~bit[124]&~bit[123]&~bit[122]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((bit[126]&~bit[125]&bit[124]&bit[123]&~bit[122]) | (~bit[126]&~bit[125]&bit[124]&bit[123]&~bit[122]) | (bit[127]&~bit[125]&bit[124]&~bit[123]&bit[122]) | (~bit[127]&~bit[126]&bit[124]&~bit[123]&bit[122]) | (~bit[127]&~bit[125]&bit[124]&~bit[123]&bit[122]) | (~bit[126]&bit[125]&~bit[124]&bit[123]&bit[122]) | (bit[126]&~bit[125]&~bit[124]&bit[123]) | (bit[126]&bit[125]&~bit[124]&bit[123]&~bit[122]) | (~bit[126]&bit[125]&~bit[124]&bit[123]&~bit[122]) | (bit[125]&bit[124]&bit[123]&bit[122]) | (~bit[124]&~bit[123]&~bit[122]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((~bit[127]&bit[126]&bit[125]&bit[124]&~bit[123]&bit[122]) | (~bit[127]&bit[126]&~bit[124]&bit[123]&bit[122]) | (bit[126]&~bit[125]&~bit[124]&bit[123]&~bit[122]) | (bit[126]&bit[125]&bit[124]&bit[123]&~bit[122]) | (~bit[127]&~bit[125]&bit[124]&~bit[123]&bit[122]) | (bit[127]&~bit[126]&bit[124]&bit[122]) | (~bit[126]&bit[125]&~bit[124]&bit[123]&~bit[122]) | (~bit[126]&~bit[125]&bit[124]&bit[123]&bit[122]) | (~bit[126]&~bit[125]&~bit[123]&bit[122]) | (~bit[126]&bit[124]&~bit[123]&~bit[122]) | (bit[126]&bit[125]&~bit[124]&bit[123]&bit[122]) | (~bit[126]&bit[125]&bit[124]&bit[123]) | (bit[125]&bit[124]&bit[123]&bit[122]) | (~bit[125]&bit[124]&~bit[123]&~bit[122]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((~bit[127]&bit[126]&bit[125]&bit[124]&~bit[123]&bit[122]) | (~bit[126]&~bit[125]&bit[124]&bit[123]&~bit[122]) | (bit[127]&~bit[125]&bit[124]&~bit[123]&bit[122]) | (bit[127]&bit[125]&bit[124]&~bit[123]&bit[122]) | (bit[126]&bit[125]&bit[124]&~bit[123]&~bit[122]) | (~bit[127]&~bit[124]&bit[123]&bit[122]) | (bit[126]&bit[125]&~bit[124]&bit[123]&~bit[122]) | (~bit[126]&~bit[125]&bit[124]&~bit[123]) | (~bit[126]&~bit[125]&~bit[124]&~bit[122]) | (bit[126]&bit[125]&~bit[124]&bit[123]&bit[122]) | (~bit[126]&bit[125]&bit[124]&bit[123]) | (bit[126]&~bit[125]&bit[123]&bit[122]) | (bit[126]&~bit[125]&~bit[124]) | (~bit[125]&bit[124]&~bit[123]&~bit[122]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((bit[127]&~bit[126]&bit[125]&~bit[124]) | (bit[126]&~bit[125]&bit[124]&bit[123]&~bit[122]) | (bit[126]&bit[125]&bit[124]&bit[123]&~bit[122]) | (bit[127]&bit[125]&bit[124]&~bit[123]&bit[122]) | (~bit[127]&~bit[126]&bit[124]&~bit[123]&bit[122]) | (~bit[127]&bit[126]&~bit[124]&bit[122]) | (bit[126]&bit[125]&bit[124]&~bit[123]&~bit[122]) | (~bit[127]&bit[124]&bit[123]&bit[122]) | (bit[126]&bit[125]&~bit[124]&bit[123]&~bit[122]) | (~bit[126]&bit[125]&~bit[124]&bit[123]&~bit[122]) | (bit[126]&~bit[125]&~bit[124]&bit[122]) | (~bit[126]&~bit[123]&~bit[122]) | (~bit[126]&bit[125]&~bit[123]))) | (disable_update & out[0]);

		disable_update= disable_update | (control & ~bit[121]);
		control=control & bit[121];

		//-------------122--------------

		out[6] = (~disable_update & (bit[126]&~bit[124])) | (disable_update & out[6]);		
		out[5] = (~disable_update & ((~bit[124]&~bit[123]))) | (disable_update & out[5]);
		out[4] = (~disable_update & ((~bit[126]&~bit[125]&~bit[124]&bit[123]) | (bit[126]&bit[125]&bit[124]&~bit[123]) | (~bit[125]&bit[124]&~bit[123]) | (~bit[126]&bit[124]&~bit[123]))) | (disable_update & out[4]);
		out[3] = (~disable_update & ((~bit[127]&~bit[126]&bit[125]&~bit[124]&bit[123]) | (bit[127]&bit[126]&~bit[125]&~bit[124]&bit[123]) | (~bit[127]&bit[126]&~bit[125]&~bit[124]&bit[123]) | (~bit[126]&~bit[125]&~bit[124]&~bit[123]) | (~bit[125]&bit[124]&~bit[123]) | (~bit[127]&~bit[125]&~bit[123]))) | (disable_update & out[3]);
		out[2] = (~disable_update & ((bit[127]&~bit[126]&bit[125]&~bit[124]&bit[123]) | (bit[127]&~bit[126]&bit[125]&~bit[124]&~bit[123]) | (~bit[127]&bit[126]&~bit[125]&~bit[124]&bit[123]) | (bit[127]&bit[126]&~bit[125]&~bit[124]&~bit[123]) | (bit[126]&bit[125]&~bit[124]&bit[123]) | (~bit[127]&bit[125]&~bit[124]&~bit[123]) | (~bit[126]&bit[124]&~bit[123]))) | (disable_update & out[2]);
		out[1] = (~disable_update & ((bit[127]&bit[126]&~bit[125]&~bit[124]&bit[123]) | (bit[127]&~bit[126]&bit[125]&~bit[124]&bit[123]) | (bit[127]&bit[126]&~bit[125]&~bit[124]&~bit[123]) | (bit[126]&bit[125]&bit[124]&~bit[123]) | (~bit[127]&bit[124]&~bit[123]) | (~bit[126]&~bit[125]&~bit[124]&~bit[123]) | (~bit[127]&~bit[126]&~bit[123]))) | (disable_update & out[1]);
		out[0] = (~disable_update & ((~bit[127]&bit[126]&bit[125]&bit[124]&~bit[123]) | (~bit[127]&bit[126]&bit[125]&~bit[124]&bit[123]) | (bit[127]&~bit[126]&bit[125]&~bit[124]&~bit[123]) | (bit[127]&bit[126]&~bit[125]&~bit[123]) | (~bit[127]&~bit[126]&~bit[125]&bit[123]) | (~bit[127]&~bit[126]&~bit[125]&~bit[124]))) | (disable_update & out[0]);

		//disable_update= disable_update | (control & ~bit[122]);
		//control=control & bit[122];

		//-------------123--------------


		for(k=0;k<64;k++){
			sample[k]=0;
			sample[k]=(out[6])&1;
			sample[k]=(sample[k]<<1)|((out[5])&1);
			sample[k]=(sample[k]<<1)|((out[4])&1);
			sample[k]=(sample[k]<<1)|((out[3])&1);
			sample[k]=(sample[k]<<1)|((out[2])&1);
			sample[k]=(sample[k]<<1)|((out[1])&1);
			sample[k]=(sample[k]<<1)|((out[0])&1);

			out[6]=out[6]>>1;
			out[5]=out[5]>>1;			
			out[4]=out[4]>>1;			
			out[3]=out[3]>>1;
			out[2]=out[2]>>1;
			out[1]=out[1]>>1;
			out[0]=out[0]>>1;

		}
		clock2 = cpucycles();

		clock3=clock3+(clock2-clock1);
		clock4=clock4+(clock2-clock11);

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------

		for(k=0;k<64;k++){
			hist[sample[k]]++;
			printf("  %lld  ,",sample[k]);
		}

		printf("\n iteration : %ld\n",i);

		if(disable_update!=0xffffffffffffffff)//even if the sample is not found within depth 128
			break;
	}

	printf("Time to generate 64 samples with 128-bit precision without random number generation: %lld\n",clock3/repeat);
	printf("Time to generate 64 samples with 128-bit precision with random number generation: %lld\n",clock4/repeat);

//---------------------------print the histogram------------------------------
	printf("The Histogram is :\n");
	for(i=0;i<84;i++)
		printf(" %ld : %llu",i,hist[i]);
//--------------------------print the histogram ends--------------------------

	printf("\n");
	return 1;
}








