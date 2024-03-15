#include "hal/audio_mixer.h"
#include "hal/beats.h"
#include "hal/timer.h"

static char* files[] = {"wave-files/100059__menegass__gui-drum-snare-soft.wav", // snare
                        "wave-files/100053__menegass__gui-drum-cc.wav",         // hi-hat
                        "wave-files/100051__menegass__gui-drum-bd-hard.wav"   // bass
                        };   
static wavedata_t wavSounds[3];


//noDrumBeat is index 0, standardRockBeat is index 1, otherBeat is index 2
static int bpmArray[] =  {0, 0, 0};
static int activeArray[] =  {0, 0, 0};

void initializeSounds(){
    for(int i = 0; i < 3; i++){
        AudioMixer_readWaveFileIntoMemory(files[i], &wavSounds[i] );
    }
}

void noDrumBeat(){
    bpmArray[BEATS_NO_DRUM_BEAT] = 0;
    activeArray[BEATS_NO_DRUM_BEAT] = 1;
    activeArray[BEATS_STANDARD_ROCK_BEAT] = 0;
    activeArray[BEATS_OTHER_BEAT] = 0;
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

    bpmArray[BEATS_STANDARD_ROCK_BEAT] = bpm;
    activeArray[BEATS_NO_DRUM_BEAT] = 0;
    activeArray[BEATS_STANDARD_ROCK_BEAT] = 1;
    activeArray[BEATS_OTHER_BEAT] = 0;
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

    bpmArray[BEATS_OTHER_BEAT] = bpm;
    activeArray[BEATS_NO_DRUM_BEAT] = 0;
    activeArray[BEATS_STANDARD_ROCK_BEAT] = 0;
    activeArray[BEATS_OTHER_BEAT] = 1;
}

Beats_BeatIndex getActive() {
    for(int i = 0; i < NUM_BEATS; i++) {
        if(activeArray[i]) {
            return (Beats_BeatIndex)i;
        }
    }
    return NUM_BEATS;
}

int getBpm(Beats_BeatIndex index) {
    if(index >= NUM_BEATS) {
        return NUM_BEATS;
    }
    return bpmArray[index];
}