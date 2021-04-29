#include "Mapping.h";
#include "globalVariables.h"

#define min(X,Y) ((X) < (Y) ? (X) : (Y))
#define max(X,Y) ((X) > (Y) ? (X) : (Y))

coordinate currentPosition;
int startingX = 0 + 220;
int startingY = 0 + 122;

int marginOfError;
int resolution;
int roomSize;

double angle = 0.0;

int map[MAP_SIDE_LENGTH][MAP_SIDE_LENGTH / 2];

void setField(int mapSide, int errorMargin, int res, int roomWidth, int widthSet, int heightSet){
    mapSideLength = mapSide;
    marginOfError = errorMargin;
    resolution = res;
    roomSize = roomWidth;
    width = widthSet;
    height = heightSet;
}

void initializeMap(char setmap[width][height]){

    //map = (char*)malloc(sizeof(char[mapSideLength][mapSideLength / 2]));

    int i = 0;
    for( i = 0; i < mapSideLength; i++){
        int j = 0;
        for(j = 0; j < mapSideLength; j++){
            setmap[i][j] = UNDISCOVERED;
        }
    }
}

void setUpDefaultMap(int width, int height){
    int sideLength = mapSideLength;
    int i = 0;
    for( i = 0; i < width; i++){
        int j = 0;
        for(j = 0; j < height; j++){
            map[i][j] = UNDISCOVERED;
        }
    }

    currentPosition.x = startingX;
    currentPosition.y = startingY;

    putAtPosition(currentPosition, ROBOT);

}


void putAtPosition(coordinate pos, OBSTACLE obj){
    int xIndex = cmToIndex(pos.x);
    int yIndex = cmToIndex(pos.y);
    if(marginOfError > 0){
        int placeMaxX = min(xIndex + marginOfError, mapSideLength);
        int placeMinX = max(xIndex - marginOfError , 0);
        int placeMaxY = min(yIndex + marginOfError, mapSideLength / 2);
        int placeMinY = max(yIndex - marginOfError , 0);
        int i = placeMinX;
        for( i = placeMinX; i < placeMaxX; i++){
            int j = placeMinY;
            for(j = placeMinY; j < placeMaxY; j++){
                if( pow((i - xIndex),2) + pow((j - yIndex),2) <= pow(marginOfError, 2) ){

                        map[i][j] = obj;

                }

            }
        }
    } else {
        map[xIndex][yIndex] = obj;
    }
}

void scanNearby( int x, int y, int radius, oi_t* io){

    coordinate scanPosition;
    scanPosition.x = x;
    scanPosition.y = y;
    scanNearbyCoords(scanPosition, radius, io);

}




// Scan and put nearby object into map
void scanNearbyCoords( coordinate pos, int radius, oi_t* io){

    if ( currentPosition.x < pos.x){
        turn_right( io, 180);
    }



}


void scanInFront(oi_t * io){

    oi_setWheels(0,0);

   double servoPosition = 0;

   servo_move(servoPosition);

   int i = 0;

    for( i = 0; i < 180; i += 1){

        servo_move(i);

        double dist = 0;
        unsigned long raw_dis = adc_read();
        dist = adc_distance(raw_dis);
        double angle = (90 - i) * PI / 180.0;

        int calculatedX = currentPosition.x + dist * cos(angle);
        int calculatedY = currentPosition.y + dist * sin(angle);

            coordinate obstaclePos;
            obstaclePos.x = calculatedX;
            obstaclePos.y = calculatedY;

            int j = calculatedX;

            for( j = currentPosition.x; j < calculatedX; j += resolution){
                coordinate measuringPosition;
                measuringPosition.x = j;
                measuringPosition.y = currentPosition.y + (((double)calculatedY - currentPosition.y) / (calculatedX - currentPosition.x)) * (j - currentPosition.x);
                putAtPosition(measuringPosition, EMPTY);
            }

            if(dist < 300){
                putAtPosition( obstaclePos, TALL_OBJ);
            }


            putAtPosition( currentPosition, ROBOT);

            //printMap();

            timer_waitMillis(50);
    }

}

void changeRobotDirection(double newAngle){
    angle += newAngle;
}

void moveRobotOnMap(int distance_mm){

   int calculatedX = currentPosition.x + 10.0 * distance_mm * cos(angle);
   int calculatedY = currentPosition.y - 10.0 * distance_mm * sin(angle);

   currentPosition.x = calculatedX;
   currentPosition.y = calculatedY;

}

void shiftMap(MAPSIDE dir){

    if(dir == LEFT){
        int i = 0;
        for(i = 0; i < width; i++){
            int j = 0;
            for(j = 0; j < height; j++){
                if(i == width - 1){
                    map[i][j] = UNDISCOVERED;
                } else {
                    map[i][j] = map[max(min(i + 1, width), 0)][max(min(j, height), 0)];
                }
            }
        }
    }
    if(dir == RIGHT){
        int i = 1;
        for(i = 1; i < width; i++){
            int j = 0;
            for(j = 0; j < height; j++){
                map[max(min(i - 1, width), 0)][max(min(j, height), 0)] = map[i][j];
            }
        }
    }

}

// List of completed rooms
coordinate* findRooms(int width, int height){

    coordinate foundRooms[10];
    int idx = 0;


    int i = 0;

    int roomSizeInIndexes = roomSize / resolution;

    for( i = 0; i < width; i++){
        int j = 0;
        for(j = 0; j < height; j++){
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
void findPotentialRooms(int width, int height, coordinate foundRooms[10]){

    int idx = 0;


    int i = 0;

    int roomSizeInIndexes = mapSideLength / resolution;

    for( i = 0; i < width; i++){
        int j = 0;
        for(j = 0; j < height / 2; j++){
            if(map[i][j] == TALL_OBJ){
                coordinate searchPos;
                searchPos.x = i;
                searchPos.y = j;
                int corner = findWichCorner(searchPos, 2 << TALL_OBJ | 2 << UNDISCOVERED);

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

                if(corner != -1)
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
        if( (2 << map[pos.x][pos.y] & searchCondition) &&
            (2 << map[pos.x - roomSizeInIndexes][pos.y] & searchCondition) &&
            (2 << map[pos.x][pos.y - roomSizeInIndexes] & searchCondition) &&
            (2 << map[pos.x - roomSizeInIndexes][pos.y - roomSizeInIndexes] & searchCondition) ){
            return TOP_RIGHT; // Found room, passed in position was top-right
        }
    }

    if(((pos.x - roomSizeInIndexes) >= 0) && ((pos.y + roomSizeInIndexes < mapSideLength))){
        if( (2 << map[pos.x][pos.y] & searchCondition) &&
            (2 << map[pos.x - roomSizeInIndexes][pos.y] & searchCondition) &&
            (2 << map[pos.x][pos.y + roomSizeInIndexes] & searchCondition) &&
            (2 << map[pos.x - roomSizeInIndexes][pos.y + roomSizeInIndexes] & searchCondition) ){
            return BOTTOM_RIGHT; // Found room, passed in position was bottom-right
        }
    }

    if(((pos.x + roomSizeInIndexes) >= 0) && ((pos.y - roomSizeInIndexes >= 0))){
        if( (2 << map[pos.x][pos.y] & searchCondition) &&
            (2 << map[pos.x + roomSizeInIndexes][pos.y] & searchCondition) &&
            (2 << map[pos.x][pos.y - roomSizeInIndexes] & searchCondition) &&
            (2 << map[pos.x + roomSizeInIndexes][pos.y - roomSizeInIndexes] & searchCondition) ){
            return TOP_LEFT; // Found room, passed in position was top-left
        }
    }

    if(((pos.x + roomSizeInIndexes) >= 0) && ((pos.y + roomSizeInIndexes < mapSideLength))){
        if( (2 << map[pos.x][pos.y] & searchCondition) &&
            (2 << map[pos.x + roomSizeInIndexes][pos.y] & searchCondition) &&
            (2 << map[pos.x][pos.y + roomSizeInIndexes] & searchCondition) &&
            (2 << map[pos.x + roomSizeInIndexes][pos.y + roomSizeInIndexes] & searchCondition) ){
            return BOTTOM_LEFT; // Found room, passed in position was bottom-left
        }
    }

    return -1;
}

int cmToIndex(int num){
    return num / resolution;
}
//
//int indexToCM(int num){
//    return num * resolution;
//}
//
//int getMapSideLength(){
//    return getMapSideLength;
//}
