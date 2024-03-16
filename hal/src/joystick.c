#include "hal/joystick.h"
#include "hal/timer.h"
#include "hal/beats.h"
#include "../../app/include/udpListener.h"
#include "hal/audio_mixer.h"

static pthread_t jpid, modepid; 
static pthread_mutex_t bpmMutex = PTHREAD_MUTEX_INITIALIZER; 

void joystick_init(void) {
    runCommand(UP);
    runCommand(DOWN);
    runCommand(LEFT);
    runCommand(RIGHT);
    runCommand(CENTER); 

    // launch joystick thread 
    pthread_create(&jpid, NULL, &joystickController, NULL);
    beatMode_init(); 
}

void joystick_cleanup(){
    beatMode_cleanup(); 
    pthread_cancel(jpid); 
    pthread_join(jpid, NULL); 
}
void beatMode_init(){
    pthread_create(&modepid, NULL, &playMode, NULL); 
}

void beatMode_cleanup(){
    cancelThread(); 
    pthread_join(modepid, NULL); 
}

int readFromFileToScreen(char *fileName){
    FILE *pFile = fopen(fileName, "r");
    if (pFile == NULL) {
        printf("ERROR: Unable to open file (%s) for read\n", fileName);
        exit(-1);
    }
    // Read string (line)
    const int MAX_LENGTH = 1024;
    char buff[MAX_LENGTH];
    fgets(buff, MAX_LENGTH, pFile);
    // Close
    fclose(pFile);
    return buff[0] - '0';
}

void runCommand(char* command){
    //Execute shell command (output into pipe)
    FILE *pipe = popen(command, "r");

    //Ignore output of the command; but consume it 
    //so we don't get an error when closing the pipe 
    char buffer[1024];
    while(!feof(pipe) && !ferror(pipe)){
        if(fgets(buffer, sizeof(buffer), pipe) == NULL)
        break; 
        // printf("--> %s", buffer); // uncomment for debugging
    }

    // get the exit code from the pipe; non-zero is an error: 
    int exitCode =  WEXITSTATUS(pclose(pipe));
    if (exitCode != 0){
        perror("Unable to execute command: ");
        printf("    command: %s\n", command); 
        printf("    exit code: %d\n", exitCode);
    }

}



int response(void){
    if(!readFromFileToScreen(UP_FILE)){
        return 1;
    }else if(!readFromFileToScreen(DOWN_FILE)){
        return 2;
    }else if(!readFromFileToScreen(LEFT_FILE)){
        return 3;
    }else if(!readFromFileToScreen(RIGHT_FILE)){
        return 4;
    }else if(!readFromFileToScreen(CENTER_FILE)){
        return 5;
    }else{
        return 0; 
    }

}

void* joystickController(){
    int vol = 80; 
    int beatMode = 0;
    int bpm = 0;
    AudioMixer_setVolume(vol);
    while(1){
        switch(response()){
            case 1: // UP 
                vol = AudioMixer_getVolume() + 5;
                if(vol > 100){
                    vol = 100; 
                }
                AudioMixer_setVolume(vol);
                sleepForMs(200);
                break; 
            case 2: // DOWN
                vol = AudioMixer_getVolume() - 5; 
                if(vol < 0){
                    vol = 0; 
                }
                AudioMixer_setVolume(vol);
                sleepForMs(200);
                break; 
            case 3: // left
                pthread_mutex_lock(&bpmMutex); 
                bpm = getBpm();
                if(bpm-5 < 40){
                    setBpm(40);
                }
                else {
                    setBpm(bpm-5);
                }
                pthread_mutex_unlock(&bpmMutex); 
                break;  
            case 4: // right
                pthread_mutex_lock(&bpmMutex); 
                bpm = getBpm();
                if(bpm+5 > 300){
                    setBpm(300);
                }
                else {
                    setBpm(bpm+5);
                }
                pthread_mutex_unlock(&bpmMutex); 
                break; 
        }
        sleepForMs(100);
    }
}

