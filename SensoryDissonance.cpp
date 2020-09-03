//(c) Nick Collins 2019 all rights reserved

//William A. Sethares method (see e.g., Consonance-Based Spectral Mappings, CMJ 22(1): 56-72, 1998).
//this is the harmonic dissonance method, looking for peaks in spectrum, and looking at proximity of peaks as indicating potential dissonance
//An alternative roughness formulation is a multiband filterbank with amplitude modulation detection in each band looking for rates of 15-100Hz or so

//outputs dissonance measure
//could also output number of detected peaks as an interesting auxilliary noisiness measure (could get average dissonnance per peak from that, though not especially meaingful in itself?)

#include "SensoryDissonance.h"


float SensoryDissonance::Calculate() {
	
	//int i, j;
    
    //make sure in real and imag form
    //SCComplexBuf * complexbuf = ToComplexApx(buf);
    //float * data= (float *)ToComplexApx(buf);
    float * powers= stft->powers;
    
    //int numindices= numindices_;
    
    //float real, imag;
    //int index;
    
    int numpeaks = 0;
    
    float intensity;
    float position;
    
    float threshold = peakthreshold;
    
    float prev=0.0, now=0.0, next=0.0;
    
    //float frequencyperbin = frequencyperbin_;
    
    //float totalpeakpower = 0.0f;
    float temp1, refinement;
    
    for (int j=1; j<=topbin; ++j) {
        
        //				index = 2*j;
        //				real= data[index];
        //				imag= data[index+1];
        intensity = powers[j]; //(real*real) + (imag*imag);
        
        next= intensity;
        
        if(j>=3) {
			
            //hunt for peaks
            
            //look for peak by scoring within +-3
            //assume peak must be centrally greater than 60dB say
            
            //powertest_
            //minpeakdB_ was 60
            
            if (now>threshold)  {
                
                //y1= powerspectrum_[i-1];
                //				//y2= valuenow;
                //				y3= powerspectrum_[i+1];
                //
                if ((now>prev) && (now>next)) {
                    
                    //second peak condition; sum of second differences must be positive
                    //NCfloat testsum= (valuenow - powerspectrum_[i-2]) + (valuenow - powerspectrum_[i+2]);
                    
                    //if (testsum>0.0) {
                    
                    //refine estimate of peak using quadratic function
                    //see workbook 28th Jan 2010
                    
                    temp1= prev+next-(2*now);
                    
                    if (fabs(temp1)>0.00001) {
                        position=(prev-next)/(2*temp1);
                        
                        //running quadratic formula
                        refinement = (0.5*temp1*(position*position)) + (0.5*(next-prev)*position) + now;
                        //refinement= y2 -  (((y3-y1)^2)/(8*temp1));
                        
                    } else {
                        //degenerate straight line case; shouldn't occur
                        //since require greater than for peak, not equality
                        
                        position=0.0; //may as well take centre
                        
                        //bettervalue= max([y1,y2,y3]); %straight line through them, find max
                        
                        refinement= now; //must be max for else would have picked another one in previous calculation! %max([y1,y2,y3]);
                        
                    }
                    
                    //correct??????????????????????????????
                    peakfreqs[numpeaks] = (j-1+position)*frequencyperbin; //frequencyconversion;
                    //printf("peakfrequencies %d is %f from i %d position %f freqperbin %f \n", numpeaks_,peakfrequencies_[numpeaks_],i, position, frequencyperbin_);
                    
                    peakamps[numpeaks] = sqrt(refinement); //Sethares formula requires amplitudes
                    //totalpeakpower += refinement;
                    
                    //cout << " peak " << numpeaks_ << " " << peakfrequencies_[numpeaks_] << " " << refinement << " " ;
                    
                    ++numpeaks;
                    
                    //}
                    
                }
                
            }
            
            //test against maxnumberpeaks_
            if ( numpeaks == maxnumpeaks )
                break;
            
            
            
        }
        
        prev = now; now=next;
        
        
        
        
        
        
        
    }
    
    
    //now have list of peaks: calculate total dissonance:
    
    //iterate through peaks, matching each to min of next 10, and no more than octave, using Sethares p. 58 CMJ article
    
    float dissonancesum = 0.0;
    
    float f1, v1, f2, v2;
    float d;
    float diff; //, minf;
    float s, a, b;
    float octave;
    
    for (int i=0; i<(numpeaks-1); ++i) {
        
        f1 = peakfreqs[i];
        v1 = peakamps[i];
        s = 0.24f/(0.21f*f1+19.f); //constant needed as denominator in formula
        a = -3.5f*s;
        b= -5.75f*s;
        
        octave = 2.0f*f1;
        
        //sc_min(i+20,numpeaks)
        int test = i+20;
        if(numpeaks<test) test = numpeaks;
        
        for (int k=i+1; k<test; ++k) {
            
            f2 = peakfreqs[k];
            v2 = peakamps[k];
            
            if(f2>octave) break; //shortcut escape if separated by more than an octave
            
            diff = f2-f1; //no need for fabs, f2>f1
            //minf =  //always f1 lower
            
            d = v1*v2*(exp(a*diff) - exp(b*diff));
            
            dissonancesum += d;
        }
        
    }
    
    //no clamping, let's see what happens (can process more later if desired)
    dissonance = dissonancesum*norm;
    
    //if(clamp < dissonance)
    //    dissonance = clamp;
    
    //dissonance_ = sc_min(clamp_,dissonancesum*norm_); //numpeaks; //dissonancesum;  //divide by fftsize as compensation for amplitudes via FFT
    
    //ZOUT0(i) = dissonance_;
    //ZOUT0(0) = dissonance_;
    
    return dissonance;
    
}




