#include <stdio.h>
#include <stdlib.h>
#include "camera.h"

Camera::Camera(){
	cam.open(0);
	if(!cam.isOpened()){
		puts("failed to open camera");
	}
}
void Camera::processFrame(){
	cam.retrieve(data);
}
void Camera::grabFrame(){
	cam.grab();
}
