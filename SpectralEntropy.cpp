//(c) Nick Collins 2019 all rights reserved

#include "SpectralEntropy.h"


//only calculating for one band at present, no need to return multiple floats
float SpectralEntropy::Calculate() {
    
    int i,j;
    
    //float * data= stft->complex;
    float * powers= stft->powers;
    
    //float real, imag;
    float intensity;
    
    //data[1] = 0.0f; //avoid issues with dc, nyquist packed together, just want dc here
    
    for  (j=0; j<numbands; ++j) {
        
        int start = bandindices[j];
        int end = bandindices[j+1];
        
        float max = 0.0f;
        float entropysum = 0.0f;
        
        //less than because of guard elements
        for  (i=start; i<end; ++i) {
            
//            int index = 2*i;
//            real = data[index];
//            imag = data[index+1];
//            intensity = (real*real) + (imag*imag);
//
            intensity = powers[i];
            //intensities[i] = intensity;
            
            if(intensity>max) {
                
                max = intensity;
                
            }
            
        }
        
        if(max>0.0f) {
            
            max = 1.0f/max; //will be used as straight multiplier in calculation now
            
            for  (i=start; i<end; ++i) {
                
                //float prob = intensities[i] * max;
                float prob = powers[i] * max;
                
                //negative worked in via -=
                if(prob>0.0f) entropysum -= prob * log2(prob);
                
                //leave sum alone otherwise, has no contribution if 0.0 probability
            }
            
            entropies[j] = entropysum;
            
        } else
            entropies[j] = 0.0f;
        
    }
	
    
    //for (i=0; i<numbands; ++i)
	//	ZOUT0(i) = entropies[i];
    
    return entropies[0];
    
	
}









