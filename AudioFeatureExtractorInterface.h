//(c) Nick Collins 2019 all rights reserved

#pragma once

#ifdef _WIN32
//required for Windows
#define DLLExport __declspec(dllexport)
#else
#define DLLExport

#endif

extern "C" {

    DLLExport int SetupAudioFeatureExtractor(float sampleRate, int blocksize);
    DLLExport int SetupAudioFeatureExtractorL(float sampleRate, int blocksize);
    DLLExport int SetupAudioFeatureExtractorR(float sampleRate, int blocksize);
    DLLExport int SetupSpectrumExtractor(float sampleRate, int blocksize, int fftsize, int hopsize, int windowtype);
    
    
    DLLExport void Calculate(float *,float *);
    DLLExport void CalculateL(float *,float *);
    DLLExport void CalculateR(float *,float *);
    DLLExport void CalculateSpectrum(float *,float *);
    
    DLLExport void DeleteAudioFeatureExtractor();
    DLLExport void DeleteAudioFeatureExtractorL();
    DLLExport void DeleteAudioFeatureExtractorR();
    DLLExport void DeleteSpectrumExtractor();
    
    
}
