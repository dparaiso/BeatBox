#include <stdio.h> 
#include "hal/audio_mixer.h"
#include "hal/timer.h"
#include "hal/beats.h"
#include "hal/joystick.h"

int main(){
    // load in wav files that we want to play 
    initializeSounds(); 
    AudioMixer_init(); 
    joystick_init();
    while(1){
        // standardRockBeat(120);
        // otherBeat(120); 

    } 
    AudioMixer_cleanup(); 
    joystick_cleanup(); 
    freeSounds(); 
    return 0; 
}