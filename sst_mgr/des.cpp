/******************************************************************
 * Implementation of DES cryptography algorithm by Roman
 * Ver. 0.5
 * Date 03.12.2008
 ******************************************************************/

#include <stdint.h>
#include <stdio.h>
#include "tables.h"

#define BIT(in, pos) ((in >> (pos)) & 0x1)

uint8_t p_table[32] = 
		{16,  7, 20, 21,
		 29, 12, 28, 17,
		 1,  15, 23, 26,
		 5,  18, 31, 10,
		 2,  8,  24, 14,
		 32, 27,  3,  9,
		 19, 13, 30,  6,
		 22, 11,  4, 25};

uint8_t sst_table[64] = {17, 0, 58, 25, 51, 24, 32, 33, 20, 42, 40, 1, 10, 36, 8, 38, 47, 60, 55, 11, 62, 46, 13, 37, 4, 39, 7, 29, 2, 23, 15, 45, 56, 31, 22, 28, 50, 61, 57, 34, 54, 49, 9, 5, 59, 48, 44, 16, 18, 14, 27, 19, 53, 35, 12, 3, 26, 30, 6, 41, 21, 43, 63, 52};

void DES_keySchedule(uint64_t key, uint64_t key_schedule[]) {
  /*This function computes the sub key (48 bits) from primary key and round number*/
  int i, j;         /*counter*/

  /*KeysTab - table 16x16x16*/
  for(j = 0; j < 16; j++) key_schedule[j] = 0x0ULL;
  for(i = 0; i < 16; i++) {
    for(j = 0; j < 16; j++)
      key_schedule[j] |= KeysTab[j][15 - i][(unsigned int)(key & 0xfULL)];
    key >>= 4;
  }
}

void DES_IP(uint64_t plaintext, uint32_t *L, uint32_t *R) {
	/*This function computes initial permutation for plaintext,
	 * returns full permuted text in variable mes
	 * and splits it in right R and left L parts*/
  int i;
  uint64_t mes = 0x0ULL;

  for(i = 7; i >= 0; i--) {
    mes |= IPTab[i][(int)(plaintext & 0xffULL)];
    plaintext >>= 8;
  }
  *L = (uint32_t)(mes >> 32);
  *R = (uint32_t)(mes & 0xffffffffULL);
}

uint64_t DES_E(const uint32_t R) {
	/*This function computes expanded R and returns 48-bits array*/
  uint64_t  mes = Expan[3][(int)(R & 0xffULL)] | Expan[2][(int)(R>>8 & 0xffULL)] | Expan[1][(int)(R>>16 & 0xffULL)] | Expan[0][(int)(R>>24 & 0xffULL)];
  
  /*printf("%012llX\n",Expan[3][(int)(R & 0xffULL)]);
  printf("%012llX\n",Expan[2][(int)(R>>8 & 0xffULL)]);
  printf("%012llX\n",Expan[1][(int)(R>>16 & 0xffULL)]);
  printf("%012llX\n",Expan[0][(int)(R>>24 & 0xffULL)]);*/
  return mes;
}

uint32_t DES_Sbox(uint64_t input)
{
  /*This function computes substitution operation and returns 32-bits array out of 48-bits input*/
  int i = 0;
  uint32_t S = 0x0ULL;
    
  for(i = 0; i < 8; i++) {
    S |= Sbox[7-i][input & 0x3F];
    input >>= 6;
  }
  return S;      
}

uint64_t DES_SST(const uint64_t input)
{
  /*This function permutes 64-bit input*/
  int iBit = 0;

  uint64_t res = 0x0ULL;
  uint64_t one = 0x1ULL;

  for (iBit = 0; iBit<64; iBit++)
	  res |= BIT(input, 64 - sst_table[iBit]) << (63 - iBit);

  
  return res;
}

uint32_t permutation(const uint32_t input)
{
	uint32_t res = 0x0ULL;

	for (int i = 0; i<32; i++)
		if (BIT(input, 32 - p_table[i])
			res |= 0x1ULL << (31 - i);


	return res;
}

uint64_t DES_FP(uint32_t R, uint32_t L) {
	/*This function computes final permutation from right R and left L parts*/
  int i;
  uint64_t mes = 0x0ULL;

  for(i = 3; i >= 0; i--) {
    mes |= FPTab[4 + i][L & 0xff];
    mes |= FPTab[i][R & 0xff];
    L >>= 8;
    R >>= 8;
  }
  return mes;
}

uint64_t DES_Encrypt(const uint64_t plaintext, const uint64_t key_schedule[]) {
	/*Just function for encryption which uses all functions above*/
  int i;
  uint32_t L, R, RStorage; 		/*left and right parts of plaintext*/
  
  DES_IP(plaintext, &L, &R);
  for(i = 0; i < 16; i++) {
    RStorage = R;
    R = permutation( DES_Sbox(DES_E(R) ^ key_schedule[i]) ) ^ L;
    L = RStorage;
  }
  return DES_FP(R, L);
}
