/*
 * movement.c
 *
 *  Created on: Feb 11, 2021
 *      Author: Lamarque
 */
#include "open_interface.h"
#include "movement.h"
#include "collision_detection.h"
#include "Mapping.h"

double move_forward(oi_t *sensor_data, double distance_mm)
{
    double toMove = 0; //how far it moves
    oi_setWheels(200, 200); //move forward at full speed

    while (toMove < distance_mm)
    { //move for certain amount

        oi_update(sensor_data); //update sensor data
        if ((sensor_data->bumpLeft == true) || (sensor_data->bumpRight == true))
        {
            detect_collision(sensor_data);
            break;

        }
        if (sensor_data->cliffLeftSignal > 4000 || sensor_data->cliffRightSignal > 4000
            || sensor_data->cliffFrontLeftSignal > 4000 || sensor_data->cliffFrontRightSignal > 4000)
        {
            detect_border(sensor_data);
            break;

        }

        if (sensor_data->cliffLeft || sensor_data->cliffRight
            || sensor_data->cliffFrontLeft || sensor_data->cliffFrontRight)
        {
            detect_hole(sensor_data);
            break;
        }
        toMove = toMove + sensor_data->distance; //calculate how far it has moved
        moveRobotOnMap(toMove);
    }

    oi_setWheels(0, 0); //stop moving
    return toMove;
}
double move_backward(oi_t *sensor_data, double distance_mm)
{

    double sum = 0; // distance member in oi_t struct is type double

    oi_setWheels(-300, -300); //move forward at full speed

    while (sum > distance_mm)
    {
        oi_update(sensor_data);
        sum = sum + sensor_data->distance; // use -> notation since pointer
    }
    oi_setWheels(0, 0);
    return sum;

}
void turn_right(oi_t *sensor_data, double degrees)
{
    changeRobotDirection(degrees);
    double turn = 0; //
    oi_update(sensor_data);
    while (turn < degrees)
    {
        oi_setWheels(100, -100); //rotate right
        oi_update(sensor_data);
        turn = turn + sensor_data->angle; //pointer into oi_t struct angle variable
    }
    oi_setWheels(0, 0);
}
void turn_left(oi_t *sensor_data, double degrees)
{
    changeRobotDirection(-degrees);
    double turn = 0;
    oi_update(sensor_data);
    degrees = degrees * -1;
    while (turn > degrees)
    {
        oi_setWheels(-100, 100); //rotate left
        oi_update(sensor_data);
        turn = turn + sensor_data->angle; //pointer into oi_t struct angle variable
    }
    oi_setWheels(0, 0);
}

