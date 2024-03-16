#include <stdio.h> 
#include "hal/audio_mixer.h"
#include "hal/timer.h"
#include "hal/beats.h"
#include "hal/joystick.h"
#include "text_display.h"
#include "udpListener.h"
#include "periodTimer.h"

int main(){
    // load in wav files that we want to play 
    Period_init();
    initializeSounds(); 
    AudioMixer_init(); 
    standardRockBeat(120);

    joystick_init();
    
    // Txt_init();
    UDP_init();

    

    // Txt_cleanup();
    joystick_cleanup();
    AudioMixer_cleanup();
    Period_cleanup();
    freeSounds(); 
    return 0; 
}