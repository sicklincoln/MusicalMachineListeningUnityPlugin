using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;
using UnityEngine.UI;

public class AudioFeatures : MonoBehaviour
{

    //talk to dynamic link library (dll)
    //used to be AudioFeatureExtractor

    #if UNITY_EDITOR
        [DllImport("MusicalMachineListeningUnityPlugin", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SetupAudioFeatureExtractor")]
        private static extern int SetupAudioFeatureExtractor(float sampleRate, int blocksize);
        //if use EntryPoint can have a different name for C# function wrapping the call to the dll
        [DllImport("MusicalMachineListeningUnityPlugin", CallingConvention = CallingConvention.Cdecl, EntryPoint = "Calculate")]
        private unsafe static extern void Calculate(float* data, float* features);
        [DllImport("MusicalMachineListeningUnityPlugin", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SetupAudioFeatureExtractorL")]
        private static extern int SetupAudioFeatureExtractorL(float sampleRate, int blocksize);
        //if use EntryPoint can have a different name for C# function wrapping the call to the dll
        [DllImport("MusicalMachineListeningUnityPlugin", CallingConvention = CallingConvention.Cdecl, EntryPoint = "CalculateL")]
        private unsafe static extern void CalculateL(float* data, float* features);
        [DllImport("MusicalMachineListeningUnityPlugin", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SetupAudioFeatureExtractorR")]
        private static extern int SetupAudioFeatureExtractorR(float sampleRate, int blocksize);
        //if use EntryPoint can have a different name for C# function wrapping the call to the dll
        [DllImport("MusicalMachineListeningUnityPlugin", CallingConvention = CallingConvention.Cdecl, EntryPoint = "CalculateR")]
        private unsafe static extern void CalculateR(float* data, float* features);
    //#endif

#elif UNITY_ANDROID
    [DllImport("__Internal")]
    //[DllImport("__Internal", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SetupAudioFeatureExtractor")]
    private static extern int SetupAudioFeatureExtractor(float sampleRate, int blocksize);
    //if use EntryPoint can have a different name for C# function wrapping the call to the dll
    [DllImport("__Internal")]
    private unsafe static extern void Calculate(float* data, float* features);
    [DllImport("__Internal")]
    private static extern int SetupAudioFeatureExtractorL(float sampleRate, int blocksize);
    //if use EntryPoint can have a different name for C# function wrapping the call to the dll
    [DllImport("__Internal")]
    private unsafe static extern void CalculateL(float* data, float* features);
    [DllImport("__Internal")]
    private static extern int SetupAudioFeatureExtractorR(float sampleRate, int blocksize);
    //if use EntryPoint can have a different name for C# function wrapping the call to the dll
    [DllImport("__Internal")]
    private unsafe static extern void CalculateR(float* data, float* features);
#endif


    AudioSource _audioSource;
    //public float loudness = 0;
    public float centroid = 0;
    public float power = 0;
    public float irregularity = 0;
    public float spectralentropy = 0;
    public float sensorydissonance = 0;
    public float keyclarity = 0; //was rms
    public float qitch = 0; //was zcr
    public float densityofonsets = 0;
    public float meanIOI = 0;
    public float stddevIOI = 0;
    public float beathistogramentropy = 0;
    public float beathistogramfirsttosecondratio = 0;
    public float beathistogramdiversity = 0;
    public float beathistogrammetricity = 0;
    public float onset = 0;
    public float onsetdetected = 0;
    public float beatdetected = 0;

    [Range(0.0f, 1.0f)]
    public float alpha = 0.5f; //to control averaging in recursive IIR low pass filter


    //Good Latency gives me 512 sample blocksize
    int blocksize = 0;
    int sampleRate = 0;

    float[] fftbuffer2; //left channel or mono mix
    int numfeatures = 16;
    float[] rawfeaturedata;
    float[] featuredata;

    bool stereoseparatedextraction = true; 
    float[] fftbufferL;
    float[] rawfeaturedataL;
    float[] featuredataL;
    float[] fftbufferR;
    float[] rawfeaturedataR;
    float[] featuredataR;

    public float centroidL = 0;
    public float powerL = 0;
    public float irregularityL = 0;
    public float spectralentropyL = 0;
    public float sensorydissonanceL = 0;
    public float keyclarityL = 0; //was rms
    public float qitchL = 0; //was zcr
    public float densityofonsetsL = 0;
    public float meanIOIL = 0;
    public float stddevIOIL = 0;
    public float beathistogramentropyL = 0;
    public float beathistogramfirsttosecondratioL = 0;
    public float beathistogramdiversityL = 0;
    public float beathistogrammetricityL = 0;
    public float onsetL = 0;
    public float onsetdetectedL = 0;
    public float beatdetectedL = 0;

    public float centroidR = 0;
    public float powerR = 0;
    public float irregularityR = 0;
    public float spectralentropyR = 0;
    public float sensorydissonanceR = 0;
    public float keyclarityR = 0; //was rms
    public float qitchR = 0; //was zcr
    public float densityofonsetsR = 0;
    public float meanIOIR = 0;
    public float stddevIOIR = 0;
    public float beathistogramentropyR = 0;
    public float beathistogramfirsttosecondratioR = 0;
    public float beathistogramdiversityR = 0;
    public float beathistogrammetricityR = 0;
    public float onsetR = 0;
    public float onsetdetectedR = 0;
    public float beatdetectedR = 0;


    bool awakened = false;
    //int whichaudio = 0;
    //int numaudioclips = 3;
    //AudioClip[] audioclips; 

    //public void OnValueChanged(float newValue)
    //{
    //    alpha = newValue;
    //}


    // Start is called before the first frame update but may be earlier audio callbacks
    void Awake()
    {


        AudioConfiguration config = AudioSettings.GetConfiguration();
        //config.dspBufferSize = 64;
        //AudioSettings.Reset(config);

        blocksize = config.dspBufferSize;
        sampleRate = config.sampleRate;

        int ok = SetupAudioFeatureExtractor(sampleRate, blocksize);
       
        fftbuffer2 = new float[blocksize];

        //numfeatures = 10; 
        rawfeaturedata = new float[numfeatures];
        featuredata = new float[numfeatures];

        for (int i = 0; i < blocksize; ++i)
        {
            fftbuffer2[i] = 0.0f;
        }

        for (int i = 0; i < numfeatures; ++i)
        {
            rawfeaturedata[i] = 0.0f;
            featuredata[i] = 0.0f;
        }

        if(stereoseparatedextraction)
        {
            ok = SetupAudioFeatureExtractorL(sampleRate, blocksize);
            ok = SetupAudioFeatureExtractorR(sampleRate, blocksize);

            fftbufferL = new float[blocksize];
            fftbufferR = new float[blocksize];

            //numfeatures = 10; 
            rawfeaturedataL = new float[numfeatures];
            featuredataL = new float[numfeatures];
            rawfeaturedataR = new float[numfeatures];
            featuredataR = new float[numfeatures];

            for (int i = 0; i < blocksize; ++i)
            {
                fftbufferL[i] = 0.0f;
                fftbufferR[i] = 0.0f;
            }

            for (int i = 0; i < numfeatures; ++i)
            {
                rawfeaturedataL[i] = 0.0f;
                featuredataL[i] = 0.0f;
                rawfeaturedataR[i] = 0.0f;
                featuredataR[i] = 0.0f;
            }

        }



        _audioSource = GetComponent<AudioSource>();

        awakened = true;

    }

    // Update is called once per frame
    void Update()
    {

        if (Input.GetKeyDown("space"))
        {
            print("space key was pressed");

            //_audioSource.clip()


        }

    }





    public unsafe void getFillArrayNative(float[] sampledata, float[] featurevalues)
    {
        //Pin Memory
        fixed (float* p = sampledata)
        {
            fixed (float* q = featurevalues)
            {
                Calculate(p, q);

            }
        }
    }


    public unsafe void getFillArrayNativeL(float[] sampledata, float[] featurevalues)
    {
        //Pin Memory
        fixed (float* p = sampledata)
        {
            fixed (float* q = featurevalues)
            {
                CalculateL(p, q);

            }
        }
    }


    public unsafe void getFillArrayNativeR(float[] sampledata, float[] featurevalues)
    {
        //Pin Memory
        fixed (float* p = sampledata)
        {
            fixed (float* q = featurevalues)
            {
                CalculateR(p, q);

            }
        }
    }



    void OnAudioFilterRead(float[] data, int channels)
    {

        if (awakened)
        {
            int i;

            //double samplenow = AudioSettings.dspTime * sampleRate;
            int dataLen = data.Length / channels;


            for (i = 0; i < dataLen; ++i)
            {
                int baseindex = i * channels;

                float sum = 0.0f;

                for (int j = 0; j < channels; ++j)
                {
                    //data[baseindex + j] += 0.0f;
                    sum += data[baseindex + j];
                };

                fftbuffer2[i] = sum * 0.5f;
            }


            //fftsize
            getFillArrayNative(fftbuffer2, rawfeaturedata);


            //IIR low pass

            for (i = 0; i < 15; ++i)
            { 
            featuredata[i] = (alpha * rawfeaturedata[i]) + ((1 - alpha) * featuredata[i]); 

                //safety
            if(Single.IsNaN(featuredata[i]))
                {

                    featuredata[i] = 0; 
                }

            }


                //

            centroid = featuredata[0];
            power = featuredata[1];
            irregularity = featuredata[2];
            spectralentropy = featuredata[3];
            sensorydissonance = featuredata[4];
            // rms = featuredata[5];
            // zcr = featuredata[6];
            keyclarity = featuredata[5];
            qitch = featuredata[6];

            densityofonsets = featuredata[7];
            meanIOI = featuredata[8];
            stddevIOI = featuredata[9];

            beathistogramentropy = featuredata[10];
            beathistogramfirsttosecondratio = featuredata[11];
            beathistogramdiversity = featuredata[12];
            beathistogrammetricity = featuredata[13];

            onset = featuredata[14];

            onsetdetected = rawfeaturedata[14];


            beatdetected = rawfeaturedata[15];

            //Debug.Log("external centroid = " + centroid);


            if (stereoseparatedextraction)
            {


                for (i = 0; i < dataLen; ++i)
                {
                    //assumes channels = 2
                    int baseindex = i * channels;

                    fftbufferL[i] = data[baseindex];
                    fftbufferR[i] = data[baseindex+1];

                }


                //fftsize
                getFillArrayNativeL(fftbufferL, rawfeaturedataL);
                getFillArrayNativeR(fftbufferR, rawfeaturedataR);

                //IIR low pass

                for (i = 0; i < 15; ++i)
                {
                    featuredataL[i] = (alpha * rawfeaturedataL[i]) + ((1 - alpha) * featuredataL[i]);
                    featuredataR[i] = (alpha * rawfeaturedataR[i]) + ((1 - alpha) * featuredataR[i]);


                    //safety
                    if (Single.IsNaN(featuredataL[i]))
                    {

                        featuredataL[i] = 0;
                    }

                    if (Single.IsNaN(featuredataR[i]))
                    {

                        featuredataR[i] = 0;
                    }

                }


                //

                centroidL = featuredataL[0];
                powerL = featuredataL[1];
                irregularityL = featuredataL[2];
                spectralentropyL = featuredataL[3];
                sensorydissonanceL = featuredataL[4];
                // rms = featuredata[5];
                // zcr = featuredata[6];
                keyclarityL = featuredataL[5];
                qitchL = featuredataL[6];

                densityofonsetsL = featuredataL[7];
                meanIOIL = featuredataL[8];
                stddevIOIL = featuredataL[9];

                beathistogramentropyL = featuredataL[10];
                beathistogramfirsttosecondratioL = featuredataL[11];
                beathistogramdiversityL = featuredataL[12];
                beathistogrammetricityL = featuredataL[13];

                onsetL = featuredataL[14];

                onsetdetectedL = rawfeaturedataL[14];

                beatdetectedL = rawfeaturedataL[15];


                centroidR = featuredataR[0];
                powerR = featuredataR[1];
                irregularityR = featuredataR[2];
                spectralentropyR = featuredataR[3];
                sensorydissonanceR = featuredataR[4];
                // rms = featuredata[5];
                // zcr = featuredata[6];
                keyclarityR = featuredataR[5];
                qitchR = featuredataR[6];

                densityofonsetsR = featuredataR[7];
                meanIOIR = featuredataR[8];
                stddevIOIR = featuredataR[9];

                beathistogramentropyR = featuredataR[10];
                beathistogramfirsttosecondratioR = featuredataR[11];
                beathistogramdiversityR = featuredataR[12];
                beathistogrammetricityR = featuredataR[13];

                onsetR = featuredataR[14];

                onsetdetectedR = rawfeaturedataR[14];

                beatdetectedR = rawfeaturedataR[15];

            }




            }
    }

}
