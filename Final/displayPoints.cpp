#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char* output = NULL;
char* getPointer(int x, int y);
int w;
void displayPoints(int width, int height, int pointCount, double* x, double* y, double* z){
	w = width;
	int size = width*height*2+height*3;
	if(output == NULL){
		output = (char*)malloc(size);//two blocks plus empty down the middleand \n terminators
	}
	memset(output, '.', size);
	for(int temp = 0; temp < height; temp++){
		*(getPointer(width*2+2, temp)) = '\n';
		*(getPointer(width*2+1, temp)) = '|';
		*(getPointer(width, temp)) = '|';
	}
	for(int temp = 0; temp < pointCount; temp++){
		*(getPointer(x[temp]*width, z[temp]*height)) = temp+97;
		*(getPointer(y[temp]*width+width, z[temp]*height)) = temp+97;
	}
//	printf("%s\n\n\n", output);
}
char* getPointer(int x, int y){
	return output+x+y*(w*2+2);
}
