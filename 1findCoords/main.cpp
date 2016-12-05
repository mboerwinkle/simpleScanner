#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../shared/defs.h"
extern uint32_t* loadImage(char *name, int *x, int *y);
bool processFrame(int frame);
void findAndPrintLight(uint32_t* image, uint32_t color);
extern void writeImage();

extern uint32_t* finalImage;
extern int x, y;
int main(){
	
	for(int frame = 0; processFrame(frame); frame++){
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
			return false;
		}
		for(int light_idx = 0; light_idx < LIGHT_COUNT; light_idx++){
			findAndPrintLight(image, LIGHT_COLOR[light_idx]);
		}
		printf("\n");
		free(image);
		writeImage();
		finalImage = NULL;
		getchar();
	}
	return true;
}
