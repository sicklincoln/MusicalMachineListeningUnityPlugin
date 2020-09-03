//(c) Nick Collins 2019 all rights reserved

#pragma once

#include "MMLLSTFT.h"


//http://see-programming.blogspot.co.uk/2013/02/double-ended-queue-dequeue.html
//http://www.dreamincode.net/forums/topic/35344-c-deque-tutorial/
#include <deque>
using namespace std; //::deque;

const int g_maxnumonsets = 200;



class OnsetStatistics {
   
    //MMLLSTFT * stft;
 
    //int maxnumonsets;
    
    int samplecounter_;
    
    float windowsize_;
    float hopsize_;
    int windowsizeinsamples_;
    int hopsizeinsamples_;
    
    float krsampletotime_; //conversion factor to get from kr samples to actual time in seconds
    
    //int * onsettimes_; //relative to last updated master position
    //int onsettimesindex_;
    
    deque<int> * onsettimes_;
    float iois_[g_maxnumonsets];
    
    float previnput_;
    
public:
    OnsetStatistics(float sr) {
        
        //input,windowsize=1024,peakpicksize=10,leak=0.999,energythreshold=0.01,sumthreshold=100,mingap=0
        
        //OUT0(0) = ZIN0(1);
        
        //zero outputs at sample one
        //OUT0(0) = 0.0f;
        //OUT0(1) = 0.0f;
        //OUT0(2) = 0.0f;
        
        previnput_ = 0.f;
        
        krsampletotime_ = 512/sr;
        samplecounter_ = 0;
        //maxnumonsets = 100;
        
        
        //could change dynamically if within next function
        windowsize_ = 1; //ZIN0(1);
        windowsizeinsamples_ = 512; //HARD CODED HACK! //windowsize_/krsampletotime_;
        
        //printf("sample time %f window in samples %d\n",krsampletotime_,windowsizeinsamples_);
        
        //should use placement new but you only live once so not bothering right now; would also require custom allocator for deque to use etc
        //http://en.wikipedia.org/wiki/Placement_syntax#Placement_delete
        
        onsettimes_ = new deque<int>; //(deque<int> *)RTAlloc(mWorld, sizeof(deque<int>))
        
        //printf("sanity check %lu\n",onsettimes_->size());
        
        // onsettimes_ = (int *) RTAlloc(mWorld, sizeof(int)* maxnumonsets);
        //onsettimesindex_ = 0;
        
        int i; 
        
        //    for (i=0; i<maxnumonsets; ++i) {
        //        onsettimes_[i] = -99999999;
        //        
        //    }

        
        
       
    }
    
    ~OnsetStatistics() {
 
        delete onsettimes_;
        
    }
    
    void Calculate(float input, float * featureoutput);
    
    
};
