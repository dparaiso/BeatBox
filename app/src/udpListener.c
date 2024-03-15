#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "udpListener.h"
#include "hal/audio_mixer.h"
#include "hal/beats.h"

pthread_t tid; 
char lastBuff [BUFFER_SIZE];

void UDP_init() {
  pthread_create(&tid, NULL, &UDP_startListening, NULL);
  pthread_join(tid, NULL); 
} 

void UDP_cleanup(pthread_t tid){
    pthread_cancel(tid); 
}

static int UDP_receiveAndConnect(int sockId, char* buff, struct sockaddr_in client, int* clientLen) {
  int bytesRead = recvfrom(sockId, buff, BUFFER_SIZE-1, 0, (struct sockaddr*)&client, (socklen_t *) clientLen);
  buff[bytesRead-1] = '\0';
  if(connect(sockId, (struct sockaddr*)&client, *clientLen) == -1) {
    perror("socket connect failed");
    exit(EXIT_FAILURE);
  }
  return bytesRead;
}

//TODO: not complete nor tested
void UDP_cycleBeat(char* recvMsg, char* msg, int len) {
  Beats_BeatIndex active = getActive();
  if(active >= NUM_BEATS) {
    snprintf(msg, "error: beat not cycled", BUFFER_SIZE);
    return;
  }

  switch (active)
  {
  case BEATS_NO_DRUM_BEAT:
    standardRockBeat(120); 
    break;
  
  case BEATS_STANDARD_ROCK_BEAT:
    otherBeat(120);
    break;
  case BEATS_OTHER_BEAT:
    noDrumBeat();
    break;
  default:
    snprintf(msg, "error: beat not cycled", BUFFER_SIZE);
    return;
  }

  snprintf(msg, "beat cycled", BUFFER_SIZE);
}


//TODO: not tested
void UDP_setVolume(char* recvMsg, char* msg, int len) {
  char* recvTok = strtok(recvMsg," ");
  recvTok = strtok(NULL," ");
  recvTok = strtok(NULL," ");
  if(recvTok != NULL) {
    int newVol = (int) strtol(recvTok, NULL, 10);
    AudioMixer_setVolume(newVol);
    snprintf(msg, "volume set", BUFFER_SIZE);
  }
  else {
    snprintf(msg, "error: volume not changed", BUFFER_SIZE);
  }
}


//TODO: not complete
void UDP_setBpm(char* recvMsg, char* msg, int len) {
  char* recvTok = strtok(recvMsg," ");
  recvTok = strtok(NULL," ");
  recvTok = strtok(NULL," ");
  if(recvTok != NULL) {
    int newVol = (int) strtol(recvTok, NULL, 10);
    
    snprintf(msg, "bpm set", BUFFER_SIZE);
  }
  else {
    snprintf(msg, "error: bpm not changed", BUFFER_SIZE);
  }
}

//TODO: not complete
void UDP_playBeat(char* recvMsg, char* msg, int len) {
  
}

void UDP_stopProgram(char* recvMsg, char* msg, int len) {
  char newMsg[] = "Program terminating\n\n";
  strncpy(msg, newMsg, strlen(newMsg)+1);
}

void UDP_parseMessage(char* buff, int bytesRead, char* msg) { 
  char* possibleCommands[] = {"cycle_beat", "volume", "bpm", "play beat", "stop"};
  char recvMsg[bytesRead];
  for(int i = 0; i < bytesRead; i++) {
    recvMsg[i] = tolower(buff[i]);
  }
  if(strstr(recvMsg, possibleCommands[0]) != NULL) {
    UDP_cycleBeat(recvMsg, msg, bytesRead);
  }
  else if(strstr(recvMsg, possibleCommands[1]) != NULL) {
    UDP_setVolume(recvMsg, msg, bytesRead);
  }
  else if(strstr(recvMsg, possibleCommands[2]) != NULL) {
    UDP_setBpm(recvMsg, msg, bytesRead);
  }
  else if(strstr(recvMsg, possibleCommands[3]) != NULL) {
    UDP_playBeat(recvMsg, msg, bytesRead);
  }
  else if(strstr(recvMsg, possibleCommands[4]) != NULL) {
    UDP_stopProgram(recvMsg, msg, bytesRead);
  }               
  else {
    char newMsg[] = "unknown command\n\n"; 
    strncpy(msg, newMsg, strlen(newMsg)+1);
  }
}

static void UDP_parseAndSend(int sockId, char* buff, int bytesRead) {
  char msg[BUFFER_SIZE];
  UDP_parseMessage(buff, bytesRead, msg);
  strncpy(lastBuff, buff, bytesRead);

  // strncpy(lastMsg, msg, strlen(msg)+1);
  send(sockId, msg, sizeof(char)*(strlen(msg)+1), 0);
}

void* UDP_startListening() {
  struct sockaddr_in addr;
  struct sockaddr_in client;
  int sockId = socket(AF_INET, SOCK_DGRAM, 0);
  if(sockId < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  } 

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(PORT);

  if(bind(sockId, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    perror("socket bind failed");
    exit(EXIT_FAILURE);
  }

  char buff[BUFFER_SIZE];
  int clientLen = sizeof(client);

  char defaultLastBuff[] = "";
  strncpy(lastBuff, defaultLastBuff, strlen(defaultLastBuff));
  printf("Listening on port: %d\n", PORT);
  do {    
    int bytesRead = UDP_receiveAndConnect(sockId, buff, client, &clientLen);
    UDP_parseAndSend(sockId, buff, bytesRead);
  }
  while(strcmp(buff, "stop") != 0);

  close(sockId);
  return NULL;
}