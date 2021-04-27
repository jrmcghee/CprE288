/**
 * @file lab8_template.c
 * @author
 * Template file for CprE 288 Lab 8
 */

#include "Mapping.h"
#include "uart-interrupt.h"
#include "scan.h"
#include "open_interface.h"
#include "resetSimulation.h"
#include "Timer.h"

#define PI 3.14159265

// Uncomment or add any include directives that are needed

int main(void) {

    uart_interrupt_init();
    initializeMap();
    ping_init();
    adc_init();
    servo_init();
    timer_init();

    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data); // do this only once at start of main()

    setUpDefaultMap();


    int degrees = 0;

    coordinate currentPosition;
    int startingX = -180 + 220;
    int startingY = 0 + 220;

    currentPosition.x = startingX;
    currentPosition.y = startingY;

    //resetSimulationBoard();
    while (1)
    {

        oi_setWheels(0,0);

        double servoPosition = 0;

        servo_move(servoPosition);

        int i = 0;

        for( i = 0; i < 180; i += 5){

            servo_move(i);

            double dist = 0;
            unsigned long raw_dis = adc_read();
            dist = adc_distance(raw_dis);
            double angle = (90 - i) * PI / 180.0;

            if( dist < 150){
                int calculatedX = currentPosition.x + dist * cos(angle);
                int calculatedY = currentPosition.y + dist * sin(angle);

                coordinate obstaclePos;
                obstaclePos.x = calculatedX;
                obstaclePos.y = calculatedY;

                int j = calculatedX;

                for( j = currentPosition.x; j < calculatedX; j++){
                    coordinate measuringPosition;
                    measuringPosition.x = j;
                    measuringPosition.y = currentPosition.y + ((calculatedY - currentPosition.y) / (calculatedX - currentPosition.x)) * j;
                    putAtPosition(measuringPosition, EMPTY);
                }

                putAtPosition( obstaclePos, TALL_OBJ);
                putAtPosition( currentPosition, ROBOT);
            }

            timer_waitMillis(400);
            printMap();
        }
	}

}
