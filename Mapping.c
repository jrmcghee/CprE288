#include "Mapping.h"

int resolution = 4; // This is cm / index in the map array
int marginOfError = 1; //Number of indices that the corners of a room could be off by
int roomSize = 10; // This is the size of the room in cm
int fieldSize = 400; // Side length of the field in cm
int mapSideLength;

unsigned int** map;

coordinate currentPosition;

void initializeMap(){

    mapSideLength = fieldSize / resolution;
    map = malloc( mapSideLength * sizeof(unsigned int*) + mapSideLength * mapSideLength * sizeof( unsigned int ) );
    int i = 0;
    for( i = 0; i < mapSideLength; i++){
        map[i] = malloc(mapSideLength * sizeof(unsigned int));
        int j = 0;
        for(j = 0; j < mapSideLength; j++){
            map[i][j] = UNDISCOVERED;
        }
    }
}


void putAtPosition(coordinate pos, OBSTACLE obj){
    map[pos.x][pos.y] = obj;
}


// Scan and put nearby object into map
void scanNearby( coordinate pos ){

}

//Return running map of objects
unsigned int** getMap(){
    return map;
}

// Set pre-defined map
void setMap(unsigned int** preSetMap){
    map = preSetMap;
}

// List of completed rooms
coordinate* findRooms(){

    coordinate foundRooms[10];
    int idx = 0;


    int i = 0;

    int roomSizeInIndexes = roomSize / resolution;

    for( i = 0; i < mapSideLength; i++){
        int j = 0;
        for(j = 0; j < mapSideLength; j++){
            if(map[i][j] == TALL_OBJ){
                coordinate searchPos;
                searchPos.x = i;
                searchPos.y = j;
                int corner = findWichCorner(searchPos, 2 << TALL_OBJ);

                switch(corner){
                    case TOP_RIGHT:
                        searchPos.x -= ( roomSizeInIndexes / 2);
                        searchPos.y -= (roomSizeInIndexes / 2);
                        break;
                    case TOP_LEFT:
                        searchPos.x += ( roomSizeInIndexes / 2);
                        searchPos.y -= (roomSizeInIndexes / 2);
                        break;
                    case BOTTOM_RIGHT:
                        searchPos.x -= ( roomSizeInIndexes / 2);
                        searchPos.y += (roomSizeInIndexes / 2);
                        break;
                    case BOTTOM_LEFT:
                        searchPos.x += ( roomSizeInIndexes / 2);
                        searchPos.y += (roomSizeInIndexes / 2);
                        break;
                };

                foundRooms[idx++] = searchPos;
            }
        }
    }
}

// Return list of found rooms, return position of the center of the room
coordinate* findPotentialRooms(){

    coordinate foundRooms[10];
    int idx = 0;


    int i = 0;

    int roomSizeInIndexes = roomSize / resolution;

    for( i = 0; i < mapSideLength; i++){
        int j = 0;
        for(j = 0; j < mapSideLength; j++){
            if(map[i][j] == TALL_OBJ){
                coordinate searchPos;
                searchPos.x = i;
                searchPos.y = j;
                int corner = findWichCorner(searchPos, 2 << TALL_OBJ || 2 << UNDISCOVERED);

                switch(corner){
                    case TOP_RIGHT:
                        searchPos.x -= ( roomSizeInIndexes / 2);
                        searchPos.y -= (roomSizeInIndexes / 2);
                        break;
                    case TOP_LEFT:
                        searchPos.x += ( roomSizeInIndexes / 2);
                        searchPos.y -= (roomSizeInIndexes / 2);
                        break;
                    case BOTTOM_RIGHT:
                        searchPos.x -= ( roomSizeInIndexes / 2);
                        searchPos.y += (roomSizeInIndexes / 2);
                        break;
                    case BOTTOM_LEFT:
                        searchPos.x += ( roomSizeInIndexes / 2);
                        searchPos.y += (roomSizeInIndexes / 2);
                        break;
                };

                foundRooms[idx++] = searchPos;
            }
        }
    }
    
}

coordinate* locationsToScan(){

}

int findWichCorner(coordinate pos, unsigned int searchCondition){

    int roomSizeInIndexes = roomSize / resolution;

    if(((pos.x - roomSizeInIndexes) >= 0) && ((pos.y - roomSizeInIndexes >= 0))){
        if( (2 << map[pos.x][pos.y] & searchCondition) && (2 << map[pos.x - roomSizeInIndexes][pos.y] & searchCondition) && (2 << map[pos.x][pos.y - roomSizeInIndexes] & searchCondition) && (2 << map[pos.x - roomSizeInIndexes][pos.y - roomSizeInIndexes] & searchCondition) ){
            return TOP_RIGHT; // Found room, passed in position was top-right
        }
    }

    if(((pos.x - roomSizeInIndexes) >= 0) && ((pos.y + roomSizeInIndexes < mapSideLength))){
        if( (2 << map[pos.x][pos.y] & searchCondition) && (2 << map[pos.x - roomSizeInIndexes][pos.y] & searchCondition) && (2 << map[pos.x][pos.y + roomSizeInIndexes] & searchCondition) && (2 << map[pos.x - roomSizeInIndexes][pos.y + roomSizeInIndexes] & searchCondition) ){
            return BOTTOM_RIGHT; // Found room, passed in position was bottom-right
        }
    }

    if(((pos.x + roomSizeInIndexes) >= 0) && ((pos.y - roomSizeInIndexes >= 0))){
        if( (2 << map[pos.x][pos.y] & searchCondition) && (2 << map[pos.x + roomSizeInIndexes][pos.y] & searchCondition) && (2 << map[pos.x][pos.y - roomSizeInIndexes] & searchCondition) && (2 << map[pos.x + roomSizeInIndexes][pos.y - roomSizeInIndexes] & searchCondition) ){
            return TOP_LEFT; // Found room, passed in position was top-left
        }
    }

    if(((pos.x + roomSizeInIndexes) >= 0) && ((pos.y + roomSizeInIndexes < mapSideLength))){
        if( (2 << map[pos.x][pos.y] & searchCondition) && (2 << map[pos.x + roomSizeInIndexes][pos.y] & searchCondition) && (2 << map[pos.x][pos.y + roomSizeInIndexes] & searchCondition) && (2 << map[pos.x + roomSizeInIndexes][pos.y + roomSizeInIndexes] & searchCondition) ){
            return BOTTOM_LEFT; // Found room, passed in position was bottom-left
        }
    }

    return -1;
}