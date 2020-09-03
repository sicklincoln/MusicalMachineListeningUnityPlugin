/*
 *  CalculateFFT.h
 *
 *  Created by Nick Collins on 23/11/2008.
 *  Copyright 2008 Nick Collins. All rights reserved.
 *
 */

#pragma once

#include "KissFFTr/kiss_fftr.h"

//hop and shunt see FeatureFrame

class CalculateFFT {

private:
float * fftbuffer;
	float * zeropad; 	
//float * prefftbuffer;
//float * postfftbuffer;
float * window;

    //float * rin;
    kiss_fft_cpx * sout;
    kiss_fft_scalar * rin;
    
    
    kiss_fftr_cfg cfg;
    
    int fftsize;
    //singleton audio feature extractor instantiated with library load? static object?
    int halffftsize;
    
    
    
    
//vDSP

void prepareHanningWindow();

public:
//default to hanning window for now
//CalculateFFT(int fftsize, int hopsize);
//~CalculateFFT();

int fftsize;
int inputsize; 
int padding; 
int nover2;
int dowindowing; 
//int hopsize;
//int halfsize;

//could have a Spectrum object to hold this data, and override for other spectral representations? 
//returns power spectrum
void calculateFrame(float * input, float * output);

//CalculateFFT::CalculateFFT(int fftsize, int hopsize) : fftsize(fftsize), hopsize(hopsize) {
CalculateFFT::CalculateFFT(int inputsize=1470, int fftsize=2048, int windowflag=0) : inputsize(inputsize), fftsize(fftsize), dowindowing(windowflag) {
	
	nover2= fftsize >>1;
	
	//in place
	//fftbuffer= new float[fftsize];
	
	//prefftbuffer = new float[fftsize];
	//postfftbuffer = new float[fftsize];
	window= new float[fftsize];
	padding = fftsize-inputsize; 
//	
//
//	//not needed
//	zeropad= new float[fftsize];
//
//	for (int i = 0; i < fftsize; ++i) {
//		
//		zeropad[i]=0.0; //assuming 2048-1470 end samples of this should stay zero always
//	
//	}
//	
//	
	prepareHanningWindow();
	
	////////KissFFT///////////////
  
        //kiss_fft_scalar rin[nfft+2];
        //kiss_fft_cpx cout[nfft];
        
        fftsize = fftsizenow;
        halffftsize = fftsize/2;
        
        cfg = kiss_fftr_alloc(fftsize,0,0,0);
        
        //    sout = new kiss_fft_cpx[nfft];
        //
        //    rin = new kiss_fft_scalar[nfft+2];
        
        //ptr = (int*) malloc(100 * sizeof(int));
        sout = (kiss_fft_cpx*) malloc(fftsize * sizeof(kiss_fft_cpx));
        rin = (kiss_fft_scalar*) malloc((fftsize+2) * sizeof(kiss_fft_scalar));
        
        for (int i =0; i< fftsize+2; ++i)
            rin[i] = 0.0;


    
    //fftbuffer = ( float* ) malloc ( fftsize * sizeof ( float ) );
	
	//get LOG2CEIL as per sc_fft
//	
//	int vlog= 0; 
//	
//	int currfftsize= fftsize; 
//	// printf("currfftsize %d vlog %d \n", currfftsize, vlog);
//	while(currfftsize>1) {++vlog; currfftsize>>=1;}
//	//printf("log of n =%d is %d \n", fftsize, vlog); 
//	
//	vlog2n = vlog; //11; //hard code for now log2(fftsize); //10; //N is hard coded as 1024, so 10^2=1024 //log2max(N);
//	//vsetup = create_fftsetup(vlog2n, 0);
//	
    
	//halfsize= fftsize/2;
	
}


CalculateFFT::~CalculateFFT() {
	
    
    free(cfg);
    
    //delete [] sout;
    free(rin);
    free(sout);
    
	free(fftbuffer);

	delete [] window;
	//delete [] zeropad; 
	
}




};