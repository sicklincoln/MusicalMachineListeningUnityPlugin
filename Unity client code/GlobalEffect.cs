using UnityEngine;
using UnityEngine.Rendering.PostProcessing;

public class GlobalEffect : MonoBehaviour
{
    PostProcessVolume m_Volume;
//    Vignette m_Vignette;
    Bloom m_Vignette;

    private AudioFeatures af;

    void Start()
    {

        af = GameObject.Find("Audio Source").GetComponent<AudioFeatures>();


        m_Vignette = ScriptableObject.CreateInstance<Bloom>();
        m_Vignette.enabled.Override(true);
        m_Vignette.intensity.Override(1f);

        m_Volume = PostProcessManager.instance.QuickVolume(gameObject.layer, 100f, m_Vignette);
    }

    void Update()
    {
        m_Vignette.intensity.value = af.power * 1000;  //Mathf.Sin(Time.realtimeSinceStartup);
    }

    void OnDestroy()
    {
        RuntimeUtilities.DestroyVolume(m_Volume, true, true);
    }
}