//(c) Nick Collins 2020 all rights reserved

#include "SpectrumExtractor.h"


void SpectrumExtractor::Calculate(float* input, float * output) {
    
    int ready = stft->next(input,blocksize);
    //int ready2 = stft2048->next(input,blocksize);
    
    if(ready) {

        int i;
        
        float* complex = stft->complex;

        for (int i=0; i< fftsize; ++i) {
            
            output[i] = complex[i];
        }
        
    }
    
}
