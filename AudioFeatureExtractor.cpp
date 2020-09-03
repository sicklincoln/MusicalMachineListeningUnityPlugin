//(c) Nick Collins 2019 all rights reserved


#include "AudioFeatureExtractor.h"

float minima[] = {0,0,0.00097661,0,0,0,22.5081,0,0,0,0,0,0.443182,0.676811};
//float maxima[] = {0.845689,214.75,0.989537,463.233,9.21763,6.98894,95.2131,97,5.9327,2.9083,6.64386,2.79087,1,1};
//float maxmindiff[] = {0.845689, 214.75, 0.98856039, 463.233, 9.21763, 6.98894, 72.705, 97, 5.9327, 2.9083, 6.64386, 2.79087, 0.556818, 0.323189};
float maxmindiffr[] = {1.1824677866213, 0.0046565774155995, 1.0115719890416, 0.0021587408496372, 0.10848775661423, 0.14308321433579, 0.013754212227495, 0.010309278350515, 0.16855731791596, 0.34384348244679, 0.15051491151228, 0.35831120761626, 1.7959189537695, 3.0941647147644};




void AudioFeatureExtractor::Calculate(float* input, float * outputfeatures) {
    
    int ready = stft1024->next(input,blocksize);
    int ready2 = stft2048->next(input,blocksize);
    int ready4 = stft4096->next(input,blocksize);
    
    int onset = 0;
    int beat = 0;
  
    if(ready) {

        int i;
        
        float* powers = stft1024->powers;
    
        float sum1 = 0.0f;
        float sum2 = 0.0f;
        float powernow = 0.f;
        
        irregularity = 0.0f;
        float lastfftbinpower = 0.0f;
      
        int halffftsize = stft1024->halffftsize;
        
        //misses off DC
        for (i =0; i< halffftsize; ++i) {
       
            powernow = powers[i];
            
            sum1 += powernow;
            sum2 += powernow * i;
            
            irregularity += fabs(powernow-lastfftbinpower);
            lastfftbinpower = powernow;
            
            
        }
        
        irregularity /= 2.0*(sum1+0.01); //avoid potential divide by zero
        
        centroid = (float)(sum2 / (sum1 * halffftsize + 1));
        power = sum1/halffftsize;
        
        onset = onsetdetector->Calculate();
        onsetstats->Calculate(onset,features+7);
        
        beat = beatstats->Calculate(features+10);
        
        
    }
    
    if(ready2) {
        
        spectralentropy = spectralentropyextractor->Calculate();
        sensorydissonance = sensorydissonanceextractor->Calculate();
        
    }
    
    if(ready4) {
        
        keyclarity = keyclarityextractor->Calculate();
        
    }
    
    //time domain features
    //RMS
//    float sum = 0.0;
//	
//	for (int i=0; i<blocksize; ++i) {
//		
//        float latest = input[i];
//		
//		sum += latest*latest;
//	}
//	
//	features[5] = sqrt(sum/blocksize);
//    
    
    //ZCR
//    int sum2 = 0;
//	
//    float prev = input[0];
//    
//	for (int i=1; i<blocksize; ++i) {
//		
//        float latest = input[i];
//        
//        //float diff = latest-prev;
//        
//        
//        if((prev<0) && (latest>0))
//            ++sum2;
//      
//        
//        prev = latest;
//  	}
//	
//	features[6] = (float)sum2*2.f/blocksize; //sqrt(sum/blocksize);

    qitchdetector->Calculate(input,blocksize);
    
    features[6] = qitchdetector->m_midipitch; //currfreq;
    
    //printf("qcurr %f qmidi %f\n",qitchdetector->m_currfreq,qitchdetector->m_midipitch); //currfreq;
    
    for (int i=0; i< NUMFEATURES; ++i) {
        
        outputfeatures[i] = features[i];
    }
    
    
    
    
    //latest value, either just calculated or last cached
    outputfeatures[0] = centroid;
    outputfeatures[1] = power;
    outputfeatures[2] = irregularity;
    
    outputfeatures[3] = spectralentropy;
    outputfeatures[4] = sensorydissonance;
    outputfeatures[5] = keyclarity;
    
    //outputfeatures[5] = qitchdetector->m_currfreq;
    
    
    
    //MIN-MAX NORMALISE
    
    
    for (int i=0; i< NUMFEATURES; ++i) {
        
        outputfeatures[i] = (outputfeatures[i] - minima[i]) * maxmindiffr[i];
    }
    
    //printf("problem? %f\n",outputfeatures[6] ); //currfreq;
    
    outputfeatures[NUMFEATURES] = onset;
    
    outputfeatures[NUMFEATURES+1] = beat;
    
    
}
