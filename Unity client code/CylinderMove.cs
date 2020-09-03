using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CylinderMove : MonoBehaviour
{

    private AudioFeatures af;
    public GameObject cy;

    private Renderer r;
    private Material m; 
    // Start is called before the first frame update
    void Start()
    {
        af = GameObject.Find("Audio Source").GetComponent<AudioFeatures>();
        cy = GameObject.Find("Cylinder"); //_00000

        //m = cy.GetComponent<Renderer>().sharedMaterial; 

        //.SetFloat("_YourParameter", someValue);

    }

    // Update is called once per frame
    void Update()
    {
        //transform.localPosition = new Vector3(af.power * 4 - 1, af.irregularity * 4 - 1, (3 * af.keyclarity) - 5);
        //transform.localEulerAngles = new Vector3(af.beathistogramentropy * 40, af.sensorydissonance * 50, af.qitch * 80);
        //transform.eulerAngles = new Vector3(af.beathistogramentropy * 40, af.sensorydissonance * 50, af.qitch * 80);

        //transform.eulerAngles = new Vector3(10, yRotation, 0);
        //transform.position = new Vector3(af.power, af.irregularity * 10 - 5, (30 * af.keyclarity) - 15);

        //m.SetFloat("_Amount", af.power * 1000);
        //m.SetFloat("_Glossiness", af.irregularity);
        //m.SetFloat("_Metallic", af.sensorydissonance * 100);
    }
}
