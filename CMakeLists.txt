#minimal Cmake file for shared library building (dylib)
#cmake -DCMAKE_OSX_ARCHITECTURES='i386;x86_64' ..
#cmake -DCMAKE_OSX_ARCHITECTURES='x86_64' ..
#cmake ..

cmake_minimum_required (VERSION 2.6)
project (MusicalMachineListeningUnityPlugin)

add_library(MusicalMachineListeningUnityPlugin SHARED kiss_fft.c kiss_fftr.c SpectrumExtractor.cpp AudioFeatureExtractor.cpp AudioFeatureExtractorInterface.cpp MMLLwindowing.cpp MMLLSTFT.cpp SpectralEntropy.cpp SensoryDissonance.cpp OnsetStatistics.cpp OnsetDetection.cpp BeatStatistics.cpp KeyClarity.cpp QitchDetector.cpp)

#${SOURCES} ${HEADERS}
#for bundle use MODULE, for dylib use SHARED

#add_library(MusicalMachineListeningUnityPlugin SHARED kiss_fft.c kiss_fftr.c SpectrumExtractor.cpp AudioFeatureExtractor.cpp AudioFeatureExtractorInterface.cpp MMLLwindowing.cpp MMLLSTFT.cpp SpectralEntropy.cpp SensoryDissonance.cpp OnsetStatistics.cpp OnsetDetection.cpp BeatStatistics.cpp KeyClarity.cpp QitchDetector.cpp)

#set_target_properties(MusicalMachineListeningUnityPlugin PROPERTIES BUNDLE TRUE)
