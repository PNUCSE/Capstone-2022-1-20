using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Navigation : Singleton<Navigation>
{
    [SerializeField]
    AudioClip[] audioClips;

    [SerializeField]
    AudioSource audioSource;

    float timer;
    bool isTimerOn;

    private void Start()
    {
        timer = 0f;
        isTimerOn = false;
    }

    private void Update()
    {
        if(isTimerOn == true)
        {
            timer += Time.deltaTime;

            if (timer >= 1.0f)
                isTimerOn = false;
        }
    }

    public void Rfid_Reader(long message)
    {
        if (isTimerOn == true)
            return;

        switch (message)
        {
            case 2541813042:
                EnteringCpuSchool();
                break;
            case 9455219144:
                NavRight();
                break;
            case 126125221144:
                NavEnd();
                break;
        }

        timer = 0f;
        isTimerOn = true;
    }

    private void EnteringCpuSchool()
    {
        audioSource.clip = audioClips[0];
        audioSource.Play();
        Invoke("NavStartToilet", 4.8f);
    }

    private void NavStartToilet()
    {
        audioSource.clip = audioClips[1];
        audioSource.Play();
        Invoke("NavStraight", 5.0f);
    }

    private void NavStraight()
    {
        audioSource.clip = audioClips[2];
        audioSource.Play();
    }

    private void NavRight()
    {
        audioSource.clip = audioClips[3];
        audioSource.Play();
    }

    private void NavEnd()
    {
        audioSource.clip = audioClips[4];
        audioSource.Play();
    }

}
