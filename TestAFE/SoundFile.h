/*
 *  Sound.h
 *  AudioFeatures
 *
 *  Created by Nick Collins on 01/08/2007.
 *  Copyright 2007 Nick Collins. All rights reserved.
 *
 */

#pragma once

#include "/data/gitprojects/SuperCollider/external_libraries/libsndfile/sndfile.h"




#include <string>
//#include "Signal.h"

using std::string;

//mono or stereo only; creates final mono data
class SoundFile {
	
public:
	string filename;
	//char * filename;
	
	float * data; //single channel; will be copy of data pointer if numchannels 1, else mixdown of 2 if 2
	sf_count_t numframes; //number of sample frames
	int numchannels;
	int sr;
	
	SoundFile(const char * which, int readorwrite=0, float * output=0, int num=0);
	~SoundFile();
    
//private:
	SNDFILE *file;
	SF_INFO info;
	FILE *ofp;
	//float * source; //interleaved source data
	
};


