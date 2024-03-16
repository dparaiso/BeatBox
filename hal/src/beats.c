#include "hal/audio_mixer.h"
#include "hal/beats.h"
#include "hal/timer.h"
#include "hal/joystick.h"
#include <pthread.h> 
#include <stdbool.h> 

static char* files[] = {"wave-files/100059__menegass__gui-drum-snare-soft.wav", // snare
                        "wave-files/100053__menegass__gui-drum-cc.wav",         // hi-hat
                        "wave-files/100051__menegass__gui-drum-bd-hard.wav"   // bass
                        };   
static wavedata_t wavSounds[3];
static pthread_t mpid; 
static pthread_mutex_t bpmMutex = PTHREAD_MUTEX_INITIALIZER; 
bool cancel = false; 


//noDrumBeat is index 0, standardRockBeat is index 1, otherBeat is index 2
static int bpmArray[] =  {0, 0, 0};
static int activeArray[] =  {0, 0, 0};

void initializeSounds(){
    for(int i = 0; i < 3; i++){
        AudioMixer_readWaveFileIntoMemory(files[i], &wavSounds[i] );
    }
}

void freeSounds(){
    bpmArray[BEATS_NO_DRUM_BEAT] = 0;
    activeArray[BEATS_NO_DRUM_BEAT] = 1;
    activeArray[BEATS_STANDARD_ROCK_BEAT] = 0;
    activeArray[BEATS_OTHER_BEAT] = 0;
    for(int i = 0; i < 3; i++){
        AudioMixer_freeWaveFileData(&wavSounds[i] );
    }
}


void cancelThread(){
    cancel = true; 
}

void* standardRockBeat(){
    while(1){
        pthread_mutex_lock(&bpmMutex); 
        int bpm = getBpm(); 
        pthread_mutex_unlock(&bpmMutex); 
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


    bpmArray[BEATS_STANDARD_ROCK_BEAT] = bpm;
    activeArray[BEATS_NO_DRUM_BEAT] = 0;
    activeArray[BEATS_STANDARD_ROCK_BEAT] = 1;
    activeArray[BEATS_OTHER_BEAT] = 0;
}

void* otherBeat(){
    while(1){
        pthread_mutex_lock(&bpmMutex); 
        int bpm = getBpm(); 
        pthread_mutex_unlock(&bpmMutex); 
        AudioMixer_queueSound(&wavSounds[1]); //hi-hat
        AudioMixer_queueSound(&wavSounds[2]); //bass
        sleepForHalfBeat(bpm); 
        AudioMixer_queueSound(&wavSounds[1]); //hi-hat
        sleepForHalfBeat(bpm); 
        AudioMixer_queueSound(&wavSounds[1]); //hi-hat
        AudioMixer_queueSound(&wavSounds[2]); //bass
        AudioMixer_queueSound(&wavSounds[0]); //snare
        sleepForHalfBeat(bpm*2); 
        AudioMixer_queueSound(&wavSounds[1]); //hi-hat
        sleepForHalfBeat(bpm*2); 
        AudioMixer_queueSound(&wavSounds[1]); //hi-hat
        sleepForHalfBeat(bpm);     
    }

} 

void* playMode(){
    int prevMode = 0;  
    int mode = 1;
    bool noThread = true; 
    int last_response = 0; 
    while(!cancel){
        // check if joystick is pressed in center
        if(response() == 5){
            if(last_response < 1){
                mode++;
                last_response++; 
            }
        }else{
            last_response = 0; 
        }

        // cycle beats back to the beginning 
        if(mode > 2){
            mode = 0; 
        }

        // creates thread for chosen beat
        if(noThread){
            if(mode == 0){
                prevMode = 0; 
                noThread = true; 
            }else if(mode == 1){
                pthread_create(&mpid, NULL, &standardRockBeat, NULL); 
                prevMode = 1; 
                noThread = false; 
            }else if(mode == 2){
                pthread_create(&mpid, NULL, &otherBeat, NULL); 
                prevMode = 2;
                noThread = false;  
            }
        }

        // cancel old thread to create a new one 
        if(!noThread && (prevMode != mode)){
            if(prevMode != 0){
                pthread_cancel(mpid); 
                noThread = true; 
            }
        }
        sleepForMs(50);
        
    }

    // clean up existing threads
    if(!noThread){
        pthread_cancel(mpid); 
    }
    return NULL; 
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