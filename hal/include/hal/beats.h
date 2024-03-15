#ifndef _BEATS_H_
#define _BEATS_H_

void initializeSounds();
void freeSounds(); 
void cancelThread(); 
void* standardRockBeat();
void* otherBeat();
void* playMode();

#endif