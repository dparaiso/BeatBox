#include <stdio.h> 
#include "audio_mixer.h"
#include "hal/timer.h"

static char* files[] = {"wave-files/100059__menegass__gui-drum-snare-soft.wav", //snare 
                        "wave-files/100053__menegass__gui-drum-cc.wav",         // hi-hat
                        "wave-files/100051__menegass__gui-drum-bd-hard.wav"};   // bass
static wavedata_t wavSounds[4];
int main(){
    // load in wav files that we want to play 
    for(int i = 0; i < 3; i++){
        AudioMixer_readWaveFileIntoMemory(files[i], &wavSounds[i] );
    }
    AudioMixer_init(); 
    while(1){
        AudioMixer_queueSound(&wavSounds[1]); //hi-hat
        AudioMixer_queueSound(&wavSounds[2]); //bass
        sleepForHalfBeat(120);
        AudioMixer_queueSound(&wavSounds[1]); //hi-hat
        sleepForHalfBeat(120);
        AudioMixer_queueSound(&wavSounds[1]); //hi-hat
        AudioMixer_queueSound(&wavSounds[0]); //snare
        sleepForHalfBeat(120); 
        AudioMixer_queueSound(&wavSounds[1]); //hi-hat
        sleepForHalfBeat(120);

    } 
    return 0; 
}