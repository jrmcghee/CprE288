/*
 * detect_collision.h
 *
 *  Created on: Apr 21, 2021
 *      Author: Lamarque
 */

#ifndef COLLISION_DETECTION_H_
#define COLLISION_DETECTION_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inc/tm4c123gh6pm.h>
#include "Timer.h"
#include "open_interface.h"

void detect_collision(oi_t *sensor_data);
void detect_border(oi_t *sensor_data);
void detect_hole(oi_t *sensor_data);

#endif /* COLLISION_DETECTION_H_ */
