/*
 *  Sound.cpp
 *  AudioFeatures
 *
 *  Created by Nick Collins on 01/08/2007.
 *  Copyright 2007 Nick Collins. All rights reserved.
 *
 */

#include "SoundFile.h"

#include <iostream>

//
//SoundFile
SoundFile::SoundFile (const char * which, int readorwrite, float * output, int num) : filename(which) {
	
    if (readorwrite==0) {
    
	float * source;
	
	info.format=0;
	
	//filename = which; //copy string? 
	//printf("input %s output %s \n",argv[1],argv[2]);
	
	printf("%s \n", which);
	
	//try + catch?    exit(-1); 
	if(!(file = sf_open(which,SFM_READ,&info))){     printf("error opening input file\n");     exit(-1);}  
	
	numframes=info.frames;
	numchannels=info.channels;
	sr=info.samplerate;
	
	printf("numframes %d numchan %d sr %d \n",(int)numframes, numchannels,sr);
	
	//try + catch? 
	if (sr!=44100) {printf("wrong sample rate %d not 44100",sr);  exit(-1);}
	if (numchannels>2) {printf("wrong num channels %d > 2",numchannels);  exit(-1);}
	
	//Need to use either just mono channel or load both and mix *0.5, or just load left? 
	//could also run on both left and right and make a bigger feature vector? 
	
	//libsndfile - items is number of floats say, interleaved channels, use readf for frame locked fetch
	//zero padding will be dealt with later by CalculateFFT class as necessary
	
	source= new float[numchannels*numframes];
	
	sf_readf_float(file, source, numframes);      
	
	if(numchannels==1) {data = source; source=NULL;}
	else
	{
		data= new float[numframes];
		
		for (int i=0; i<numframes; ++i)
			data[i]= 0.5*(source[2*i]+ source[2*i+1]);
		
		numchannels=1; //now use as mono source
	}
	//use new? 
	//float * data= (float *)malloc(numframes * sizeof(float));
	//sf_readf_float(file, data, numframes);
	
	//http://music.columbia.edu/pipermail/linux-audio-dev/2004-February/006611.html
	
	//don't use
	//////sf_read_float(file, data, numframes);
	
	delete []source;
        
    } else {
        
        //output to audio file
        SNDFILE *file;
        SF_INFO info;
        //FILE *ofp;
        info.format = SF_FORMAT_WAV |  SF_FORMAT_PCM_16; 
        info.samplerate = 44100; 
        info.channels = 1; //mono for now 
        
        if(!(file = sf_open(which,SFM_WRITE,&info))){ 
            
            int errnum = sf_error(file);
            const char*	errorstring = sf_error_number(errnum);	
            
            printf("error opening output file %s \n", errorstring);   
            exit(-1);
            
        }  
        
        //sf_count_t	sf_writef_float	(SNDFILE *sndfile, const float *ptr, sf_count_t frames) ;
        sf_writef_float(file,output,num); 

        
    }
	
}



SoundFile::~SoundFile() {
	
	//use delete? 
	//if(numchannels==2)
		//delete []data;
	//else
		//delete []source;
	//delete both, safe since source was set to NULL for mono
	
    delete [] data; 
    
	sf_close(file);
	
}

