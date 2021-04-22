/*
 * detect_collision.c
 *
 *  Created on: Apr 21, 2021
 *      Author: Lamarque
 */

#ifndef DETECT_COLLISION_C_
#define DETECT_COLLISION_C_

#include "collision_detection.h"
#include "open_interface.h"
#include "movement.h"

void detect_collision(oi_t *sensor_data)
{
    if (sensor_data->bumpLeft)
    {
        move_backward(sensor_data, -150);
        turn_right(sensor_data, 45);
        move_forward(sensor_data, 300);
        turn_left(sensor_data, -45);
    }

    else if (sensor_data->bumpRight)
    {
        move_backward(sensor_data, -150);
        turn_left(sensor_data, -45);
        move_forward(sensor_data, 300);
        turn_right(sensor_data, 45);

    }
    else if (sensor_data->bumpLeft && sensor_data->bumpRight)
    {
        move_backward(sensor_data, -150);
        turn_right(sensor_data, 180);
        move_forward(sensor_data, 300);
    }

}

void detect_border(oi_t *sensor_data)
{
    if (sensor_data->cliffLeftSignal > 4000)
    {
        turn_right(sensor_data, 180);
    }

    if (sensor_data->cliffRightSignal > 4000)
    {
        turn_left(sensor_data, -180);
    }

    if (sensor_data->cliffFrontLeftSignal > 4000 || sensor_data->cliffFrontRightSignal > 4000)
    {
        turn_right(sensor_data, 180);
    }
}

void detect_hole(oi_t *sensor_data)
{
    if (sensor_data->cliffLeft)
    {
        oi_setWheels(0, 0);
        turn_right(sensor_data, 45);
        move_forward(sensor_data, 100);
        turn_left(sensor_data, -45);
    }
    if (sensor_data->cliffRight)
    {
        oi_setWheels(0, 0);
        turn_left(sensor_data, -45);
        move_forward(sensor_data, 100);
        turn_right(sensor_data, 45);
    }
    if (sensor_data->cliffFrontLeft || sensor_data->cliffFrontRight)
    {
        oi_setWheels(0, 0);
        turn_left(sensor_data, 90);
    }

}

#endif /* DETECT_COLLISION_C_ */
