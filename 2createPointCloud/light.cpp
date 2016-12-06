#include "stdio.h"
#include "stdlib.h"
#include "light.h"
#define DANGER_DISTANCE 1.0//this is the z component of the ray view vector... should be calibrated and work in sync with the 'correct' function...

void correct(int cam, int *x, int *y);

Light::Light(uint32_t color, double *x, double *y){
	this->color = color;
	for(int cam = 0; cam < CAMERA_COUNT; cam++){
		correct(cam, x, y);//this should be a function of a camera class... maybe once it is merged with step 0.
		bound[cam].origin[0] = CAMX[cam];
		bound[cam].origin[1] = CAMY[cam];
		bound[cam].origin[2] = 0;//all cameras are at the same z as the origin.
		
	}
	
}

void correct(int cam, int *x, int *y){
	return;
}
