#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
struct timespec t = {.tv_sec=0};
struct timespec lastTime = {.tv_sec=0, .tv_nsec = 0};
struct timespec otherTime = {.tv_sec = 0, .tv_nsec=0};
void delay(int fps){
	clock_gettime(CLOCK_MONOTONIC, &otherTime);
	int32_t sleep = (int32_t)(1000000000/fps)-(otherTime.tv_nsec-lastTime.tv_nsec)-1000000000l*(otherTime.tv_sec-lastTime.tv_sec);
	if(sleep>0){
		t.tv_nsec = sleep;
		nanosleep(&t, NULL);
	}
	clock_gettime(CLOCK_MONOTONIC, &lastTime);
}
struct timespec elapsedTime = {.tv_sec = 0, .tv_nsec = 0};
void startTime(){
	clock_gettime(CLOCK_MONOTONIC, &elapsedTime);
}
long int endTime(){
	struct timespec endElapsedTime = {.tv_sec = 0, .tv_nsec = 0};
	clock_gettime(CLOCK_MONOTONIC, &endElapsedTime);
	return (endElapsedTime.tv_nsec-elapsedTime.tv_nsec)-1000000000l*(endElapsedTime.tv_sec-elapsedTime.tv_sec);
}
