/*
 * scan.h
 *
 *  Created on: Apr 21, 2021
 *      Author: edbryant
 */

#ifndef SCAN_H_
#define SCAN_H_

#include <stdint.h>

#include <stdbool.h>

#include <inc/tm4c123gh6pm.h>

#include "driverlib/interrupt.h"

#include <stdio.h>

#include <string.h>

//PING Sensor
void ping_init (void);

void ping_trigger (void);

void TIMER3B_Handler(void);

float ping_getDistance (void);

//IR Sensor
void adc_init();

int adc_read();

double adc_distance(int);

//PWM Motor
void servo_init();

int servo_move(float);

#endif /* SCAN_H_ */
