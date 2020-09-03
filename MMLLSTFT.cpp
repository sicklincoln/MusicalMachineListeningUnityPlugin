//(c) Nick Collins 2019 all rights reserved

#include "MMLLSTFT.h"


int MMLLSTFT::next(float * input,int n) {
    
    //update by audioblocksize samples
    int ready = windowing->next(input,n);
    
    if(ready) {
        
        int i;
        
        float * store = windowing->store;
        
        //no window function (square window)
        if(windowtype==0) {
            for (i = 0; i< fftsize; ++i) {
                rin[i] = store[i]; //*hanning[i];
                //imags[i] = 0.0;
                
            }
        } else {
            for (i = 0; i< fftsize; ++i) {
                rin[i] = store[i]*hann[i];
                //imags[i] = 0.0;
                
            }
        }
        
        //fft library call
       
        kiss_fftr(cfg,rin,sout);
        
        //results from sout into rin
        
        //misses off Nyquist
        for (i =0; i< fftsize/2; ++i) {
            
            complex[2*i] = sout[i].r;
            complex[2*i+1] = sout[i].i;
            
            
        }
        
        
        //output format is interleaved k*2, k*2+1 real and imag parts
        //DC and 0 then bin 1 real and imag ... nyquist and 0
        
        //power spectrum not amps, for comparative testing
        for (int k = 0; k < halffftsize; ++k) {
            //Math.sqrt(
            int twok = 2*k;
            //powers[k] = ((output[twok] * output[twok]) + (output[twok+1] * output[twok+1]) ); // * fftnormmult;
            
            powers[k] = ((complex[twok] * complex[twok]) + (complex[twok+1] * complex[twok+1]) );
            
            //will scale later in onset detector itself
            
            //powers[k] = ((reals[k] * reals[k]) + (imags[k] * imags[k]) ); // * fftnormmult;
            
            //freqs[k - align] = (2 * k / N) * (sample_rate / 2);
        }
   
        
    }
    
    return ready;
}
