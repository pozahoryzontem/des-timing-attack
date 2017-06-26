#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
#include <stdint.h>
#include <numeric>
#include <omp.h>
#include <inttypes.h>

#include "tables.h"
#include "des.h"

#define MAXIMUM_THREADS 8


using namespace std;

template <class T1, class T2> double ComputePCC_vector(vector<T1> &x, vector<T2> &y);
double ComputePCC_float_float_vector(vector<float> &x, vector<float> &y) { return ComputePCC_vector<float, float>(x,y); }


typedef float (*des_bin_op1) (uint16_t plainByte, uint16_t key);

typedef float (*des_bin_op) (uint8_t plainByte, uint8_t key);

float sbox7 (uint8_t plainByte, uint8_t key) { return (float)__builtin_popcount(Sbox[7][plainByte ^ key]); }
float sbox6 (uint8_t plainByte, uint8_t key) { return (float)__builtin_popcount(Sbox[6][plainByte ^ key]); }
float sbox5 (uint8_t plainByte, uint8_t key) { return (float)__builtin_popcount(Sbox[5][plainByte ^ key]); }
float sbox4 (uint8_t plainByte, uint8_t key) { return (float)__builtin_popcount(Sbox[4][plainByte ^ key]); }
float sbox3 (uint8_t plainByte, uint8_t key) { return (float)__builtin_popcount(Sbox[3][plainByte ^ key]); }
float sbox2 (uint8_t plainByte, uint8_t key) { return (float)__builtin_popcount(Sbox[2][plainByte ^ key]); }
float sbox1 (uint8_t plainByte, uint8_t key) { return (float)__builtin_popcount(Sbox[1][plainByte ^ key]); }
float sbox0 (uint8_t plainByte, uint8_t key) { return (float)__builtin_popcount(Sbox[0][plainByte ^ key]); }

float sbox23 (uint16_t plainByte, uint16_t key) { return (float)__builtin_popcount(Sbox[6][( plainByte >> 6) ^ (key >> 6)])  + (float)__builtin_popcount(Sbox[7][( plainByte & 0x3f) ^ (key & 0x3f)]); }
float sbox22 (uint16_t plainByte, uint16_t key) { return (float)__builtin_popcount(Sbox[4][( plainByte >> 6) ^ (key >> 6)])  + (float)__builtin_popcount(Sbox[5][( plainByte & 0x3f) ^ (key & 0x3f)]); }
float sbox21 (uint16_t plainByte, uint16_t key) { return (float)__builtin_popcount(Sbox[2][( plainByte >> 6) ^ (key >> 6)])  + (float)__builtin_popcount(Sbox[3][( plainByte & 0x3f) ^ (key & 0x3f)]); }
float sbox20 (uint16_t plainByte, uint16_t key) { 
	//printf("plain: %" PRIu16 "\n", plainByte);
	//printf("key: %" PRIu16 "\n", key);
	//printf("plain first: %" PRIu16 "\n", plainByte >> 6);
	//printf("plain second: %" PRIu16 "\n", plainByte & 0x3f);
	//printf("key first: %" PRIu16 "\n", key >> 6);
	//printf("key second: %" PRIu16 "\n", key & 0x3f);	
	
	return (float)__builtin_popcount(Sbox[0][( plainByte >> 6) ^ (key >> 6)])  + (float)__builtin_popcount(Sbox[1][( plainByte & 0x3f) ^ (key & 0x3f)]);
	
	
	
}

void TimingAttackDES_OneSbox(unsigned int nEnc, char* realtimeFilename, char* plaintextFilename, char* resultFilename, uint8_t SboxNum)
{
	const uint16_t keyCandidates = 64;
	const uint8_t nBytes = 8;
	
	uint8_t iByte = 0;
	uint32_t iKey = 0;
	uint32_t iEnc = 0;
	uint32_t L = 0;
	uint32_t R = 0;
	uint64_t tmpExpR = 0x0ULL;
	uint64_t tmpIP = 0x0ULL;
	double tmp = 0;

	
	vector< vector<double> > pcc;		/*The resulted likelihood information*/
	vector<float> realTime;			/*Explicit real time*/
	vector<float> model;			/*Timing model that is used as a prediction*/
	vector<uint8_t> keyArray;		/*Key value*/
	vector<vector<uint8_t> > expR; 	/*Expanded R, i.e. E(R) permutation of DES*/

	des_bin_op func[] = {sbox0, sbox1, sbox2, sbox3, sbox4, sbox5, sbox6, sbox7};
	
	/*Dimension assignments*/
	expR.resize(nBytes);
	for (iByte = 0; iByte < nBytes; iByte++)
		expR[iByte].resize(nEnc);

	pcc.resize(nBytes);
	for (iByte = 0; iByte < nBytes; iByte++)
		pcc[iByte].resize(keyCandidates);
	
	model.resize(nEnc);
	realTime.resize(nEnc);
	keyArray.resize(nEnc);


	/*-------------------------------------------------------------*/
	/*Read timing*/
	/*-------------------------------------------------------------*/
	FILE *flTime;
	
	flTime = fopen(realtimeFilename,"r");
	if (flTime == NULL) {
		printf("Can't open the file %s.\n",realtimeFilename);
		exit(-1);
	}
	
	printf("Reading %d time values. Please wait.\n",nEnc);
	for (iEnc = 0; iEnc < nEnc; iEnc++){
		if ( fscanf(flTime,"%lf\n",&tmp) == 1){
			realTime[iEnc] = (float)tmp;
		}
		else{
			printf("Error during reading time file.\n");
			exit(-1);
		}
	}
	fclose(flTime);
	
	/*-------------------------------------------------------------*/
	/*Read plaintexts*/
	/*-------------------------------------------------------------*/	
	FILE *flPlaintext;
	
	flPlaintext = fopen(plaintextFilename, "r");
	if (flPlaintext == NULL) {
		printf("Can't open the file %s.\n",plaintextFilename);
		exit(-1);
	}

	printf("Reading %d plaintexts. Please wait.\n",nEnc);
	for (iEnc = 0; iEnc < nEnc; iEnc++){
		if ( fscanf(flPlaintext,"%llX",&tmpIP) == 1) {
			/*Read the plaintext, permute in with DES Initial Permutation and store the E(R)*/
			/*i.e. the extension of the right part only.*/
			DES_IP(tmpIP, &L, &R);
			tmpExpR = DES_E(R);
			
			for (iByte=0; iByte<nBytes; iByte++)
				expR[iByte][iEnc] = (tmpExpR >> ((nBytes - iByte - 1)*(unsigned int)log2(keyCandidates))) & (keyCandidates-1);
		}	
		else{
			printf("Error during reading plaintext file.\n");
			exit(-1);
		}		
	}
	fclose(flPlaintext);
	
	
	/*-------------------------------------------------------------*/
	/*Compute model and correlation*/
	/*-------------------------------------------------------------*/
	FILE *flPcc;
	
	flPcc = fopen(resultFilename,"w");
	if (flPcc == NULL) {
		printf("Can't open the file %s.\n",resultFilename);
		exit(-1);
	}
	
	printf("Model computation and verification process.\n");
	fflush(stdout);
	
		for (iKey = 0; iKey < keyCandidates; iKey++){
			/*Fill the whole key with the same key candidate*/
			std::fill(keyArray.begin(),keyArray.end(),iKey);
		
			/*Compute time model*/
			/*Initialize constants for parallezation*/
			const unsigned int num = nEnc;
			const unsigned int constByte = SboxNum;

			int i = 0;
			int nthreads = 0;

			omp_set_dynamic(0);
			omp_set_num_threads(MAXIMUM_THREADS);

			#pragma omp parallel 
		  	#pragma omp single
		  	nthreads = omp_get_num_threads();

		  	#pragma omp parallel default(none) shared(nthreads, expR, model, keyArray, func) 
		  	{
		       		int tid 	= omp_get_thread_num();
		       		int nitems 	= num/nthreads;
		       		int start 	= tid*nitems;
		       		int end   	= start + nitems;

		       		if (tid == nthreads-1) end = num;

		       		#pragma omp for
		       		for (i=0; i<nthreads; i++)
					std::transform (expR[constByte].begin()+start, expR[constByte].begin()+end, keyArray.begin()+start, model.begin()+start, func[constByte]);
		  	}

		  	/*Compute the correlation between the model and the real time*/
		  	pcc[SboxNum][iKey] = ComputePCC_float_float_vector(realTime, model);
			

			printf("Progress: %.2lf%%\r",(double)(iKey+1)*(double)100/(double)(keyCandidates));
			fflush(stdout);	
		}/*iKey*/
	
	/*Print out the results*/
	fprintf(flPcc,"#           Key        pcc byte\n");
	for (iKey = 0; iKey < keyCandidates; iKey++)
		fprintf(flPcc,"%d %15f\n",iKey, pcc[SboxNum][iKey]*pcc[SboxNum][iKey]*100000);
		
	fclose(flPcc);
}

void TimingAttackDES_TwoSbox(unsigned int nEnc, char* realtimeFilename, char* plaintextFilename, char* resultFilename, uint8_t SboxNum)
{
	const uint16_t keyCandidates = 64*64;
	const uint8_t nBytes = 4;
	
	uint8_t iByte = 0;
	uint32_t iKey = 0;
	uint32_t iEnc = 0;
	uint32_t L = 0;
	uint32_t R = 0;
	uint64_t tmpExpR = 0x0ULL;
	uint64_t tmpIP = 0x0ULL;
	double tmp = 0;

	
	vector< vector<double> > pcc;		/*The resulted likelihood information*/
	vector<float> realTime;			/*Explicit real time*/
	vector<float> model;			/*Timing model that is used as a prediction*/
	vector<uint16_t> keyArray;		/*Key value*/
	vector<vector<uint16_t> > expR; 	/*Expanded R, i.e. E(R) permutation of DES*/

	des_bin_op1 func[] = {sbox20, sbox21, sbox22, sbox23};
	
	/*Dimension assignments*/
	expR.resize(nBytes);
	for (iByte = 0; iByte < nBytes; iByte++)
		expR[iByte].resize(nEnc);

	pcc.resize(nBytes);
	for (iByte = 0; iByte < nBytes; iByte++)
		pcc[iByte].resize(keyCandidates);
	
	model.resize(nEnc);
	realTime.resize(nEnc);
	keyArray.resize(nEnc);


	/*-------------------------------------------------------------*/
	/*Read timing*/
	/*-------------------------------------------------------------*/
	FILE *flTime;
	
	flTime = fopen(realtimeFilename,"r");
	if (flTime == NULL) {
		printf("Can't open the file %s.\n",realtimeFilename);
		exit(-1);
	}
	
	printf("Reading %d time values. Please wait.\n",nEnc);
	for (iEnc = 0; iEnc < nEnc; iEnc++){
		if ( fscanf(flTime,"%lf\n",&tmp) == 1){
			realTime[iEnc] = (float)tmp;
		}
		else{
			printf("Error during reading time file.\n");
			exit(-1);
		}
	}
	fclose(flTime);
	
	/*-------------------------------------------------------------*/
	/*Read plaintexts*/
	/*-------------------------------------------------------------*/	
	FILE *flPlaintext;
	
	flPlaintext = fopen(plaintextFilename, "r");
	if (flPlaintext == NULL) {
		printf("Can't open the file %s.\n",plaintextFilename);
		exit(-1);
	}

	printf("Reading %d plaintexts. Please wait.\n",nEnc);
	for (iEnc = 0; iEnc < nEnc; iEnc++){
		if ( fscanf(flPlaintext,"%llX",&tmpIP) == 1) {
			/*Read the plaintext, permute in with DES Initial Permutation and store the E(R)*/
			/*i.e. the extension of the right part only.*/
			DES_IP(tmpIP, &L, &R);
			tmpExpR = DES_E(R);		
			for (iByte=0; iByte<nBytes; iByte++) {
				expR[iByte][iEnc] = (tmpExpR >> ((nBytes - iByte - 1)*(unsigned int)log2(keyCandidates))) & (keyCandidates-1);				
			}
		}	
		else{
			printf("Error during reading plaintext file.\n");
			exit(-1);
		}		
	}
	fclose(flPlaintext);
	
	
	/*-------------------------------------------------------------*/
	/*Compute model and correlation*/
	/*-------------------------------------------------------------*/
	FILE *flPcc;
	
	flPcc = fopen(resultFilename,"w");
	if (flPcc == NULL) {
		printf("Can't open the file %s.\n",resultFilename);
		exit(-1);
	}
	
	printf("Model computation and verification process.\n");
	fflush(stdout);
	
		for (iKey = 0; iKey < keyCandidates; iKey++){
			/*Fill the whole key with the same key candidate*/
			std::fill(keyArray.begin(),keyArray.end(),iKey);
		
			/*Compute time model*/
			/*Initialize constants for parallezation*/
			const unsigned int num = nEnc;
			const unsigned int constByte = SboxNum;

			int i = 0;
			int nthreads = 0;

			omp_set_dynamic(0);
			omp_set_num_threads(MAXIMUM_THREADS);

			#pragma omp parallel 
		  	#pragma omp single
		  	nthreads = omp_get_num_threads();

		  	#pragma omp parallel default(none) shared(nthreads, expR, model, keyArray, func) 
		  	{
		       		int tid 	= omp_get_thread_num();
		       		int nitems 	= num/nthreads;
		       		int start 	= tid*nitems;
		       		int end   	= start + nitems;

		       		if (tid == nthreads-1) end = num;

		       		#pragma omp for
		       		for (i=0; i<nthreads; i++)
					std::transform (expR[constByte].begin()+start, expR[constByte].begin()+end, keyArray.begin()+start, model.begin()+start, func[constByte]);
		  	}

		  	/*Compute the correlation between the model and the real time*/
		  	pcc[SboxNum][iKey] = ComputePCC_float_float_vector(realTime, model);
			

			printf("Progress: %.2lf%%\r",(double)(iKey+1)*(double)100/(double)(keyCandidates));
			fflush(stdout);	
		}/*iKey*/
	
	/*Print out the results*/
	fprintf(flPcc,"#           Key        pcc byte\n");
	for (iKey = 0; iKey < keyCandidates; iKey++)
		fprintf(flPcc,"%d %15f\n",iKey, pcc[SboxNum][iKey]*pcc[SboxNum][iKey]*100000);
		
	fclose(flPcc);
}


/**********************************************************************************************
* Function that computes pearson correlation coefficient between two vectors of the same length
* INPUT: x - first vector                                                                     
*        y - second vector                                                                                                               
*OUTPUT: pcc - pearson correlation coefficient                                                
**********************************************************************************************/
template <class T1, class T2> double ComputePCC_vector(vector<T1> &x, vector<T2> &y)
{
	double sumX = 0.0,  sumY = 0.0;
	double sumX2 = 0.0, sumY2 = 0.0;
	double sumXY = 0.0;
	const unsigned int num = x.size();
	const double init = 0;
	
	double pcc = 0.0;
	int i = 0;
	int nthreads = 0;

	omp_set_dynamic(0);
	omp_set_num_threads(MAXIMUM_THREADS);
	
	#pragma omp parallel 
  	#pragma omp single
  	nthreads = omp_get_num_threads();

  	#pragma omp parallel default(none) reduction(+:sumX, sumX2, sumY, sumY2, sumXY) shared(nthreads,x,y) 
  	{
       		int tid 	= omp_get_thread_num();
       		int nitems 	= num/nthreads;
       		int start 	= tid*nitems;
       		int end   	= start + nitems;
       		
       		if (tid == nthreads-1) end = num;

       		#pragma omp for
       		for (i=0; i<nthreads; i++){
          		sumX2 	+= std::inner_product( x.begin() + start, x.begin()+end, x.begin()+start, init);
				sumX 	+= std::accumulate(x.begin()+start, x.begin()+end, init);
				sumY2 	+= std::inner_product( y.begin() + start,y.begin()+end, y.begin()+start, init);
				sumY 	+= std::accumulate(y.begin()+start, y.begin()+end, init);
				sumXY 	+= std::inner_product( x.begin() + start, x.begin()+end, y.begin()+start, init);
			}
  	}
	
	pcc = (sumXY - sumX*sumY/num) / sqrt((sumX2 - sumX * sumX / num) * (sumY2 - sumY * sumY / num));
	
	if (!isnan(pcc))
		return pcc;
	else{
		/*printf("\t\tDuring pcc computation a NaN or INFINITY result was obtained\n");
		fflush(stdout);*/
		return 0;
	}
	
	return 0;
}
