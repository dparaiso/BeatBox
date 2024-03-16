#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_


#include <stdbool.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h> 

#define LEFT "config-pin p8.18 gpio"
#define RIGHT "config-pin p8.15 gpio"
#define UP "config-pin p8.14 gpio"
#define DOWN "config-pin p8.16 gpio"
#define CENTER "config-pin p8.17 gpio"

#define LEFT_FILE "/sys/class/gpio/gpio65/value"
#define RIGHT_FILE "/sys/class/gpio/gpio47/value"
#define UP_FILE "/sys/class/gpio/gpio26/value"
#define DOWN_FILE "/sys/class/gpio/gpio46/value"
#define CENTER_FILE "/sys/class/gpio/gpio27/value"

void joystick_init(void);
void joystick_cleanup();
void beatMode_init();
void beatMode_cleanup(); 
int readFromFileToScreen(char *fileName);
void runCommand(char* command);
int response(void);
void* joystickController();
int getBpm();

#endif