#include "hal/audio_mixer.h"
#include "hal/beats.h"
#include "hal/timer.h"

static char* files[] = {"wave-files/100059__menegass__gui-drum-snare-soft.wav", // snare
                        "wave-files/100053__menegass__gui-drum-cc.wav",         // hi-hat
                        "wave-files/100051__menegass__gui-drum-bd-hard.wav"   // bass
                        };   
static wavedata_t wavSounds[3];

void initializeSounds(){
    for(int i = 0; i < 3; i++){
        AudioMixer_readWaveFileIntoMemory(files[i], &wavSounds[i] );
    }
}

void noDrumBeat(){
    // figure out how to turn off
}

void standardRockBeat(int bpm){
    AudioMixer_queueSound(&wavSounds[1]); //hi-hat
    AudioMixer_queueSound(&wavSounds[2]); //bass
    sleepForHalfBeat(bpm);
    AudioMixer_queueSound(&wavSounds[1]); //hi-hat
    sleepForHalfBeat(bpm);
    AudioMixer_queueSound(&wavSounds[1]); //hi-hat
    AudioMixer_queueSound(&wavSounds[0]); //snare
    sleepForHalfBeat(bpm); 
    AudioMixer_queueSound(&wavSounds[1]); //hi-hat
    sleepForHalfBeat(bpm);

}

void otherBeat(int bpm){
    AudioMixer_queueSound(&wavSounds[1]); //hi-hat
    AudioMixer_queueSound(&wavSounds[2]); //bass
    sleepForHalfBeat(bpm); 
    AudioMixer_queueSound(&wavSounds[1]); //hi-hat
    sleepForHalfBeat(bpm); 
    AudioMixer_queueSound(&wavSounds[1]); //hi-hat2
    AudioMixer_queueSound(&wavSounds[2]); //bass
    AudioMixer_queueSound(&wavSounds[0]); //snare
    sleepForHalfBeat(bpm*2); 
    AudioMixer_queueSound(&wavSounds[1]); //hi-hat
    sleepForHalfBeat(bpm*2); 
    AudioMixer_queueSound(&wavSounds[1]); //hi-hat
    sleepForHalfBeat(bpm);     

}