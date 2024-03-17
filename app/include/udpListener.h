// udpListener.h
// Module to listen to port PORT in the background (uses a thread).
//
// The application will continuously check for messages received and give
// the correct output. 

#ifndef _UDPLISTENER_H_
#define _UDPLISTENER_H_

#define PORT 12345
#define BUFFER_SIZE 16384

// Begins the background thread which listens to UDP PORT.
// Thread finishes when stop is called
pthread_t UDP_init();

// Listens to PORT for messages
void* UDP_startListening();

// Takes the received message in buff and populates msg with the correct response
void UDP_parseMessage(char* buff, int bytesRead, char* msg);

// setBPM from joystick or udp command
void setBpm(int b);
int getBpm();

// setMode from joystick or udp command
void setMode(int m);
int getMode();

// set sound 0 to be played from udp command
void setPressedX(bool x);
bool getPressedX();
// set sound 1 to be played from udp command
void setPressedY(bool y);
bool getPressedY();
// set sound 2 to be played from udp command
void setPressedZ(bool z);
bool getPressedZ();

#endif