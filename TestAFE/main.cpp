//load dylib and test functionality

#include <iostream>
#include <math.h>

///data/durham/code/library/AudioFeatureExtractor
#include "../AudioFeatureExtractorInterface.h"

//#include "filenames.h"
#include "filenamesartmusic.h"

#include "SoundFile.h"

#define BLOCKSIZE 512
#define POLL 3000 //around once per 30 seconds of analysed audio
#define NUMFEATURES 16
//14 basic features + beat trigger + onset trigger in last slots


//[0,0,0.00097661,0,0,0,22.5081,0,0,0,0,0,0.443182,0.676811,]
//[0.845689,214.75,0.989537,463.233,9.21763,6.98894,95.2131,97,5.9327,2.9083,6.64386,2.79087,1,1,]



int main (int argc, const char * argv[]) {
    
    int numsamples;
    
    int maxlength = 44100*30; //12; //0;
    
    float features[NUMFEATURES];
    float maxima[NUMFEATURES];
    float minima[NUMFEATURES];
    float featurenow;
    
    int i;
    for(int i=0; i< NUMFEATURES; ++i) {
        maxima[i] = 0;
        minima[i] = 99999999.99;
        
    }
  
    //0
    //817
    for (int j=10; j<11; ++j) {
    
    //must pass sample rate and blocksize
    SetupAudioFeatureExtractor(44100.0,BLOCKSIZE);
    //SetupAudioFeatureExtractor(48000.0,BLOCKSIZE);
        
        std::cout << j << " " << strings[j] << std::endl;
        
        //SoundFile sound(strings[j]);
        
        SoundFile sound("/data/durham/teaching/creativemusictechnology/quiz/quiz1.wav");
        //SoundFile sound("/Users/ioi/Desktop/silence.wav");
        
        //SoundFile sound("/data/audio/littleaudio/galv.wav"); //aaahhh
    
    //10 seconds 
   	//SoundFile sound("/data/audio/mirdata/EMcorpus/Björk/Homogenic/Pluto.wav");

    numsamples = sound.numframes;
        
        //analyse at most 2m from start
        if(numsamples>maxlength) numsamples = maxlength;
    
    //float block[BLOCKSIZE];
    
    int position = 0;
        int blockcount = 0;
    
    while (position<(numsamples-BLOCKSIZE)) {
       
        //BLOCKSIZE samples to take every time
        Calculate(sound.data + position,features);
        
        
        //assess max and min
        for(i=0; i< NUMFEATURES; ++i) {
            //
            featurenow = features[i];
        
            if(featurenow<minima[i]) minima[i] = featurenow;
            if(featurenow>maxima[i]) maxima[i] = featurenow;
            
        }
        
       
        if(features[14]>0.5) {std::cout << "onset!" << std::endl;}
        if(features[15]>0.5) {std::cout << "beat!" << std::endl;}
        
        
        if(blockcount%POLL==0) {
        
        std::cout << position;
//        
        for(int i=0; i< NUMFEATURES; ++i) {
            
            std::cout << " " << features[i];
        }
        
            //convert to MIDI note numbers
            
            //(((log2( features[6]/440.0)) * 12) + 69);
//           std::cout << " " << (features[6]);  //just pitch //just spectral clarity
        
        std::cout << std::endl;
        
        // << " " << features[0] << " " << features[1] << " " << features[2] <<  " " << features[3] << std::endl;
        
        }
        
        
        position += BLOCKSIZE;
        ++blockcount;
    }
    
    
    //sound.data,numsamples

    
    
    //numsamples = mp.numsamples_;
    
    //float * output = new float[numsamples];
    
    //for(int i=199680; i<200000; ++i) {
        //
        //        std::cout << i << " " << output[i] << std::endl;
        //
        //    }
    

    DeleteAudioFeatureExtractor();
        
    }
    
       std::cout << "[";
    for(i=0; i< NUMFEATURES; ++i)
        std::cout << minima[i]<< ",";
        std::cout << "]" <<std::endl;
    
    std::cout << "[";
    for(i=0; i< NUMFEATURES; ++i)
        std::cout << maxima[i]<< ",";
    std::cout << "]" <<std::endl;

    return 0;
}







//    //output to audio file
//   	SNDFILE *file;
//	SF_INFO info;
//	//FILE *ofp;
//	info.format = SF_FORMAT_WAV |  SF_FORMAT_PCM_16; 
//	info.samplerate = 44100; 
//	info.channels = 1; //mono for now 
//
//	if(!(file = sf_open("/Users/nickcollins/Desktop/dbmoutput.wav",SFM_WRITE,&info))){ 
//        
//        int errnum = sf_error(file);
//		const char*	errorstring = sf_error_number(errnum);	
//		
//        printf("error opening output file %s \n", errorstring);     exit(-1);
//        
//	}  
//
//     //sf_count_t	sf_writef_float	(SNDFILE *sndfile, const float *ptr, sf_count_t frames) ;
//    sf_writef_float(file,output,numsamples); 
//    //sf_writef_float(file,mp.residual_,numsamples);
//    sf_close(file);
