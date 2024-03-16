#ifndef _BEATS_H_
#define _BEATS_H_

typedef enum Beats_BeatIndex_t {
    BEATS_NO_DRUM_BEAT,
    BEATS_STANDARD_ROCK_BEAT,
    BEATS_OTHER_BEAT,
    NUM_BEATS
}Beats_BeatIndex;

void initializeSounds();
<<<<<<< HEAD
void noDrumBeat();
void standardRockBeat(int bpm);
void otherBeat(int bpm);
Beats_BeatIndex getActive();
int getBpm(Beats_BeatIndex index);
=======
void freeSounds(); 
void cancelThread(); 
void* standardRockBeat();
void* otherBeat();
void* playMode();
>>>>>>> main

#endif