#ifndef MAPPING_H_
#define MAPPING_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

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
}

// Initialize map
void initializeMap();

// Put Object at (x,y) coord
void putAtPosition(coordinate pos, OBSTACLE obj);

// Scan and put nearby object into map
void scanNearby( coordinate pos );

//Return running map of objects
int** getMap();

// Set pre-defined map
void setMap(int** map);

// Return list of found rooms, return position of the center of the room
coordinate* findRooms();



#endif /* MAPPING_H_ */
