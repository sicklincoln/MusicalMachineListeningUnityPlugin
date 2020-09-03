using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PrefabFireLR : MonoBehaviour
{
    private AudioFeatures af;
    public GameObject prefab;
    public float speed = 4;

    private GameObject[] cubes;
    private Rigidbody[] body;
    private int numcubes;
    private int cubeindex;

    private Vector3 centre;

    private int onsetdetected = 0;
    private int beatdetected = 0; 

    void Start()
    {
        numcubes = 100;
        cubeindex = 0;

        af = GameObject.Find("Audio Source").GetComponent<AudioFeatures>();

        //https://answers.unity.com/questions/847848/refer-to-a-parent-rigidbody.html
        //daVodaParent.rigidbody;
        cubes = new GameObject[numcubes];
        body = new Rigidbody[numcubes];

        centre = new Vector3(0,7,-1);
    }


    void Update()
    {
        if (af.onsetdetectedL > 0.5f) onsetdetected = 1;
        if ((af.beatdetectedR > 0.5f) && (af.beatdetectedR < 1.5f)) beatdetected = 1; 


    }

    void FixedUpdate()
    {
        //if (Input.GetButtonDown("Fire1"))
        //{
        //if ((af.beatdetected > 0.5f) && (af.beatdetected < 1.5f))
        //if(af.onsetdetected > 0.5f)

            if (onsetdetected ==1) 
        {
                onsetdetected = 0; 

            for (int k = 0; k < 3; ++k)
            {

                if (!(cubes[cubeindex] == null))
                {
                    Destroy(cubes[cubeindex]);  //.transform.parent.gameObject

                }
                //transform.position + 
                GameObject p = Instantiate(prefab, new Vector3(5*k-5, Random.Range(1, 10.1f) + 2, -2 - k), transform.rotation);
                Rigidbody r = p.GetComponent<Rigidbody>();

                //r.velocity = new Vector3(af.qitch * 0.3f - 0.15f, 5.0f + (5.0f * af.sensorydissonance), 10.0f) * af.power * 100;  //transform.forward

                r.velocity = new Vector3(Random.Range(-0.1f, 0.1f), 1, Random.Range(-0.1f, 0.1f)) * af.powerL * 100;  //transform.forward

                cubes[cubeindex] = p;
                body[cubeindex] = r;

                cubeindex = (cubeindex + 1) % numcubes;

            }

        }

            if(beatdetected == 1) {
            //if ((af.beatdetected > 0.5f) && (af.beatdetected < 1.5f)) {

            beatdetected = 0; 

        for (int k = 0; k < numcubes; ++k)
            {

                if (!(cubes[k] == null))
                {

                    Vector3 posnow = cubes[k].transform.position;
                    float ypos = posnow.y;

                    Vector3 difference = centre - posnow;

                    //body[k].AddForce(Vector3.Cross(centre,posnow).normalized*0.3f); //ForceMode.Impulse

                    //body[k].AddForce((centre - posnow) * 0.125f * (1 + (1000*af.power))); //ForceMode.Impulse
                    //too much but great fun
                    //body[k].AddForce((centre - posnow) * 10f * (1 + (1000 * af.power)), ForceMode.Impulse); //ForceMode.Impulse

                    body[k].AddForce((centre - posnow) * 1.5f * (1 + (1000 * af.powerR)), ForceMode.Impulse); //ForceMode.Impulse


                    //https://docs.unity3d.com/ScriptReference/Rigidbody.MovePosition.html
                    //rb.MovePosition(transform.position + transform.forward * Time.deltaTime);
                    //body[k].MovePosition(posnow + (15 * difference * Time.deltaTime));  //adjust by duration over which to interpolate positions



                    //Vector3.Distance(other.position, transform.position);

                    //if position in y less than 1, apply force to push back up

                    if (ypos < (-1))
                    {
                        float excess = (-1 - ypos); //-1 - 
                                                    //Mathf.Abs(
                        body[k].AddForce(af.sensorydissonanceL * 0.1f - 0.01f, af.powerR * 5.0f * excess * excess, af.centroidL * -0.4f, ForceMode.Impulse);

                    }


                }

            }
        }
               

    }


}