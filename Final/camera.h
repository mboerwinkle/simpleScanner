#ifndef CAMERA_H
#define CAMERA_H
#include <opencv2/opencv.hpp>
#include "defs.h"
using namespace cv;
class Camera{
public:
	Camera(int *idx);
	int idx;
	double lightPos[LIGHT_COUNT][2];
	int lightPosInt[LIGHT_COUNT][2];
	char winName[80];
	//writes camera output to internal frame storage.
	void processFrame();
	//Quickly grabs the next frame...
	void grabFrame();
	Mat data;
	Mat drawData;
	int width, height;
	void drawCross(int x, int y, int r, int g, int b);
	int getRed(int x, int y);
	int getBlue(int x, int y);
	int getGreen(int x, int y);
	int getComp(int x, int y, char c);
	void findAndDrawLight(int light_idx);
	int getPower(int x, int y, char c);
private:
	cv::VideoCapture cam;
};
#endif
