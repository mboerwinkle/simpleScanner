#include <stdio.h>
#include <stdlib.h>
#include "camera.h"
#include "delay.h"
#include "defs.h"
using namespace cv;
#define FRAMERATE 30

extern void calculateLights();

void drawReticule(Camera& targ);

int cam_init_idx = 0;
Camera cams[CAMERA_COUNT](&cam_init_idx);

int main(){
	startWindowThread();
	int periodic = 0;
	while(true){
		delay(FRAMERATE);
		periodic++;
		if(periodic == FRAMERATE){
			periodic = 0;
			for(int temp = 0; temp < CAMERA_COUNT; temp++){
				resizeWindow(cams[temp].winName, cams[temp].width, cams[temp].height);
			}
		}
		for(int temp = 0; temp <  CAMERA_COUNT; temp++){
			cams[temp].grabFrame();
		}
		for(int temp = 0; temp <  CAMERA_COUNT; temp++){
			cams[temp].processFrame();
			GaussianBlur(cams[temp].data, cams[temp].data, Size(41,41), 0, 0);
			drawReticule(cams[temp]);
			for(int light_idx = 0; light_idx < LIGHT_COUNT; light_idx++){
				cams[temp].findAndDrawLight(light_idx);
			}
			imshow(cams[temp].winName, cams[temp].drawData);
//			printf("R%d G%d B%d\n", cams[temp].getRed(cams[temp].width*0.5, cams[temp].height*0.5), cams[temp].getGreen(cams[temp].width*0.5, cams[temp].height*0.5), cams[temp].getBlue(cams[temp].width*0.5, cams[temp].height*0.5));
		}
		calculateLights();
	}
}
void drawReticule(Camera& targ){
	targ.drawCross(targ.width*0.5, targ.height*0.5, 255, 255, 255);
	targ.drawCross(targ.width*0.25, targ.height*0.5, 255, 255, 255);
	targ.drawCross(targ.width*0.75, targ.height*0.5, 255, 255, 255);
	targ.drawCross(targ.width*0.5, targ.height*0.25, 255, 255, 255);
	targ.drawCross(targ.width*0.5, targ.height*0.75, 255, 255, 255);
}
