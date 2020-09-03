//(c) Nick Collins 2019 all rights reserved

#pragma once

#include "MMLLSTFT.h"

class SensoryDissonance {
   
public:
    
    int fftsize;
	int topbin;
	int frequencyperbin;
	
	float dissonance;
    
	int maxnumpeaks;
	float peakthreshold;
	
	float * peakfreqs;
	float * peakamps;
	
	float norm;
	int clamp;

    MMLLSTFT * stft;
    
public:
    SensoryDissonance(float sr, MMLLSTFT * stft2048):stft(stft2048) {
        
        //fft, maxpeaks: 100, peakthreshold: 0.1, norm, clamp: 1
        //norm = norm ?? {0.01/maxpeaks};
        
        dissonance = 0.0;
        
        //stft = new MMLLSTFT(1024,512,1);//Hann windowing
   
        fftsize = stft->fftsize; //buf->frames; //ZIN0(1);
        //printf("check fftsize %d \n",fftsize);
        topbin= fftsize*0.25;
        
        frequencyperbin = sr / fftsize;
        
        maxnumpeaks = 100; //ZIN0(1); //100;
        peakthreshold = 0.1; //ZIN0(2);
        peakfreqs = new float[maxnumpeaks]; //(float *)RTAlloc(mWorld, sizeof(float)*maxnumpeaks);
        peakamps = new float[maxnumpeaks]; //(float *)RTAlloc(mWorld, sizeof(float)*maxnumpeaks);
        
        norm = 0.01/maxnumpeaks; //ZIN0(3); //0.01/maxnumpeaks; //fftsize;
        
        clamp = 2; //ZIN0(4);
        
    }
    
    ~SensoryDissonance() {
 
        
        delete [] peakfreqs;
        delete [] peakamps;
    }
    
    float Calculate();
    
    
};
