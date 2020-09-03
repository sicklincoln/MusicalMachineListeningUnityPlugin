//(c) Nick Collins 2019 all rights reserved

#pragma once

class MMLLwindowing {
public:
    int windowsize,hopsize,overlap,storepointer;
    float * store;
    
    
public:
    MMLLwindowing(int windowsize=1024,int hopsize=512): windowsize(windowsize), hopsize(hopsize) {
        
        if(hopsize>windowsize) hopsize = windowsize;
        
        overlap = windowsize - hopsize;
        
        store = new float[windowsize];
        
        //only zero old data
        for (int ii=0; ii<overlap; ++ii)
            store[ii] = 0;
        
        storepointer = overlap;
        

        
    }
    
    ~MMLLwindowing() {
        
        delete [] store;
        
    }
    
    int next(float * input,int n);
    
    
};