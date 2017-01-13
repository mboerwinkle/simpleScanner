#include <stdio.h>
#include <stdlib.h>
#include "camera.h"
#include "delay.h"
#include "defs.h"
using namespace cv;
#define FRAMERATE 60
#define TIME

extern void calculateLights();

void drawReticule(Camera& targ);

int cam_init_idx = 0;//1 if you want to ignore integrated cam
Camera cams[CAMERA_COUNT](&cam_init_idx);

int main(){
//Just to see how long zero time is...
//	startTime();
//	printf("%ld\n", endTime());
	startWindowThread();
	int periodic = 0;
	#ifdef TIME
	long int grabtime, processtime, blurtime, lightfindtime, showtime, lightcalctime;
	#endif
	while(true){
		delay(FRAMERATE);
		periodic++;
		if(periodic == FRAMERATE){
			periodic = 0;
			for(int temp = 0; temp < CAMERA_COUNT; temp++){
				resizeWindow(cams[temp].winName, cams[temp].width, cams[temp].height);
			}
		}
		#ifdef TIME
		//grab frame time
		startTime();
		#endif
		for(int temp = 0; temp <  CAMERA_COUNT; temp++){
			cams[temp].grabFrame();
		}
		#ifdef TIME
		grabtime = endTime();
		processtime = 0;
		blurtime = 0;
		lightfindtime = 0;
		showtime = 0;
		#endif
		for(int temp = 0; temp <  CAMERA_COUNT; temp++){
			startTime();//
			cams[temp].processFrame();
			processtime+=endTime();//
			startTime();//
			GaussianBlur(cams[temp].data, cams[temp].data, Size(81,81), 0, 0);
			GaussianBlur(cams[temp].drawData, cams[temp].drawData, Size(81,81), 0, 0);
			blurtime+=endTime();//
			drawReticule(cams[temp]);
			startTime();//
			for(int light_idx = 0; light_idx < LIGHT_COUNT; light_idx++){
				cams[temp].findAndDrawLight(light_idx);
			}
			lightfindtime+=endTime();//
			startTime();//
			imshow(cams[temp].winName, cams[temp].drawData);
			showtime+=endTime();//
//			printf("R%d G%d B%d\n", cams[temp].getRed(cams[temp].width*0.5, cams[temp].height*0.5), cams[temp].getGreen(cams[temp].width*0.5, cams[temp].height*0.5), cams[temp].getBlue(cams[temp].width*0.5, cams[temp].height*0.5));
		}
		#ifdef TIME
		startTime();
		#endif
		calculateLights();
		#ifdef TIME
		lightcalctime = endTime();
		printf("GRAB: %ld\nPROC: %ld\nBLUR: %ld\nFIND: %ld\nSHOW: %ld\nCALC: %ld\n", grabtime, processtime, blurtime, lightfindtime, showtime, lightcalctime);
		#endif
	}
}
void drawReticule(Camera& targ){
	targ.drawCross(targ.width*0.5, targ.height*0.5, 255, 255, 255);
	targ.drawCross(targ.width*0.25, targ.height*0.5, 255, 255, 255);
	targ.drawCross(targ.width*0.75, targ.height*0.5, 255, 255, 255);
	targ.drawCross(targ.width*0.5, targ.height*0.25, 255, 255, 255);
	targ.drawCross(targ.width*0.5, targ.height*0.75, 255, 255, 255);
}
