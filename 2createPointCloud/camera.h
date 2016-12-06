#include "geo.h"
class Camera{
public:
	Camera(int idx);
	point loc;
	
	//direction is applied first.
	double direction;//radians
	double height;//radians. positive is up.
	//
	ray viewCenter;
	
private:
};
