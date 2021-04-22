/*
 * movement.c
 *
 *  Created on: Feb 11, 2021
 *      Author: Lamarque
 */
#include "open_interface.h"
#include "movement.h"

double move_forward(oi_t *sensor_data, double distance_mm)
{
    double toMove = 0; //how far it moves
    oi_setWheels(200, 200); //move forward at full speed

    while (toMove < distance_mm)
    { //move for certain amount

        oi_update(sensor_data); //update sensor data
        if (sensor_data->bumpLeft == true)
        {
            oi_setWheels(0, 0);
            move_backward(sensor_data, 100);
            turn_right(sensor_data, 80);
            move_forward(sensor_data, 100);
            turn_left(sensor_data, 80);
            oi_setWheels(200, 200);

        }
        if (sensor_data->bumpRight == true)
        {
            oi_setWheels(0, 0);
            move_backward(sensor_data, 100);
            turn_left(sensor_data, 80);
            move_forward(sensor_data, 100);
            turn_right(sensor_data, 80);
            oi_setWheels(200, 200);

        }
        toMove = toMove + sensor_data->distance; //calculate how far it has moved
    }

    oi_setWheels(0, 0); //stop moving
    return toMove;
}
double move_backward(oi_t *sensor_data, double distance_mm)
{

    double sum = 0; // distance member in oi_t struct is type double

    oi_setWheels(-500, -500); //move forward at full speed

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
    double turn = 0; //
    oi_update(sensor_data);
    while (turn < degrees)
    {
        oi_setWheels(200, -200); //rotate right
        oi_update(sensor_data);
        turn = turn + sensor_data->angle; //pointer into oi_t struct angle variable
    }
    oi_setWheels(0, 0);
}
void turn_left(oi_t *sensor_data, double degrees)
{
    double turn = 0;
    oi_update(sensor_data);
    degrees = degrees * -1;
    while (turn > degrees)
    {
        oi_setWheels(-200, 200); //rotate left
        oi_update(sensor_data);
        turn = turn + sensor_data->angle; //pointer into oi_t struct angle variable
    }
    oi_setWheels(0, 0);
}

