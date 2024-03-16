#include <stdio.h> 
#include "hal/audio_mixer.h"
#include "hal/timer.h"
#include "hal/beats.h"
#include "hal/joystick.h"
#include "hal/accelerometer.h"

int main(){
    // load in wav files that we want to play 
    initializeSounds(); 
    AudioMixer_init(); 
    joystick_init();
    accelerometer_init(); 
    while(1){
       

    } 
    AudioMixer_cleanup(); 
    joystick_cleanup(); 
    accelerometer_cleanup(); 
    freeSounds(); 
    return 0; 
}