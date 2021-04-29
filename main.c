/**
 * @file lab8_template.c
 * @author
 * Template file for CprE 288 Lab 8
 */

#define RESOLUTION 8
#define MARGIN_OF_ERROR 1
#define ROOM_SIZE 60
#define FIELD_SIZE 440
#define MAP_SIDE_LENGTH (FIELD_SIZE / RESOLUTION)
#define PI 3.14159265

#include "globalVariables.h"

int mapSideLength = MAP_SIDE_LENGTH;

int width = MAP_SIDE_LENGTH;
int height = MAP_SIDE_LENGTH / 2;

#include "Mapping.h"
#include "uart-interrupt.h"
#include "scan.h"
#include "open_interface.h"
#include "resetSimulation.h"
#include "Timer.h"

// Uncomment or add any include directives that are needed

int main(void) {

//    resetSimulationBoard();
//
//    return;


    uart_interrupt_init();
    oi_uartInit();
    initializeMap(map);
    ping_init();
    adc_init();
    servo_init();
    timer_init();

    setField(MAP_SIDE_LENGTH, MARGIN_OF_ERROR, RESOLUTION, ROOM_SIZE, MAP_SIDE_LENGTH, MAP_SIDE_LENGTH / 2);

    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data); // do this only once at start of main()

    setUpDefaultMap(width, height);


    int degrees = 0;


    while (1)
    {

        scanInFront(sensor_data);

        coordinate room[10];

        findPotentialRooms(width, height, room);

        //shiftMap(RIGHT);

        int test = 5;
	}

}
