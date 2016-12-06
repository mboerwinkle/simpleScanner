//unit is microns
#ifndef DEFS_H
#define DEFS_H
#define CAMERA_COUNT 1
#define CAMX {0, 0}
#define CAMY {0, 0}
//camera vectors dont exist anymore. way easier to just say "all cameras must point at the origin."
/*
|---------20cm---------|
|---10cm---|
===========R===========B
*/
#define LIGHT_COUNT 2
#define RED 0x00FF0000
#define BLUE 0x000000FF
#define GREEN 0x0000FF00
extern uint32_t LIGHT_COLOR[LIGHT_COUNT];//red on the end, blue at the middle
#define LIGHTX {0, 0}
#define LIGHTY {0, 0}
#define LIGHTZ {100000, 200000}
#endif
