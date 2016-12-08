#include <stdio.h>
#include <stdlib.h>
#include "camera.h"
#include "delay.h"
#include "defs.h"
using namespace cv;
#define FRAMERATE 5
int main(){
	startWindowThread();
	int cam_idx = 0;
	int periodic = 0;
	Camera cams[CAMERA_COUNT](&cam_idx);
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
			imshow(cams[temp].winName, cams[temp].data);
			printf("R%d G%d B%d\n", cams[temp].getRed(cams[temp].width*0.5, cams[temp].height*0.5), cams[temp].getGreen(cams[temp].width*0.5, cams[temp].height*0.5), cams[temp].getBlue(cams[temp].width*0.5, cams[temp].height*0.5));
		}
	}
}
