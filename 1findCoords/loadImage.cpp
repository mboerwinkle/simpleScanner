#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <Imlib2.h>
uint32_t* loadImage(char *name, int *x, int *y){
	Imlib_Image map = imlib_load_image(name);
	if(map == NULL) return NULL;
	imlib_context_set_image(map);
	*x = imlib_image_get_width();
	*y = imlib_image_get_height();
	uint32_t* image = (uint32_t*)calloc((*x)*(*y), sizeof(uint32_t));
	memcpy(image, imlib_image_get_data(), (*x)*(*y)*sizeof(uint32_t));
	imlib_free_image();
	return image;
}
