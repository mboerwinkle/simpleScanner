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
	for(int x = 0; x < width; x++){
		for(int y = 0; y < height; y++){
			power = getPower(x, y, c);
			if(power > highestPower){
				highestPower = power;
				lightPosInt[light_idx][0] = x;
				lightPosInt[light_idx][1] = y;
				lightPos[light_idx][0] = x/width+0.5;
				lightPos[light_idx][1] = y/height+0.5;
			}
		}
	}
	if(c == 'r'){
		drawCross(lightPosInt[light_idx][0], lightPosInt[light_idx][1], 0, 255, 255);
	}else if(c == 'g'){
		drawCross(lightPosInt[light_idx][0], lightPosInt[light_idx][1], 255, 0, 255);
	}else if(c == 'b'){
		drawCross(lightPosInt[light_idx][0], lightPosInt[light_idx][1], 255, 255, 0);
	}
}

int Camera::getPower(int x, int y, char c){
        if(c == 'r'){
                return getRed(x, y)-0.5*(getGreen(x, y)+getBlue(x, y));
        }else if(c == 'b'){
                return getBlue(x, y)-0.5*(getGreen(x, y)+getRed(x, y));
        }else if(c == 'g'){
                return getGreen(x, y)-0.5*(getRed(x, y)+getBlue(x, y));
        }else{
                puts("getPower Error");
                return -1;
        }
}

void Camera::processFrame(){
	cam.retrieve(data);
	cam.retrieve(drawData);
}
void Camera::grabFrame(){
	cam.grab();
}
