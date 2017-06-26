/******************************************************************
 * Implementation of DES cryptography algorithm by Roman
 * Ver. 0.5
 * Date 03.12.2008
 ******************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <inttypes.h>
#include "des.h"
#include "attack.h"

__inline__ uint64_t rdtsc(){
	uint32_t lo,hi;
	__asm__ __volatile__ 
		("rdtsc" : "=a" (lo), "=d" (hi));
	return (uint64_t)hi << 32 | lo;
}

void functional_test(void) {
	uint_fast64_t ks[16], k, p, c, r;

	k = 0x3030456789ABCDEFULL;
	p = 0xAA6F7720697320EEULL;
	c = 0x89081EB8E64C8951ULL;
	DES_keySchedule(0x3030456789ABCDEFULL, ks);
	r = DES_Encrypt(p, ks);
	if(r != c) {
		fprintf(stderr, "Error: DES(k=%016llX, p=%016llX) = %016llX (should be %016llX)\n", k, p, r, c);
		exit(-1);
	}
}

void CreateTimeDatabase(uint32_t nEnc, char* realtimeFilename, char* plaintextFilename)
{
	uint32_t iEnc = 0, i = 0;
	
	uint16_t nRep = 50;
	uint16_t iRep = 0;
	
	double a = 0, b = 0;
	double avg = 0.0;
	
	uint64_t plaintext = 0x0ULL, ciphertext = 0x0ULL;
	uint64_t ks[16];

	/*Extend the key*/
	DES_keySchedule(0x3030456789ABCDEFULL, ks);
	
	FILE *flPlaintext;
	FILE *flTime;
	flPlaintext = fopen(plaintextFilename,"w");
	if (flPlaintext == NULL){
		printf("Can't open the file %s.\n",plaintextFilename);
		exit(-1);
	}

	flTime = fopen(realtimeFilename, "w");
	if (flTime == NULL){
		printf("Can't open the file %s.\n",realtimeFilename);
		exit(-1);
	}

	/*Initialize RND*/
	srand ( rdtsc() );
	for (iEnc=0;iEnc<nEnc;iEnc++){
		/* generate random plaintext*/
		for(int i =0; i <4; i++){
			plaintext <<= 16;
		}
			uint8_t b1 = 0;
			uint8_t b2 = 0;
			uint8_t b3 = 0;
			do {
				b1 = rand() % 64;
				b2 = rand() % 64;
				b3 = rand() % 64;
			} while(b1 == b2 || b1 == b3 || b2 == b3);
			
  			uint64_t one = 0x1ULL;
			plaintext |= one << b1;		
			plaintext |= one << b2;
			plaintext |= one << b3;

		/*Compute the average time that is take to perform encryption of the*/ 
		/*previously generated plaintext*/
		uint64_t plaintext_sst = 0x0ULL;
		plaintext_sst = DES_SST(plaintext);
		avg = 0.0;
		for(iRep = 0; iRep < nRep; iRep++) {
			a = rdtsc();			
			ciphertext = DES_Encrypt(plaintext_sst, ks);
			b = rdtsc();
			avg += (double)(b-a);
		}
		
		fprintf(flTime,"%f\n",avg/nRep);
		fprintf(flPlaintext,"%llX\n",plaintext);
		
		printf("Progress: %.2lf%%\r",(double)(iEnc+1)*(double)100/(double)(nEnc));
		fflush(stdout);	
	}
	
	fclose(flPlaintext);
	fclose(flTime);
}


int main(int argc, char **argv) {

	unsigned int nEnc [11] = {1048576, 2097152, 2097152*2, 2097152*4, 2097152*8, 2097152*16, 67108864, 67108864*2, 67108864*4, 67108864*8, 67108864*16};

	uint8_t byteToRecover = 0;

	char realtimeFilename[40];
	char plaintextFilename[40];
	sprintf( realtimeFilename, "data/sst/des_time.dat");
	sprintf( plaintextFilename, "data/sst/des_plaintext.dat");
	printf("%s\n", plaintextFilename);

	CreateTimeDatabase(nEnc[7], realtimeFilename, plaintextFilename);
	for(uint8_t j = 0; j < 8; j++){
					char resultFilename[40];
					sprintf( resultFilename, "data/sst/pccsbox%d.dat", j+1);
					printf("%s\n", resultFilename);
					TimingAttackDES_OneSbox(nEnc[7], realtimeFilename, 						plaintextFilename, resultFilename, j);
	}
/*
	char resultFilename[40];
	char resultFilename1[40];
	char resultFilename2[40];
	sprintf( resultFilename, "data/2/pcc2sbox%d_top3_6111232.dat", 3);
	printf("%s\n", resultFilename);
	TimingAttackDES_TwoSbox(6111232, realtimeFilename, plaintextFilename, resultFilename, 2);
	*/
	/*	
	char resultFilename2[] = "data/5/pccsbox2.dat";
		TimingAttackDES_OneSbox(4898816, realtimeFilename, 				plaintextFilename, resultFilename2, 1);
	*/
	/*
	sprintf( resultFilename1, "data/pcc2sbox%d_top3_6029312.dat", 1);
	printf("%s\n", resultFilename);
	TimingAttackDES_TwoSbox(nEnc[2] + nEnc[1] - nEnc[0]/4, realtimeFilename, plaintextFilename, resultFilename1, 0);
	
	sprintf( resultFilename2, "data/2/pcc2sbox%d_top3_%d.dat", 2, nEnc[1] + nEnc[0]/2 + nEnc[0]/8);
	printf("%s\n", resultFilename2);
	TimingAttackDES_TwoSbox(nEnc[1] + nEnc[0]/2 + nEnc[0]/8, realtimeFilename, plaintextFilename, resultFilename2, 1);



	/*
	for(uint8_t k = 0; k < 10; k++){	
		for(uint8_t j = 0; j < 8; j++){
			char resultFilename[40];
			sprintf( resultFilename, "data/pw%d/pccsbox%d.dat", k+20, j+1);
			printf("%s\n", resultFilename);
			TimingAttackDES_OneSbox(nEnc[k], realtimeFilename, plaintextFilename, 				resultFilename, j);
		}
	}
	/*
	for(uint8_t i = 0; i < 8; i++){
			char realtimeFilename[40];
			char plaintextFilename[40];
			sprintf( realtimeFilename, "data/%d/des_time.dat", i+1);
			sprintf( plaintextFilename, "data/%d/des_plaintext.dat", i+1);
			printf("%s\n", plaintextFilename);
			CreateTimeDatabase(nEnc[6], realtimeFilename, plaintextFilename);
			for(uint8_t k = 0; k < 7; k++){	
				for(uint8_t j = 0; j < 8; j++){
					char resultFilename[40];
					sprintf( resultFilename, "data/%d/pw%d/pccsbox%d.dat", i+1,  k+20, j+1);
					printf("%s\n", resultFilename);
					TimingAttackDES_OneSbox(nEnc[k], realtimeFilename, plaintextFilename, resultFilename, j);
				}
		}
	}
	*/
	/*	
	for(uint8_t k = 0; k < 7; k++){	

		for(uint8_t i = 0; i < 8; i++){
			char resultFilename[40];
			sprintf( resultFilename, "data/3/pw%d/pccsbox%d.dat", k+20, i+1);
			printf("%s\n", resultFilename);
			TimingAttackDES_OneSbox(nEnc[k], realtimeFilename3, 				plaintextFilename3, resultFilename, i);
		}
		for(uint8_t i = 0; i < 4; i++){
			char resultFilename[40];
			sprintf( resultFilename, "data/1/pw%d/pcc2sbox%d.dat", k+20, i+1);
			printf("%s\n", resultFilename);
			TimingAttackDES_TwoSbox(nEnc[k], realtimeFilename, 				plaintextFilename, resultFilename, i);
		}		
	}

		//5/22/1-2
		char resultFilename[] = "data/5/pcc2sbox1.dat";
		TimingAttackDES_TwoSbox(nEnc[2] + (1048576/8) + (1048576/16) + (1048576/32), realtimeFilename, plaintextFilename, resultFilename, 0);		

		char resultFilename[] = "data/5/pccsbox1.dat";
		TimingAttackDES_OneSbox(nEnc[2] + (1048576/2) + (1048576/8) + (1048576/32) + (1048576/64), realtimeFilename, plaintextFilename, resultFilename, 0);

		char resultFilename2[] = "data/5/pccsbox2.dat";
		TimingAttackDES_OneSbox(nEnc[2] + (1048576/2), realtimeFilename, 				plaintextFilename, resultFilename2, 1);


		//2/23/1-2
		char resultFilename[] = "data/2/pcc2sbox1.dat";
		TimingAttackDES_TwoSbox(nEnc[3] + 1048576 + (1048576/8), realtimeFilename, plaintextFilename, resultFilename, 0);
	
		char resultFilename[] = "data/2/pccsbox1.dat";
		TimingAttackDES_OneSbox(nEnc[3] + 2097152/2 + 2097152/8, realtimeFilename, plaintextFilename, resultFilename, 0);	

		//5/23/5-6
		char resultFilename[] = "data/5/pccsbox5.dat";
		TimingAttackDES_OneSbox(nEnc[3] + nEnc[0] + nEnc[0]/2 + nEnc[0]/4 + nEnc[0]/16, realtimeFilename, plaintextFilename, resultFilename, 4);		

		char resultFilename[] = "data/5/pcc2sbox3.dat";
		TimingAttackDES_TwoSbox(nEnc[3] + nEnc[0] + nEnc[0]/2 + nEnc[0]/4, realtimeFilename, plaintextFilename, resultFilename, 2);

	*/	

		

return 0;
}



