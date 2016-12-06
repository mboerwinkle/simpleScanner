#include "geo.h"
#include "../shared/defs.h"
class Light{
public:
	Light(uint32_t color);
	ray bound[CAMERA_COUNT];
	uint32_t color;
private:
};
