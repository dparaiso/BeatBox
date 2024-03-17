// timer.h
// includes functions for tracking time and sleeping 

#ifndef _TIMER_H_
#define _TIMER_H_

#include <time.h>

long long getTimeInMs(void);
void sleepForMs(long long delayInMs);
void sleepForHalfBeat(int bpm);
#endif