#include <stdio.h>
#include <stdlib.h>
#include "camera.h"
#include "../shared/defs.h"
using namespace cv;
int main(){
	Camera cams[CAMERA_COUNT];
	int idx = 0;
	char name[80];
	while(true){
		for(int temp = 0; temp <  CAMERA_COUNT; temp++){
			cams[temp].grabFrame();
		}
		for(int temp = 0; temp <  CAMERA_COUNT; temp++){
			cams[temp].processFrame();
			sprintf(name, "../shared/imgs/cam%d_frame%d.png", temp, idx);
			GaussianBlur(cams[temp].data, cams[temp].data, Size(41,41), 0, 0);
			imwrite(name, cams[temp].data);
		}
		getchar();
		idx++;
	}
}
