/*
 * movement.h
 *
 *  Created on: Feb 11, 2021
 *      Author: Lamarque
 */
#include "open_interface.h"

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

double move_forward(oi_t *sensor_data, double distance_mm);
double move_backward(oi_t *sensor_data, double distance_mm);
void turn_right(oi_t *sensor_data, double degrees);
void turn_left(oi_t *sensor_data, double degrees);

#endif /* MOVEMENT_H_ */
