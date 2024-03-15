#include "hal/joystick.h"
#include "hal/timer.h"

static pthread_t jpid; 

void joystick_init(void) {
    runCommand(UP);
    runCommand(DOWN);
    runCommand(LEFT);
    runCommand(RIGHT);
    runCommand(CENTER); 

    // launch joystick thread 
    pthread_create(&jpid, NULL, joystickController, NULL); 
}

void joystick_cleanup(){
    pthread_cancel(jpid); 
    pthread_join(jpid, NULL); 
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
    int bpm = 120; 
    int vol = 80; 
    int beatMode = 0;
    AudioMixer_setVolume(vol);
    while(1){
        switch(response()){
            case 1: // UP 
                vol = AudioMixer_getVolume() + 5;
                if(vol > 100){
                    vol = 100; 
                }
                AudioMixer_setVolume(vol);
                break; 
            case 2: // DOWN
                vol = AudioMixer_getVolume() - 5; 
                if(vol < 0){
                    vol = 0; 
                }
                AudioMixer_setVolume(vol);
                break; 
            case 3: // left
                bpm -= 5; 
                if(bpm < 40){
                    bpm = 40;
                }
                break;  
            case 4: // right
                bpm +=5; 
                if(bpm > 300){
                    bpm = 300; 
                }
                break; 
            case 5: // center
                beatMode++; 
                if(beatMode > 2){
                    beatMode = 0; 
                }
                break; 
        }

        // play mode and beat 
        if(beatMode == 0){
            noDrumBeat(); 
        }else if(beatMode == 1){
            standardRockBeat(bpm); 
        }else{
            otherBeat(bpm); 
        }
    }
}

