/*
 *  CalculateFFT.cpp
 *  openFrameworks
 *
 *  Created by Nick Collins on 23/11/2008.
 *  Copyright 2008 Nick Collins. All rights reserved.
 *
 */

#include "CalculateFFT.h"
#include <math.h>
#include <iostream>

#define TWOPI 6.28318530717952646f 



//probably have to put this in load() again since 256 or more cos calls
void CalculateFFT::prepareHanningWindow() {
	float ang;
	
	ang=(1.0/inputsize)*TWOPI; //2.0*M_PI;
	
	for(int i=0;i<inputsize;++i)
		window[i]=0.5 - 0.5*cos(ang*i);
	
}


//hopefully no problems with max size of int 
//INT_MAX in limits.h 2147483648/44100/60 = 811.59623847317 minutes of audio in a file

//does all at once, could convert to just one frame at a time? 
void CalculateFFT::calculateFrame(float * input, float * output) {
	
	int j; //i, pos;
	
	//copy first	
	if(dowindowing) {
	for (j=0; j<inputsize; ++j)
			fftbuffer[j] = input[j]* window[j];
	} else {
	for (j=0; j<inputsize; ++j)
		fftbuffer[j] = input[j];  
	}
			
	for (j=0; j<padding; ++j)
		fftbuffer[inputsize+j] = 0.0;  
	

    //calculate FFT
    
    int i;
    
    //copy to rin
    for (i =0; i< fftsize; ++i)
        rin[i] = input[i];
    
    kiss_fftr(cfg,rin,sout);
    
    //results from sout into rin
    
    //misses off DC
    for (i =0; i< fftsize/2; ++i) {
        
        input[2*i] = sout[i].r;
        input[2*i+1] = sout[i].i;
        
        
    }
    
    

	//undo scale factor of 2 caused by complex fft on 2n 
	//can copy to output buffer at the same time? 
	
//	float scale= 0.5;
//	vDSP_vsmul( fftbuffer, 1, &scale, output, 1, fftsize); 
//	

    
	
//	output[0]= postfftbuffer[0]* postfftbuffer[0]; //get power
//	output[0]= 10*log10((953674*output[0])+1);
//	
//	// Squared Absolute so get power
//	//do half of the calculations
//	for(j=1; j<halfsize; j++) {
//		float val1= postfftbuffer[j];
//		float val2= postfftbuffer[fftsize-j];
//		
//		output[j] = (val1*val1) + (val2*val2); 
//		
//		output[j]= 10*log10((953674*output[j])+1);
//		
//		//should do log conversions as 0 to 120 dB (16 bit range- but some samples are 24 bit!) where x is a power after fft, 
//		//multiplier for 96dB range = 3797 
//		//120 dB range = 953674
//		//logpower= 10*log10(3797*x+1)
//		
//		//log conversions up to 105 dB max  (minimum is -30 however!)
//		//float db= 10*log10((bsum*32382)+0.001); 
//		
//		//if(j<5) printf("j %d val %f \t",j, output[j]);
//		//fftbuf[256-j] = 0.0f;
//	}
	
	
	
}
//elements 0 to 512 of array2 first 513 bins/spectral buckets, at 0, 43, 86, ..., 22050 Hz












