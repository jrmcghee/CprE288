/*
 * open_interface.h
 *
 *  Used to hold struct of sensor data.
 *  Refined based on given open_interface.h file from lab
 *
 *
 *  Created on: Apr 15, 2021
 *      Author: Lamarque
 */

#ifndef OPEN_INTERFACE_H_
#define OPEN_INTERFACE_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "Timer.h"
#include <inc/tm4c1123gh6pm.h>
#include "lcd.h"

#define M_PI 3.14159265358979323846
#define BIT0        0x01
#define BIT1        0x02
#define BIT2        0x04
#define BIT3        0x08
#define BIT4        0x10
#define BIT5        0x20
#define BIT6        0x40
#define BIT7        0x80

//Sensor Data struct
typedef struct
{
    //Boolean sensor values | Sense hole that represents wrong room
    uint32_t wheelDropLeft :1;
    uint32_t wheelDropRight :1;
    uint32_t bumpLeft :1;
    uint32_t bumpRight :1;
    uint32_t cliffLeft :1;
    uint32_t cliffFrontLeft :1;
    uint32_t cliffFrontRight :1;
    uint32_t cliffRight :1;

    //light sensors
    uint32_t lightBumperRight :1;
    uint32_t lightBumperFrontRight :1;
    uint32_t lightBumperCenterRight :1;
    uint32_t lightBumperCenterLeft :1;
    uint32_t lightBumperFrontLeft :1;
    uint32_t lightBumperLeft :1;

    //Light bump sensors
    uint16_t lightBumpLeftSignal;
    uint16_t lightBumpFrontLeftSignal;
    uint16_t lightBumpCenterLeftSignal;
    uint16_t lightBumpCenterRightSignal;
    uint16_t lightBumpFrontRightSignal;
    uint16_t lightBumpRightSignal;

    //Cliff sensors
    uint16_t cliffLeftSignal;
    uint16_t cliffFrontLeftSignal;
    uint16_t cliffFrontRightSignal;
    uint16_t cliffRightSignal;

    //Power
    int16_t leftMotorCurrent;
    int16_t rightMotorCurrent;
    int16_t mainBrushMotorCurrent;
    int16_t sideBrushMotorCurrent;

    //Motion sensors
    double distance;
    double angle;
    int8_t requestedVelocity;
    int8_t requestedRadius;
    int16_t requestedRightVelocity;
    int16_t requestedLeftVelocity;
    int16_t leftEncoderCount;
    int16_t rightEncoderCount;

    //Misc
    uint8_t oiMode;
    uint8_t numberOfStreamPackets;
    uint8_t stasis;
}

///Allocate and clear all memory for OI Struct
oi_t* oi_alloc();

///Free memory from pointer to Open Interface Struct
void oi_free(oi_t *self);

///Initialize open interface
void oi_init(oi_t *self);

void oi_close();

///Update sensor data
void oi_update(oi_t *self);

/// \brief Set direction and speed of the robot's wheels
/// \param linear velocity in mm/s values range from -500 -> 500 of right wheel
/// \param linear velocity in mm/s values range from -500 -> 500 of left wheel
void oi_setWheels(int16_t right_wheel, int16_t left_wheel);

/// Calls in built in demo to send the iRobot to an open home base
/// This will cause the iRobot to enter the Passive state
void go_charge(void);

char* oi_checkFirmware();
//initializes interrupt and gpio to handle button press to end OI
void oi_shutoff_init(void);

//used to handle interrupt to shut off OI
void GPIOF_Handler(void);

//used to get the current moved degrees from encoder count
static double oi_getDegrees(oi_t *self);

// Get the number of radians moved since last call
static double oi_getRadians(oi_t *self);

// Gets the distance moved since the last call to getDistance
static double oi_getDistance(oi_t *self);

// Sets the calibration factor for the motors. Defualt is 1
void oi_setMotorCalibration(double left, double right);

// Gets the encoder calibration value for the left encoder
double oi_getMotorCalibrationLeft(void);

// Gets the encoder calibration for the right encoder
double oi_getMotorCalibrationRight(void);

#endif /* OPEN_INTERFACE_H_ */
