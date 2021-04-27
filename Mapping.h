#ifndef MAPPING_H_
#define MAPPING_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "movement.h"
#include "open_interface.h"

#define RESOLUTION 8
#define FIELD_SIZE 440
#define MAP_SIDE_LENGTH (FIELD_SIZE / RESOLUTION)

typedef struct coordinate {
    int x;
    int y;
} coordinate;

typedef enum OBSTACLE {
    UNDISCOVERED, 
    EMPTY, 
    TALL_OBJ, 
    SHORT_OBJ, 
    PIT,
    ROOM,
    ROBOT
} OBSTACLE;

typedef enum ROOMCORNER {
    TOP_RIGHT,
    TOP_LEFT,
    BOTTOM_RIGHT,
    BOTTOM_LEFT
} ROOMCORNER;

volatile char* map;


// Initialize map
void initializeMap();

// Put Object at (x,y) coord
void putAtPosition(coordinate pos, OBSTACLE obj);

// Scan and put nearby object into map
void scanNearbyCoords( coordinate pos, int radius, oi_t* io );

// Scan and put nearby object into map
void scanNearby( int x, int y, int radius, oi_t* io );

//Return running map of objects
int** getMap();

// Set pre-defined map
void setMap( int preSetMap[][] );

// Return list of found rooms, return position of the center of the room
coordinate* findRooms();

int cmToIndex(int num);

int getMapSideLength();



#endif /* MAPPING_H_ */
