//(c) Nick Collins 2020 all rights reserved

#pragma once

#include "MMLLSTFT.h"

class SpectrumExtractor {
   
public:
    float sampleRate;
    int blocksize;
    int fftsize;
    int hopsize;
    int windowtype;
    
    MMLLSTFT * stft;
    
public:
    SpectrumExtractor(float sampleRate, int blocksize, int fftsize, int hopsize, int windowtype):sampleRate(sampleRate),blocksize(blocksize),fftsize(fftsize),hopsize(hopsize),windowtype(windowtype) {
        
        //assumes blocksize = hopsize so can update once per calculation exactly
        
        stft = new MMLLSTFT(fftsize,hopsize,windowtype);//Hann windowing
        
        //stft1024 = new MMLLSTFT(1024,512,1);//Hann windowing
     
        
    }
    
    ~SpectrumExtractor() {

        delete stft;
        
    }
    
    void Calculate(float* input, float * output);
    
    
};
