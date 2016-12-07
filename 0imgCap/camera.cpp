#include <stdio.h>
#include <stdlib.h>
#include "camera.h"

int cameraCount = 0;

Camera::Camera(){
	cam.open(cameraCount);
	cameraCount++;
	if(!cam.isOpened()){
		printf("Failed to open camera %d\n", cameraCount-1);
	}
}
void Camera::processFrame(){
	cam.retrieve(data);
}
void Camera::grabFrame(){
	cam.grab();
}
