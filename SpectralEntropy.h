//(c) Nick Collins 2019 all rights reserved

#pragma once

#include "MMLLSTFT.h"

class SpectralEntropy {
   
    MMLLSTFT * stft;
    int fftsize_, numbands, numbins;
    int * bandindices;
    float * entropies;
    //float * intensities;
    
public:
    SpectralEntropy(MMLLSTFT * stft2048):stft(stft2048) {
        
       	int i, j;
        
        fftsize_ = stft2048->fftsize;
        
        numbands = 1; //ZIN0(2);
        
        numbins = fftsize_/2; //won't use actual Nyquist bin in this UGen
        
        int split = numbins/(numbands);
        
        if(split<1) {
            
            split = 1;
            numbands = numbins;
        }
        
        
        //will include guard element at top
        bandindices = new int[numbands+1]; //(int *)RTAlloc(mWorld, sizeof(int)*(numbands_+1));
        entropies = new float[numbands]; //(float *)RTAlloc(mWorld, sizeof(float)*numbands_);
        //intensities = new float[numbins]; //(float *)RTAlloc(mWorld, sizeof(float)*numbins);
        
        for (i=0; i<numbands; ++i) {
            
            entropies[i] = 0.0f;
            
            bandindices[i] = split*i;
            
        }
        
        //guard can be one above actual final array slot index since always use less than in code below
        bandindices[numbands] = numbins; //Nyquist position
        
    }
    
    ~SpectralEntropy() {
 
        delete [] bandindices;
        delete [] entropies;
        //delete [] intensities;
        
    }
    
    float Calculate();
    
    
};
