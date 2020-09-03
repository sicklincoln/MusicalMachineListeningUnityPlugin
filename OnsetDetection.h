//(c) Nick Collins 2019 all rights reserved

#pragma once

#include "MMLLSTFT.h"


//assumes fixed sampling rate
//FFT data
//#define N 1024
#define NOVER2 512
//#define NOVER4 256
//#define OVERLAP 512
//#define OVERLAPINDEX 512
//#define HOPSIZE 512
//#define FS 44100
//#define FRAMESR 172.2656
//#define FRAMEPERIOD 0.00581
////0.01161 //seconds
///86.1328

#define NUMERBBANDS 40
#define PASTERBBANDS 3
//3 usually, but time resolution improved if made 1?

//in FFT frames
#define MINEVENTDUR 3
//4 or maybe 2

//7 frames is about 40 mS
//peak picker will use 3 back, 3 forward
#define DFFRAMESSTORED 7

//#define MAXBLOCKSIZE 64
//#define MAXBLOCKS 700



class OnsetDetection {
   
    MMLLSTFT * stft;
    
    double	threshold_;
    
    //for hanning window, constructed in plugin initialisation
    //float * hanning_;
    
    float * fftpowers_;
    
   	//time positions
	long m_frame;
	long m_lastdetect;
	
	//loudness measure
	float m_loudbands[NUMERBBANDS][PASTERBBANDS]; //stores previous loudness bands
	int m_pasterbbandcounter;
	float m_df[DFFRAMESSTORED]; //detection function buffer (last 5)
	int m_dfcounter;
    
    int m_onsetdetected;
    
    
public:
    OnsetDetection(float sr, MMLLSTFT * stft1024):stft(stft1024) {
        
        //inputs and state
        threshold_ = 0.5; //0.34;
        
        int i,j;
        
        ////////time positions//////////
        m_frame=0;
        m_lastdetect=-100;
        
        /////////loudness measure////////
        m_dfcounter=DFFRAMESSTORED-1;
        //zero loudness store
        for(j=0;j<DFFRAMESSTORED;++j) {
            m_df[j]=0.0;
        }
        
        //zero previous specific loudness in Bark bands
        for(i=0;i<PASTERBBANDS; ++i)
            for(j=0;j<NUMERBBANDS;++j) {
                m_loudbands[j][i]=0.0;
            }
        
        m_pasterbbandcounter=0;
        
        m_onsetdetected = 0;

    }
    
    ~OnsetDetection() {
 
        // RTFree(mWorld, fftpowers_);
        
    }
    
    int Calculate();
    
    //void dofft();
    void calculatedf();
    void peakpickdf();

    
    
};
