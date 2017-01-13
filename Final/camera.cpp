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
		drawData.at<cv::Vec3b>(tempy, tempx)[0] = b;//coloridx here
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
	int coloridx;//coloridx here
	if(c == 'r'){
		coloridx = 2;
	}else if(c == 'g'){
		coloridx = 1;
	}else if(c == 'b'){
		coloridx = 0;
	}
	return data.at<cv::Vec3b>(y, x)[coloridx];
}

void Camera::findAndDrawLight(int light_idx){
	char c = LIGHT_COLOR[light_idx];
	int32_t highestPower = INT32_MIN;
	int32_t power;
	int step = 3;
	int bestx = 0, besty = 0;
	for(int x = 0; x < width; x+=step){
		for(int y = 0; y < height; y+=step){
			power = getPower(x, y, c);
			if(power > highestPower){
				bestx = x;
				besty = y;
				highestPower = power;
			}
		}
	}
	for(int x = bestx-step; x < bestx+step; x++){
		for(int y = besty-step; y < besty+step; y++){
			power = getPower(x, y, c);
			if(power > highestPower){
				bestx = x;
				besty = y;
				highestPower = power;
			}
		}
	}
	lightPosInt[light_idx][0] = bestx;
	lightPosInt[light_idx][1] = besty;
	lightPos[light_idx][0] = (double)bestx/width-0.5;
	lightPos[light_idx][1] = (double)besty/height-0.5;
	if(c == 'r'){
		drawCross(lightPosInt[light_idx][0], lightPosInt[light_idx][1], 0, 255, 255);
	}else if(c == 'g'){
		drawCross(lightPosInt[light_idx][0], lightPosInt[light_idx][1], 255, 0, 255);
	}else if(c == 'b'){
		drawCross(lightPosInt[light_idx][0], lightPosInt[light_idx][1], 255, 255, 0);
	}
}

int Camera::getPower(int x, int y, char c){
	int red = getRed(x, y);
	int blue = getBlue(x, y);
	int green = getGreen(x, y);
	int brightness = (red+blue+green)/3;//equivalent to getBrightness()
        if(c == 'r'){
                return (red-(0.5*(green+blue)))*brightness;
        }else if(c == 'b'){
                return (blue-(0.5*(green+red)))*brightness;
        }else if(c == 'g'){
                return (green-(0.5*(red+blue)))*brightness;
        }else{
                puts("getPower Error");
                return -1;
        }
}

int Camera::getBrightness(int x, int y){
	return (getRed(x, y)+getBlue(x, y)+getGreen(x, y))/3;
}

void Camera::processFrame(){
	cam.retrieve(data);
	cam.retrieve(drawData);
}
void Camera::grabFrame(){
	cam.grab();
}
