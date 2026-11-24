/*
 * teleop.c
 *
 *  Created on: Nov 23, 2026
 *      Author: lewis
 */

#include <stdio.h>

#include "teleop.h"
#include "app.h"

#define WASD_FORWARD 		119
#define WASD_LEFT 			97
#define WASD_RIGHT			100
#define WASD_BACKWARD		115
#define SPACEBAR_STOP		32
#define Q_DISCONNECT		113

void wasd_control(char input) {
	if(input == WASD_FORWARD){
		drive_forwards();
	}
	else if(input == WASD_BACKWARD){
		drive_backwards();
	}
	else if(input == WASD_LEFT){
		drive_left();
	}
	else if(input == WASD_RIGHT){
		drive_right();
	}
	else if(input == SPACEBAR_STOP){
		drive_stop();
	}
	else if(input == Q_DISCONNECT){
		drive_disconnect();
	}
}
