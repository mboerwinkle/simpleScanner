#include <stdio.h>
#include <stdlib.h>
#include "camera.h"

Camera::Camera(int *idx){
	this->idx = *idx;
	(*idx)++;
	sprintf(winName, "Camera Feed %d", this->idx);
	namedWindow(winName, CV_WINDOW_AUTOSIZE);
	cam.open(this->idx);
	if(!cam.isOpened()){
		printf("Failed to open camera %d\n", this->idx);
	}
	grabFrame();
	processFrame();
	width = data.size().width;
	height = data.size().height;
}
void Camera::drawCross(int x, int y, int r, int g, int b){
	for(int tempx = x-1; tempx <=x+1; tempx++){
	for(int tempy = y-1; tempy <=y+1; tempy++){
		drawData.at<cv::Vec3b>(tempy, tempx)[0] = b;
		drawData.at<cv::Vec3b>(tempy, tempx)[1] = g;
		drawData.at<cv::Vec3b>(tempy, tempx)[2] = r;
	}}
}
int Camera::getRed(int x, int y){
	return getComp(x, y, 'r');
}
int Camera::getGreen(int x, int y){
	return getComp(x, y, 'g');
}
int Camera::getBlue(int x, int y){
	return getComp(x, y, 'b');
}
int Camera::getComp(int x, int y, char c){
	int coloridx;
	if(c == 'r'){
		coloridx = 2;
	}else if(c == 'g'){
		coloridx = 1;
	}else if(c == 'b'){
		coloridx = 0;
	}
	return data.at<cv::Vec3b>(y, x)[coloridx];
}
void Camera::processFrame(){
	cam.retrieve(data);
	cam.retrieve(drawData);
}
void Camera::grabFrame(){
	cam.grab();
}
