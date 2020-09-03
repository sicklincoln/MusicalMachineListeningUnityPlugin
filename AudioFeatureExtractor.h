//(c) Nick Collins 2019 all rights reserved

#pragma once

#include "MMLLSTFT.h"
#include "SpectralEntropy.h"
#include "SensoryDissonance.h"
#include "OnsetDetection.h"
#include "OnsetStatistics.h"
#include "BeatStatistics.h"
#include "KeyClarity.h"
#include "QitchDetector.h"

//further feature options and mappings

//tanh(0.25*SensoryDissonance.kr(fft,500,1.0,nil,99999999))
//[SpecPcile,0.9],
//JensenShannon UGen https://en.wikipedia.org/wiki/Jensen%E2%80%93Shannon_divergence
//JensenShannon.kr(FFT(LocalBuf(1024),input,1,wintype:1),86);

#define NUMFEATURES 14

class AudioFeatureExtractor {
   
public:
    float sampleRate;
    int blocksize;
    
    float centroid; //other features to add
    float power;
    float irregularity; 
    float spectralentropy;
    float sensorydissonance;
    float keyclarity;
    float features[NUMFEATURES];
    
    SpectralEntropy * spectralentropyextractor;
    SensoryDissonance * sensorydissonanceextractor;
    OnsetDetection * onsetdetector;
    OnsetStatistics * onsetstats;
    BeatStatistics * beatstats;
    KeyClarity * keyclarityextractor;
    QitchDetector * qitchdetector;
    
    //shared spectral analysis amongst subsidiary feature extractors
    MMLLSTFT * stft1024;
    MMLLSTFT * stft2048;
    MMLLSTFT * stft4096;
    
public:
    AudioFeatureExtractor(float sampleRate, int blocksize):sampleRate(sampleRate),blocksize(blocksize) {
        
        stft1024 = new MMLLSTFT(1024,512,1);//Hann windowing
        stft2048 = new MMLLSTFT(2048,512,1);//Hann windowing
        stft4096 = new MMLLSTFT(4096,512,1);//Hann windowing
        
        spectralentropyextractor = new SpectralEntropy(stft2048);
        sensorydissonanceextractor = new SensoryDissonance(sampleRate,stft2048);
        
        onsetdetector = new OnsetDetection(sampleRate,stft1024);
        onsetstats = new OnsetStatistics(sampleRate);
        
        beatstats = new BeatStatistics(sampleRate,stft1024);
        
        keyclarityextractor = new KeyClarity(sampleRate,stft4096);
        
        qitchdetector = new QitchDetector(sampleRate,4096); //512
        
        power = 0;
        centroid = 0;
        irregularity = 0;
        spectralentropy = 0;
        sensorydissonance = 0;
        keyclarity = 0;
        
        for (int i=0; i< NUMFEATURES; ++i) {
            
            features[i] = 0;
        }
        
    }
    
    ~AudioFeatureExtractor() {
        
        delete spectralentropyextractor; 
        delete sensorydissonanceextractor;
        delete onsetdetector;
        delete onsetstats;
        delete beatstats; 
        delete keyclarityextractor;
        delete qitchdetector;
        
        delete stft1024;
        delete stft2048;
        delete stft4096;
        
    }
    
    void Calculate(float* input, float * outputfeatures); 
    
    
};
