#pragma once

/*Perform key schedule*/
void DES_keySchedule(uint64_t key, uint64_t key_schedule[]);

/*Encrypt 64-bit input with the key schedule key_schedule*/
uint64_t DES_Encrypt(const uint64_t plaintext, const uint64_t key_schedule[]);

/*Permute 64-bit input and store the result in left L and right R 32-bit variables (initial DES permutation)*/
void DES_IP(uint64_t plaintext, uint32_t *L, uint32_t *R);

/*Permute left L and right R parts of the encryption (final DES permutation)*/
uint64_t DES_FP(uint32_t R, uint32_t L);

/*Expand a 32 bits to 48 bits (E table)*/
uint64_t DES_E(const uint32_t R);

/*Permute a 32 bits (P permutation)*/
uint32_t DES_P(const uint32_t input);

/*Substitute 48-bits input with 32-bits output (SB operation)*/
uint32_t DES_Sbox(uint64_t input);

uint64_t DES_SST(const uint64_t input);
