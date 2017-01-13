#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "defs.h"
#include "camera.h"
#define FOV 1.05//60 degrees, in radians
#define VERT_FOV 0.70//40 degrees, in radians

typedef struct light{
//	double view[CAMERA_COUNT][2];//defined first, coordinates on screen ([-1,1],[-1,1])
	double vec[CAMERA_COUNT][3];//defined second, unit vectors defining where a webcam is guessing it is.
	int64_t loc[3] = {0,0,0};//defined last, coords in 3d space.
}light;

extern Camera cams[CAMERA_COUNT]; 
light lights[LIGHT_COUNT];

void norm(double* vec);
int findTotalOffset(light* targ, int x, int y, int z);

void calculateLights(){
	for(int light_idx = 0; light_idx < LIGHT_COUNT; light_idx++){
		light* currLight = &(lights[light_idx]);
		for(int cam_idx = 0; cam_idx < CAMERA_COUNT; cam_idx++){
//			printf("seen at %lf %lf\n", cams[cam_idx].lightPos[light_idx][0], cams[cam_idx].lightPos[light_idx][1]);
			double dirAngle = cams[cam_idx].lightPos[light_idx][0]*FOV;
			double dirPitch = cams[cam_idx].lightPos[light_idx][1]*VERT_FOV;
//			printf("%lf degrees rotation\n%lf degrees elevation\n", 180*dirAngle/M_PI, 180*dirPitch/M_PI);
			double tempVec[3] = {-(double)CAMX[cam_idx], -(double)CAMY[cam_idx], 0};
			if(tempVec[0] == 0) puts("please choose a y placement for the camera that is NOT 0.");//FIXME
			norm(tempVec);
			double currAngle = atan(tempVec[1]/tempVec[0]);
			if(tempVec[0] < 0) currAngle+=M_PI;
			double finalAngle = currAngle+dirAngle;
			tempVec[0] = cos(finalAngle)*cos(dirPitch);
			tempVec[1] = sin(finalAngle)*cos(dirPitch);
			tempVec[2] = sin(dirPitch);
			currLight->vec[cam_idx][0] = tempVec[0];
			currLight->vec[cam_idx][1] = tempVec[1];
			currLight->vec[cam_idx][2] = tempVec[2];
//			printf("final vector: %lf %lf %lf\n", tempVec[0], tempVec[1], tempVec[2]);
		}
		//find loc. This is a REALLY dumb way of doing it.
		int64_t *loc = currLight->loc;
/*
		loc[0] = 1;
		loc[1] = 1;
		loc[2] = 1;
		int bestx = 0, besty = 0, bestz = 0;*/
//replaced by:
		int bestx = loc[0]+1, besty = loc[1]+1, bestz = loc[2]+1;
//uses previous position to make guess.
		int iterations = 0;
		while(!(loc[0] == bestx && loc[1] == besty && loc[2] == bestz)){
			iterations++;
			loc[0] = bestx;
			loc[1] = besty;
			loc[2] = bestz;
			for(int x = loc[0]-1; x <= loc[0]+1; x++){
			for(int y = loc[1]-1; y <= loc[1]+1; y++){
			for(int z = loc[2]-1; z <= loc[2]+1; z++){
				if(findTotalOffset(currLight, bestx, besty, bestz) > findTotalOffset(currLight, x, y, z)){
//					printf("Better Found: %d %d %d\nBetter Offset: %d\n", x, y, z, findTotalOffset(currLight, x, y, z));
					bestx = x;
					besty = y;
					bestz = z;
				}
			}}}
		}
		//end of dumb code region (Relatively)
		printf("FINAL LOCATION %d: %ld %ld %ld\nOFFSET %d\nITERATIONS %d\n", light_idx, loc[0], loc[1], loc[2], findTotalOffset(currLight, bestx, besty, bestz), iterations);
	}
}
//finds the distance between vector v(xyz) and point xyz
double offset(double vx, double vy, double vz, int x, int y, int z){
	double nPV[3] = {(double)x, (double)y, (double)z};//normal point vector
	norm(nPV);//the vector that points the same direction as the xyz point.

	//double angle = acos(vx*nPV[0]+vy*nPV[1]+vz*nPV[2]);
	double angle = vx*nPV[0]+vy*nPV[1]+vz*nPV[2];
	//double ret = fabs(sin(angle)*sqrt(x*x+y*y+z*z));
	double ret = fabs((1-angle*angle)*(x*x+y*y+z*z));

//	printf("%lf %lf %lf\nOFFSET: %lf\n", vx, vy, vz, ret);
	return ret;
}
int findTotalOffset(light* targ, int x, int y, int z){
//	printf("BEGINNING OF OFFSET CALCULATION\n");
	//the old method just added the offsets. This one adds the squares of the offsets. tends towards smaller equal numbers.
	int ret = 0;
	for(int cam_idx = 0; cam_idx < CAMERA_COUNT; cam_idx++){
//		ret += pow(offset(targ->vec[cam_idx][0], targ->vec[cam_idx][1], targ->vec[cam_idx][2], x-CAMX[cam_idx], y-CAMY[cam_idx], z), 2);
		ret += offset(targ->vec[cam_idx][0], targ->vec[cam_idx][1], targ->vec[cam_idx][2], x-CAMX[cam_idx], y-CAMY[cam_idx], z);
	}
//	printf("END OF OFFSET CALCULATION\n");
	return ret;
}

void norm(double* vec){
	double dist = sqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]);
	vec[0]/=dist;
	vec[1]/=dist;
	vec[2]/=dist;
}
