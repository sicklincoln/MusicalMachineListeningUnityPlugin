#minimal Cmake file to test dylib
#cmake -DCMAKE_OSX_ARCHITECTURES='i386;x86_64' ..
#cmake -DCMAKE_OSX_ARCHITECTURES='x86_64' ..
#cmake ..

cmake_minimum_required (VERSION 2.6)
project (AudioFeatureExtractorTest)


add_executable(TestAFE main.cpp SoundFile.cpp)

target_link_libraries(TestAFE /usr/local/lib/libsndfile.1.dylib ${CMAKE_SOURCE_DIR}/../Release/libMusicalMachineListeningUnityPlugin.dylib)
