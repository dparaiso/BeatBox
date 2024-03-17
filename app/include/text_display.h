// text_display.h
// Module to display data about the currently beat being played and the accelerometer in the background (uses a thread).

// Displays the following:
// Line 1:
// - Beat mode (M0, M1, M2)
// - Tempo (eg. 90bpm )
// - Volume 
// - Time between refilling audio playback buffer in audio_mixer.c
// - Time between samples of the accelerometer
// Line 2:
// Display 20 sample from the previous second


#ifndef TXT_DISPLAY_H_
#define TXT_DISPLAY_H_

void Txt_init();
void Txt_cleanup();

void* Txt_startDisplay();

#endif