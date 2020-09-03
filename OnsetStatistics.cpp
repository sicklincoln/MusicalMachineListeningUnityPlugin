//(c) Nick Collins 2019 all rights reserved

//two modes? last N events, or events in last N seconds

#include "OnsetStatistics.h"
//#include <math.h>
//#include <stdlib.h>


void OnsetStatistics::Calculate(float input, float * featureoutput) {
	
	int i; //,j;
    
 	//float input = IN0(0); //(0)[0];
    float prev = previnput_;
    
    int sampletimenow = samplecounter_;
    
    int earliestallowed = sampletimenow - windowsizeinsamples_ + 1;
    //if(earliestallowed<0)
    
    //printf("input %f earliestallowed %d sampletimenow %d\n",input, earliestallowed, sampletimenow);
    
    deque<int>& onsettimes = *onsettimes_;
    
    bool stillgoing = true;
    //http://www.dreamincode.net/forums/topic/35344-c-deque-tutorial/
    //pop_front, pop_back, push_front, push_back
    
    //printf("remove old %lu\n",onsettimes.size()); //,onsettimes[0]);
    //remove any too old
    while(onsettimes.size()>0 && stillgoing) {
        
        if ((onsettimes[0]<earliestallowed) || (onsettimes.size()>g_maxnumonsets))
            onsettimes.pop_front();
        else
            stillgoing = false;
    
    }
    
    //for (i=0; i<numSamples; ++i) {
    //}
 
    
    //printf("trigger?\n");
    
    //trigger
    if(prev<=0.f && input>0.f) {
        
        onsettimes.push_back(sampletimenow); //push_back is most recent, highest index
    }
    
    //printf("calculate statistics \n");
    
    //calculate statistics
    float ioi;
    float mean = 0.f;
    float conversion = krsampletotime_;
    
    float * iois = iois_;
    
    deque<int>::iterator itr;
    
    i = 0;
    
    for (itr=onsettimes.begin(); itr!=onsettimes.end(); ++itr) {
    
        iois[i] = *itr;
        
        //ioi = (*itr - sampletimenow) * conversion;
        
        //mean += ioi;
        
        //iois[i] = ioi;
        
        ++i;
    }
    
    
    int numiois = onsettimes.size()-1; //was -1
    if (numiois<0) numiois = 0;
    
    //printf("numiois %d\n",numiois);
    
    for (i=0; i<numiois; ++i) {
        iois[i] = (iois[i+1] - iois[i])*conversion;
        mean += iois[i];
    }
    
    if(numiois>=1)
    mean /= numiois;
    //else
    //mean = -1.0f;
    
    //printf("mean %f\n",mean);
    
    float variance = 0.f;
    float summand;
    
    
    for (i=0; i<numiois; ++i) {
        
        summand = iois[i]-mean;
        
        variance += summand*summand;
    
    }
    
    if(numiois>=1)
        variance /= numiois;
    //else
    //    variance = 0.0f;
    

    //printf("variance %f\n",variance);
    
    
    featureoutput[0] = (float)numiois;
    featureoutput[1] = mean;
    featureoutput[2] = sqrt(variance); //return standard deviation
    //OUT0(3) =

    
    previnput_ = input;
    
    ++sampletimenow;
    
    //correct all times to avoid spillage
    if(sampletimenow>10000) {
        
        for (int i=0; i<onsettimes.size(); ++i)
            onsettimes[i] -= sampletimenow;
        
        sampletimenow = 0;
        
    }
    
    samplecounter_ = sampletimenow;
    
 }


