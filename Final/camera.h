#ifndef CAMERA_H
#define CAMERA_H
#include <opencv2/opencv.hpp>
using namespace cv;
class Camera{
public:
	Camera(int *idx);
	int idx;
	char winName[80];
	//writes camera output to internal frame storage.
	void processFrame();
	//Quickly grabs the next frame...
	void grabFrame();
	Mat data;
	int width, height;
	int getRed(int x, int y);
	int getBlue(int x, int y);
	int getGreen(int x, int y);
	int getComp(int x, int y, char c);
private:
	cv::VideoCapture cam;
};
#endif