#ifndef GLOBAL_VAR_H_
#define GLOBAL_VAR_H_

#define RESOLUTION 8
#define FIELD_SIZE 440
#define MAP_SIDE_LENGTH (FIELD_SIZE / RESOLUTION)
#define PI 3.14159265

extern int mapSideLength;
extern int width;
extern int height;
extern int map[MAP_SIDE_LENGTH][MAP_SIDE_LENGTH / 2];

#endif
