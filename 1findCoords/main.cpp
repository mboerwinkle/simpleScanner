#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../shared/defs.h"
extern uint32_t* loadImage(char *name, int *x, int *y);
bool processFrame(int frame);
void findAndPrintLight(int cam, uint32_t* image, uint32_t color);
extern void writeImage();

extern uint32_t* finalImage;
extern int x, y;
extern FILE *(out[CAMERA_COUNT]);
int main(){
	for(int cam = 0; cam < CAMERA_COUNT; cam++){
		
		char outName[80];
		sprintf(outName, "../shared/cam%d_lightPositions.csv", cam);
		out[cam] = fopen(outName, "w");
	}
	for(int frame = 0; processFrame(frame); frame++){
	}
	for(int cam = 0; cam < CAMERA_COUNT; cam++){
		fclose(out[cam]);
	}
	return 0;
}

bool processFrame(int frame){
	char name[80];
	uint32_t* image;
	for(int cam_idx = 0; cam_idx < CAMERA_COUNT; cam_idx++){
		sprintf(name, "../shared/imgs/cam%d_frame%d.png", cam_idx, frame);
		image = loadImage(name, &x, &y);
		if(image == NULL){
			printf("image \"%s\" not found\n", name);
			return false;
		}
		puts("cycle");
		for(int light_idx = 0; light_idx < LIGHT_COUNT; light_idx++){
			findAndPrintLight(cam_idx, image, LIGHT_COLOR[light_idx]);
		}
		fprintf(out[cam_idx], "\n");
		free(image);
		writeImage();
		finalImage = NULL;
	}
	return true;
}
