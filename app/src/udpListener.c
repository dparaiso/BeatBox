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

static pthread_t tid; 
int bpm;
int mode;
bool pressedX = false; 
bool pressedY = false; 
bool pressedZ = false; 

void setBpm(int b) {
  bpm = b;
}
int getBpm() {
  return bpm;
}

void setMode(int m) {
  mode  = m;
}
int getMode() {
  return mode;
}
bool getPressedX(){
  return pressedX; 
}
bool getPressedY(){
  return pressedX; 
}
bool getPressedZ(){
  return pressedX; 
}
pthread_t UDP_init() {
  setBpm(120);
  setMode(0);
  pthread_create(&tid, NULL, &UDP_startListening, NULL);
  return tid;
} 

void UDP_cleanup(){
    pthread_cancel(tid); 
    pthread_join(tid, NULL); 
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
  
  char* recvTok = strtok(recvMsg," ");
  recvTok = strtok(NULL," ");
  if(recvTok != NULL) {
    int newMode = (int) strtol(recvTok, NULL, 10);
    setMode(newMode);
    snprintf(msg, BUFFER_SIZE, "beat changed");
  }
  else {
    snprintf(msg, BUFFER_SIZE, "error: beat not changed");
  }
}


//TODO: not tested
void UDP_setVolume(char* recvMsg, char* msg, int len) {
  char* recvTok = strtok(recvMsg," ");
  recvTok = strtok(NULL," ");
  if(recvTok != NULL) {
    int newVol = (int) strtol(recvTok, NULL, 10);
    AudioMixer_setVolume(newVol);
    snprintf(msg, BUFFER_SIZE, "volume set");
  }
  else {
    snprintf(msg, BUFFER_SIZE, "error: volume not changed");
  }
}


//TODO: not complete
void UDP_setBpm(char* recvMsg, char* msg, int len) {
  char* recvTok = strtok(recvMsg," ");
  recvTok = strtok(NULL," ");
  recvTok = strtok(NULL," ");
  if(recvTok != NULL) {
    int newVol = (int) strtol(recvTok, NULL, 10);
    
    snprintf(msg, BUFFER_SIZE, "bpm set");
  }
  else {
    snprintf(msg, BUFFER_SIZE, "error: bpm not changed");
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

  printf("Listening on port: %d\n", PORT);
  do {    
    int bytesRead = UDP_receiveAndConnect(sockId, buff, client, &clientLen);
    UDP_parseAndSend(sockId, buff, bytesRead);
  }
  while(strcmp(buff, "stop") != 0);
  printf("closing udp\n");
  close(sockId);
  return NULL;
}