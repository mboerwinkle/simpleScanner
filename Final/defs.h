//unit is millimeters
#ifndef DEFS_H
#define DEFS_H
#define CAMERA_COUNT 1
extern int32_t CAMX[2];
extern int32_t CAMY[2];
//camera vectors dont exist anymore. way easier to just say "all cameras must point at the origin."
#define LIGHT_COUNT 2
extern char LIGHT_COLOR[LIGHT_COUNT];//red on the end, blue at the middle
#endif
