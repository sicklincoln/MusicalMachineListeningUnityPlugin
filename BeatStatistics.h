//(c) Nick Collins 2019 all rights reserved

#pragma once

#include "MMLLSTFT.h"

//Nick Collins 16/9/2014
//Beat Statistics; using first stages of a beat tracker. Form beat histogram, then take (assumed tempo independent) statistics: entropy, ratio of first to second peak, harmonicity (how much explained by multiple positions relating to main peak)
//44.1KHz only, precalculated ERB arrays




class BeatStatistics {
    
    //FFT data
	 int numERBbands_;
    float * ERBpowers_;
    float * previousERBpowers_;
    
	//coping with different sampling rates
	float m_srate;		//use as a flag to check sample rate is correct
	float m_frameperiod;
    
    int numperiods_;
	int numpreviousvalues_;
	int storepos_;
	float * store_;  //for previous values
	float * crosscomby_;
    
    //statistics output
    float entropy_;
    float toptworatio_;
    float diversity_;
    float metricity_;
    
    MMLLSTFT * stft;
    
    //phase tracking and beat triggers
    int prevbestperiod_;
    int prevbestphase_;
    float period_;
    int periodi_;
    float phase_;
    float phasechange_;
    float lastphaseestimate_;
    float lastperiodestimate_;
    int periodinsamples_;
    int phasenowinsamples_;
    int trigger_;
    int halftrigdone_, quartertrigdone_, threequarterstrigdone_;
    
    int calcperiod_;
    int calccounter_;
    
public:
    BeatStatistics(float sr, MMLLSTFT * stft1024):stft(stft1024) {
 
        int i;
        
        
        m_srate = sr;
        
        //if sample rate is 88200 or 96000, assume taking double size FFT to start with
        if(m_srate > (44100.0*1.5)) m_srate = m_srate*0.5;
        
        if(((int)(m_srate+0.01))==44100)
        {
            //m_weights = g_weights44100;
            //m_bins = g_bins44100;
            m_frameperiod = 0.046439909297052;
        }
        else  //else 48000; potentially dangerous if it isn't! Fortunately, shouldn't write any data to unknown memory
        {
            //m_weights = g_weights48000;
            //m_bins = g_bins48000;
            m_frameperiod = 0.042666666666667;
        }

        numERBbands_ = 40;
        
        ERBpowers_ = new float[numERBbands_]; // (float*)RTAlloc(mWorld, numERBbands_ * sizeof(float));
        previousERBpowers_ = new float[numERBbands_]; //(float*)RTAlloc(mWorld, numERBbands_ * sizeof(float));
        
        //start zeroed out
        for(i=0; i<numERBbands_; ++i) {
            
            ERBpowers_[i] = 0.0f;
            previousERBpowers_[i] = 0.0f;
    
        }
 
        numperiods_ = 100;
        numpreviousvalues_ = 350;
        storepos_ = 0;
        store_ = new float[numpreviousvalues_]; //(float*)RTAlloc(mWorld, numpreviousvalues_ * sizeof(float)); //for previous values
        crosscomby_ = new float[numperiods_]; //(float*)RTAlloc(mWorld, numperiods_ * sizeof(float));
        
        
        
        for(i=0; i<numpreviousvalues_; ++i) store_[i] = 0.0f;
        
        for(i=0; i<numperiods_; ++i) crosscomby_[i] = 0.0f;
        
        //for(int i=0; i<numperiods_; ++i)
        //    printf("initial cross i %d at %f\n",i,crosscomby_[i]);
        
        
        entropy_ = 0.f;
        toptworatio_ = 0.f;
        diversity_ = 0.f;
        metricity_ = 0.f;
        
        
        calcperiod_ = 86; //ie around 2 seconds at 512 hop
        calccounter_ = 0;
        
        trigger_ = 0;
        
        prevbestperiod_ = 50;
        prevbestphase_ = 0;
        period_= 50.0;
        periodi_ = -1;
        phase_ = 0.0;
        phasechange_ = 0.0;
        periodinsamples_ = 512* period_; //assumes 512 hop size
        phasenowinsamples_=0;
        lastphaseestimate_=0;
        lastperiodestimate_= 50.0;
        
        
    }
    
    ~BeatStatistics() {
        
        delete [] ERBpowers_;
        delete [] previousERBpowers_;
        delete [] store_;
        delete [] crosscomby_;
        
    }
    
    int Calculate(float * featureoutput);
    
    
};
