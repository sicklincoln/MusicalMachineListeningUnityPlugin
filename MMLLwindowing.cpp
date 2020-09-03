//(c) Nick Collins 2019 all rights reserved

#include "MMLLwindowing.h"


int MMLLwindowing::next(float * input,int n) {
    
    int i;
    int result = 0;
    
    //if just output a window of data
    //copy and update storepointer position
    if(storepointer>=windowsize) {
        
        for (i=0; i<overlap; ++i)
            store[i] = store[hopsize+i];
        
        storepointer = overlap;
        
    }
    
    if((storepointer+n)>=windowsize) {
        n = windowsize - storepointer;
        //just in case doesn't fit exactly, don't bother if really going to wrap around since unresolvable issue if  overwrite buffer or multiple wraps in one go anyway
        
        result = 1;
        
    }
    
    for (i=0; i<n; ++i) {
        store[storepointer+i] = input[i];
        
    }
    
    
    storepointer = (storepointer + n); //%(windowsize);
    
    return result;
    
}