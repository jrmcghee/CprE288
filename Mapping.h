#ifndef MAPPING_H_
#define MAPPING_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "movement.h"
#include "scan.h"
#include "open_interface.h"

typedef struct coordinate {
    int x;
    int y;
} coordinate;

typedef enum OBSTACLE {
    UNDISCOVERED = 1,
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

typedef enum MAPSIDE {
    TOP,
    LEFT,
    RIGHT,
    BOTTOM
} MAPSIDE;

void setField(int mapSide, int errorMargin, int res, int roomWidth, int widthSet, int heightSet);

//void initializeMap(char setmap[width][height]);

void setUpDefaultMap(int width, int height);


void putAtPosition(coordinate pos, OBSTACLE obj);

void scanNearby( int x, int y, int radius, oi_t* io);


// Scan and put nearby object into map
void scanNearbyCoords( coordinate pos, int radius, oi_t* io);

void scanInFront(oi_t * io);

void changeRobotDirection(double newAngle);

void moveRobotOnMap(int distance_mm);

// List of completed rooms
coordinate* findRooms(int width, int height);

// Return list of found rooms, return position of the center of the room
void findPotentialRooms(int width, int height, coordinate foundRooms[10]);

coordinate* locationsToScan();

int findWichCorner(coordinate pos, unsigned int searchCondition);

int cmToIndex(int num);

void shiftMap(MAPSIDE dir);


#endif /* MAPPING_H_ */
