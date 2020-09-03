# MusicalMachineListeningUnityPlugin


This is a C++ code library intended for use in [Unity](https://unity.com) as a plug-in. Example client code is provided for loading the code either as a pre-compiled dynamic link library or via dropping in the code source in a folder for compilation via [IL2CPP](https://docs.unity3d.com/Manual/IL2CPP.html). The latter is the easier route for situations like building for Android and Oculus Quest, for instance. 


## License

The code was developed by [Nick Collins](http://composerprogrammer.com/index.html) and is released under the quite permissive BSD-3-Clause license; it depends on KissFFT which uses the same license.  In the test project TestAFE (test audio feature extractor), not required for use in Unity, the libsndfile library is used as a dynamically loaded library for audio file reading. libsndfile is under the LGPL (http://www.mega-nerd.com/libsndfile/).
