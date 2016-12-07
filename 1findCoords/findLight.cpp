#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <Imlib2.h>
#include <string.h>
#include "../shared/defs.h"
void findBrightest(int* outx, int* outy, uint32_t* image, uint32_t color);
int findIdx(int myx, int myy);
int64_t getPower(uint32_t sample, uint32_t color);
int getRed(uint32_t sample);
int getGreen(uint32_t sample);
int getBlue(uint32_t sample);
void writeImage();

FILE *(out[CAMERA_COUNT]);
uint32_t* finalImage = NULL;
int x, y;
void findAndPrintLight(int cam, uint32_t* image, uint32_t color){
	int outx = 0, outy = 0;
	findBrightest(&outx, &outy, image, color);
	fprintf(out[cam], "%lf,%lf,\n", (double)outx*2/x-1, (double)outy*2/y-1);
}
void findBrightest(int* outx, int* outy, uint32_t* image, uint32_t color){
	int64_t highestPower = INT64_MIN;
	uint32_t test;
	for(int row = 0; row < y; row++){
		for(int column = 0; column < x; column++){
			test = (image[findIdx(column, row)]);
			if(getPower(test, color) > highestPower){
				highestPower = getPower(test, color);
				*outx = column;
				*outy = row;
			}
		}
	}
	if(finalImage == NULL){
		finalImage = (uint32_t*)calloc(x*y, sizeof(uint32_t));
		memcpy(finalImage, image, x*y*sizeof(uint32_t));
	}
	test = (image[findIdx(*outx, *outy)]);
	printf("R%d G%d B%d P%d\n", getRed(test), getGreen(test), getBlue(test), getPower(test, color));
	finalImage[findIdx(*outx, *outy)] = 0xFFFFFFFF^color;
	finalImage[findIdx((*outx+1)%x, *outy)] = 0xFFFFFFFF^color;
	finalImage[findIdx((*outx-1+x)%x, *outy)] = 0xFFFFFFFF^color;
	finalImage[findIdx(*outx, (*outy+1)%y)] = 0xFFFFFFFF^color;
	finalImage[findIdx(*outx, (*outy-1+y)%y)] = 0xFFFFFFFF^color;
}
void writeImage(){
	Imlib_Image out = imlib_create_image_using_data(x, y, finalImage);
	imlib_context_set_image(out);
	imlib_image_set_format("png");
	imlib_save_image("out.png");
	imlib_free_image();
	free(finalImage);
	finalImage = NULL;
}
int64_t getPower(uint32_t sample, uint32_t color){
	if(color == RED){
		return getRed(sample)-0.5*(getGreen(sample)+getBlue(sample));
	}else if(color == BLUE){
		return getBlue(sample)-0.5*(getGreen(sample)+getRed(sample));
	}else if(color == GREEN){
		return getGreen(sample)-0.5*(getRed(sample)+getBlue(sample));
	}else{
		puts("getPower Error");
		return -1;
	}
}
int getGreen(uint32_t sample){
	return (sample&GREEN)/0xFF;
}
int getRed(uint32_t sample){
	return (sample&RED)/0xFFFF;
}
int getBlue(uint32_t sample){
	return (sample&BLUE);
}
int findIdx(int myx, int myy){
	return myy*x+myx;
}
