#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include "text_display.h"
#include "hal/timer.h"
#include "periodTimer.h"
#include "hal/audio_mixer.h"
#include "hal/beats.h"

static pthread_t pid; 

void Txt_init() {
    pthread_create(&pid, NULL, &Txt_startDisplay, NULL); 
}
void Txt_cleanup() {
    pthread_cancel(pid); 
    pthread_join(pid, NULL); 
}

void* Txt_startDisplay() {
    long long secondAhead;
    while(true) {
        secondAhead = getTimeInMs() + 1000; 

        //TODO: beat mode
        int beatMode = getActive();
        if(beatMode >= NUM_BEATS) {
            perror("Error: Active beat was not found\n");
            return;
        }
        //TODO: bpm
        int bpm = getBpm(beatMode);
        int vol = AudioMixer_getVolume();
        Period_statistics_t bStats;
        Period_getStatisticsAndClear(PERIOD_EVENT_SAMPLE_BUFFER, &bStats);
        //TODO: accel samples 
        Period_statistics_t aStats;
        Period_getStatisticsAndClear(PERIOD_EVENT_SAMPLE_ACCEL, &aStats);

        char line1[] = "M%d %dbpm vol:%d Audio[%f, %f] avg %f/%d Accel[%f, %f] avg %f/%d";
        printf(line1, beatMode, bpm, vol, bStats.minPeriodInMs, bStats.maxPeriodInMs, bStats.avgPeriodInMs, bStats.numSamples, aStats.minPeriodInMs, aStats.maxPeriodInMs, aStats.avgPeriodInMs, aStats.numSamples);
        if(secondAhead - getTimeInMs() > 0) {
           sleepForMs(secondAhead - getTimeInMs()); 
        }
    }
}