/*
 * teleop.h
 *
 *  Created on: Nov 23, 2026
 *      Author: lewis
 */

#ifndef INC_TELEOP_H_
#define INC_TELEOP_H_

#define WASD_FORWARD 		119
#define WASD_LEFT 			97
#define WASD_RIGHT			100
#define WASD_BACKWARD		115
#define SPACEBAR_STOP		32
#define Q_DISCONNECT		113

void wasd_control(char input);

#endif /* INC_TELEOP_H_ */
