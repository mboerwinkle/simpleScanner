#ifndef CAMERA_H
#define CAMERA_H
#include <opencv2/opencv.hpp>
using namespace cv;
class Camera{
public:
	Camera();
	//writes camera output to internal frame storage.
	void processFrame();
	//Quickly grabs the next frame...
	void grabFrame();
	Mat data;
private:
	cv::VideoCapture cam;
};
#endif
