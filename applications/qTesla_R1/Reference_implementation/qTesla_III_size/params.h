/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: qTESLA parameters
**************************************************************************************/

#ifndef PARAMS_H
#define PARAMS_H

#define PARAM_N 1024
#define PARAM_N_LOG 10
#define PARAM_SIGMA 7.64
#define PARAM_Xi 9
#define PARAM_Q 4206593
#define PARAM_Q_LOG 23
#define PARAM_QINV 4148178943
#define PARAM_BARR_MULT 1021
#define PARAM_BARR_DIV 32
#define PARAM_B 1048575
#define PARAM_B_BITS 20
#define PARAM_S_BITS 8
#define PARAM_K 1
#define PARAM_SIGMA_E PARAM_SIGMA
#define PARAM_H 48
#define PARAM_D 21	
#define PARAM_GEN_A 38
#define PARAM_KEYGEN_BOUND_E 910 
#define PARAM_REJECTION PARAM_KEYGEN_BOUND_E
#define PARAM_KEYGEN_BOUND_S 910
#define PARAM_U PARAM_KEYGEN_BOUND_S
#define PARAM_R2_INVN 1217638
#define PARAM_R 35843
#define SHAKE shake256
#define cSHAKE cshake256_simple
#define SHAKE_RATE SHAKE256_RATE

#endif
