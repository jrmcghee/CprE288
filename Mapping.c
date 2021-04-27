#include "Mapping.h";

#define min(X,Y) ((X) < (Y) ? (X) : (Y))
#define max(X,Y) ((X) > (Y) ? (X) : (Y))

#define RESOLUTION 8
#define MARGIN_OF_ERROR 1
#define ROOM_SIZE 80
#define FIELD_SIZE 440
#define MAP_SIDE_LENGTH (FIELD_SIZE / RESOLUTION)
#define PI 3.14159265

int mapSideLength = MAP_SIDE_LENGTH;

coordinate currentPosition;
int startingX = -180 + 220;
int startingY = 0 + 220;

void initializeMap(){

    map = (char*)malloc(sizeof(char[MAP_SIDE_LENGTH][MAP_SIDE_LENGTH / 2]));

    int i = 0;
    for( i = 0; i < mapSideLength; i++){
        int j = 0;
        for(j = 0; j < mapSideLength; j++){
            *map[i][j] = UNDISCOVERED;
        }
    }
}

void setUpDefaultMap(){
    int sideLength = mapSideLength;
    int i = 0;
    for( i = 0; i < mapSideLength; i++){
        int j = 0;
        for(j = 0; j < mapSideLength / 2; j++){
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
    int placeMaxX = min(xIndex + MARGIN_OF_ERROR, MAP_SIDE_LENGTH);
    int placeMinX = max(xIndex - MARGIN_OF_ERROR , 0);
    int placeMaxY = min(yIndex + MARGIN_OF_ERROR, MAP_SIDE_LENGTH);
    int placeMinY = max(yIndex - MARGIN_OF_ERROR , 0);
    int i = placeMinX;
    for( i = placeMinX; i < placeMaxX; i++){
        int j = placeMinY;
        for(j = placeMinY; j < placeMaxY; j++){
            if( pow((i - RESOLUTION),2) + pow((j - RESOLUTION),2) <= pow(RESOLUTION, 2) ){
                map[i][j] = obj;
            }

        }
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

//void goToPosition(int x, int y, oi_t* io){
//    int distanceToMove = (x - currentPosition.x) * 10;
//    if( x < currentPosition.x ){
//        turn_right( io, 180);
//        distanceToMove *= -1;
//    }
//    move_forward(io, distanceToMove);
//
//    int distanceToMove = (x - currentPosition.x) * 10;
//    if( x < currentPosition.x ){
//        if( currentPosition.y < y ){
//            turn_right( io, 180);
//            distanceToMove *= -1;
//        } else {
//
//        }
//        turn_right( io, 180);
//        distanceToMove *= -1;
//    }
//    move_forward(io, distanceToMove);
//}
double convertADC(unsigned long ir_sensor){

    double result = ir_sensor;

    result = pow(result, -1.176);

    result *= 126106;

    return result;
}


void scanInFront(oi_t * io){

//    oi_setWheels(0,0);
//
//    double servoPosition = 0;
//
//    servo_move(servoPosition);
//
//    int i = 0;
//
//    for( i = 0; i < 180; i += 20){
//
//        servo_move(i);
//
//        double dist = 0;
//        unsigned long raw_dis = adc_read();
//        dist = convertADC(raw_dis);
//        double angle = (90 - i) * PI / 180.0;
//
//        if(dist < 60){
//
//            int calculatedX = dist * cos(angle);
//            int calculatedY = dist * sin(angle);
//
//            coordinate obstaclePos;
//            obstaclePos.x = calculatedX;
//            obstaclePos.y = calculatedY;
//
//            putAtPosition( obstaclePos, TALL_OBJ);
//
//        }
//
//        timer_waitMillis(400);
//    }

}

//Return running map of objects
void copyMap( char fromMap[mapSideLength][mapSideLength / 2]){
    int i = 0;
    for(i = 0; i < mapSideLength; i++){
        int j = 0;
        for(j = 0; j < mapSideLength / 2; j++){
            fromMap[i][j] = ((char)map[i][j]);
        }
    }
    //tempMap
}

// Set pre-defined map
void setMap(int preSetMap[mapSideLength][mapSideLength / 2]){
    int i = 0;
    for(i = 0; i < mapSideLength; i++){
        int j = 0;
        for(j = 0; j < mapSideLength / 2; j++){
            int valAtMap = (OBSTACLE)preSetMap[i][j];
            map[i][j] = valAtMap;
        }
    }
}

// List of completed rooms
coordinate* findRooms(){

    coordinate foundRooms[10];
    int idx = 0;


    int i = 0;

    int roomSizeInIndexes = ROOM_SIZE / RESOLUTION;

    for( i = 0; i < mapSideLength; i++){
        int j = 0;
        for(j = 0; j < mapSideLength / 2; j++){
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

    int roomSizeInIndexes = ROOM_SIZE / RESOLUTION;

    for( i = 0; i < mapSideLength; i++){
        int j = 0;
        for(j = 0; j < mapSideLength / 2; j++){
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

    int roomSizeInIndexes = ROOM_SIZE / RESOLUTION;

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

int cmToIndex(int num){
    return num / RESOLUTION;
}

int indexToCM(int num){
    return num * RESOLUTION;
}

int getMapSideLength(){
    return getMapSideLength;
}
