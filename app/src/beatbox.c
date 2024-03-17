#include <stdio.h> 
#include <pthread.h>
#include "hal/audio_mixer.h"
#include "hal/timer.h"
#include "hal/beats.h"
#include "hal/joystick.h"
#include "hal/accelerometer.h"
#include "text_display.h"
#include "udpListener.h"
#include "periodTimer.h"

int main(){
    // load in wav files that we want to play 
    Period_init();
    initializeSounds(); 
    AudioMixer_init(); 
    accelerometer_init(); 
    
    pthread_t tid = UDP_init();
    // standardRockBeat();
    joystick_init();
    
    // Txt_init();
    pthread_join(tid, NULL);
    

    // Txt_cleanup();
    joystick_cleanup();
    accelerometer_cleanup();
    AudioMixer_cleanup();
    Period_cleanup();
    freeSounds(); 
    return 0; 
}