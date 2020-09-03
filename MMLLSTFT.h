//(c) Nick Collins 2019 all rights reserved

#pragma once

#include "MMLLwindowing.h"
#include <math.h>
#include "kiss_fftr.h"

//https://stackoverflow.com/questions/758001/log2-not-found-in-my-math-h
//static inline double log2(double n)
//{
//    return log(n) * M_LOG2E;
//}

class MMLLSTFT {
private:
   
    MMLLwindowing * windowing;
    //float * windowdata;
    float * hann;
    
    kiss_fft_cpx * sout;
    kiss_fft_scalar * rin;
    kiss_fftr_cfg cfg;
    
    float fftnormmult;
    
public:
    int fftsize,hopsize,halffftsize,windowtype;
    
    float * powers;
    float * complex;
    //float * reals;
    
    
public:
    MMLLSTFT(int fftsize=1024,int hopsize=512,int windowtype=0): fftsize(fftsize), hopsize(hopsize),windowtype(windowtype) {
        
        int i;
        
        halffftsize = fftsize/2;

        windowing = new MMLLwindowing(fftsize,hopsize);
        
        //fft = new FFTR(fftsize);
        
        //fft.setupFFT(fftsize);
        
        //windowdata = new float[fftsize]; //begins as zeroes
        hann = new float[fftsize];
        
        float ang = (2.0*M_PI)/fftsize;
        
        for(i=0;i<fftsize;++i)
        hann[i]=0.5 - 0.5*cos(ang*i);
        
                            
        cfg = kiss_fftr_alloc(fftsize,0,0,0);
        
        //    sout = new kiss_fft_cpx[nfft];
        //
        //    rin = new kiss_fft_scalar[nfft+2];
        
        //ptr = (int*) malloc(100 * sizeof(int));
        sout = new kiss_fft_cpx[fftsize]; //(kiss_fft_cpx*) malloc(fftsize * sizeof(kiss_fft_cpx));
        rin = new kiss_fft_scalar[(fftsize+2)]; //(kiss_fft_scalar*) malloc((fftsize+2) * sizeof(kiss_fft_scalar));
        
        for (i =0; i< fftsize+2; ++i)
        rin[i] = 0.0;
                
        //initialised containing zeroes
        powers = new float[halffftsize];

        complex = new float[fftsize];

        //real = new float[fftsize+2];
        
        //imags = new float[fftsize);
        
        //4 =2*2 compensates for half magnitude if only take non-conjugate part, fftsize compensates for 1/N
        fftnormmult = 4*fftsize; //*fftsize;// /4; //1.0/fftsize;  or 1/(fftsize.sqrt)
        
        
        
    }
    
    ~MMLLSTFT() {
        
        delete windowing;
        
        delete [] powers;
        delete [] complex;
        
        delete [] hann;
        
        delete [] sout;
        delete [] rin;
        
        //delete [] reals;
        
    }
    
    int next(float * input,int n);
    
    
};
