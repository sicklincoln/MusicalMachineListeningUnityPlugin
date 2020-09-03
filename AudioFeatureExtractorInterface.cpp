//(c) Nick Collins 2019 all rights reserved

#include "AudioFeatureExtractorInterface.h"
#include "AudioFeatureExtractor.h"
#include "SpectrumExtractor.h"


#ifdef __cplusplus
extern "C" {
#endif

    
//AT MOST THREE EXTRACTORS, typically one
//to allocate
AudioFeatureExtractor * g_afe;
AudioFeatureExtractor * g_afe2;
AudioFeatureExtractor * g_afe3;
SpectrumExtractor * g_spectrum;


DLLExport int SetupSpectrumExtractor(float sampleRate, int blocksize, int fftsize, int hopsize, int windowtype) {
    
    g_spectrum = new SpectrumExtractor(sampleRate, blocksize, fftsize, hopsize, windowtype);
    
    return 1;
}

DLLExport void DeleteSpectrumExtractor() {
    
    delete g_spectrum;
}


DLLExport void CalculateSpectrum(float* input, float * output) {
    
    g_spectrum->Calculate(input,output);

}



DLLExport int SetupAudioFeatureExtractor(float sampleRate, int blocksize) {

    g_afe = new AudioFeatureExtractor(sampleRate, blocksize);
    
    return 1;
}

DLLExport void DeleteAudioFeatureExtractor() {
    
    delete g_afe;
}


DLLExport void Calculate(float* input, float * outputfeatures) {
 
    
    g_afe->Calculate(input,outputfeatures);
    

    //outputfeatures[0] = g_afe->centroid;
    
}
    
    
DLLExport int SetupAudioFeatureExtractorL(float sampleRate, int blocksize) {
    
    g_afe2 = new AudioFeatureExtractor(sampleRate, blocksize);
    
    return 2;
}

DLLExport void DeleteAudioFeatureExtractorL() {
    
    delete g_afe2;
}


DLLExport void CalculateL(float* input, float * outputfeatures) {
    
    g_afe2->Calculate(input,outputfeatures);

}
    
    
DLLExport int SetupAudioFeatureExtractorR(float sampleRate, int blocksize) {
    
    g_afe3 = new AudioFeatureExtractor(sampleRate, blocksize);
    
    return 3;
}

DLLExport void DeleteAudioFeatureExtractorR() {
    
    delete g_afe3;
}


DLLExport void CalculateR(float* input, float * outputfeatures) {
    
    g_afe3->Calculate(input,outputfeatures);
    
}

#ifdef __cplusplus
}
#endif

