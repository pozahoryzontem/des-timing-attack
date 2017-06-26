#pragma once

void TimingAttackDES(unsigned int nEnc, char* realtimeFilename, char* plaintextFilename, char* resultFilename);
void TimingAttackDES_OneSbox(unsigned int nEnc, char* realtimeFilename, char* plaintextFilename, char* resultFilename, uint8_t SboxNum);
void TimingAttackDES_TwoSbox(unsigned int nEnc, char* realtimeFilename, char* plaintextFilename, char* resultFilename, uint8_t SboxNum);
