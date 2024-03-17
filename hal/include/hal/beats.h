// beats.h
// contains enum BeatIndex which has 4 types including 
// - BEATS_NO_DRUM_BEAT
// - BEATS_STANDARD_ROCK_BEAT
// - BEATS_OTHER_BEAT
// - NUM_BEATS
// Module contains the logic for playing different beats and sounds


#ifndef _BEATS_H_
#define _BEATS_H_

typedef enum Beats_BeatIndex_t {
    BEATS_NO_DRUM_BEAT,
    BEATS_STANDARD_ROCK_BEAT,
    BEATS_OTHER_BEAT,
    NUM_BEATS
}Beats_BeatIndex;

// initialize wave files
void initializeSounds();
// returns active beat
Beats_BeatIndex getActive();
// frees wave files
void freeSounds(); 
// cancels all threads in playmode()
void cancelThread(); 
// plays rock beat
void* standardRockBeat();
// plays a different beat
void* otherBeat();
void* playMode();
// plays snare
void* playAccelX();
// plays hi-hat
void* playAccelY();
// plays bass
void* playAccelZ();

#endif