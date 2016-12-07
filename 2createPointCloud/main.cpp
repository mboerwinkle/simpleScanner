#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../shared/defs.h"
#define FOV 1.05//60 degrees, in radians
#define VERT_FOV 0.70//40 degrees, in radians
FILE *(in[CAMERA_COUNT]);

typedef struct light{
	double view[CAMERA_COUNT][2];//defined first, coordinates on screen ([-1,1],[-1,1])
	double vec[CAMERA_COUNT][3];//defined second, unit vectors defining where a webcam is guessing it is.
	int64_t loc[3];//defined last, coords in 3d space.
}light;

void norm(double* vec);
int findTotalOffset(light* targ, int x, int y, int z);

int main(){
	for(int cam = 0; cam < CAMERA_COUNT; cam++){
		char name[80];
		sprintf(name, "../shared/cam%d_lightPositions.csv", cam);
		in[cam] = fopen(name, "r");
	}
	bool moreData = true;
	while(moreData){
		light lights[LIGHT_COUNT];
		for(int light_idx = 0; light_idx < LIGHT_COUNT; light_idx++){
			printf("\n");
			light* currLight = &(lights[light_idx]);
			for(int cam_idx = 0; cam_idx < CAMERA_COUNT; cam_idx++){
				//find view
				if(EOF == fscanf(in[cam_idx], "%lf, %lf,", &(currLight->view[cam_idx][0]), &(currLight->view[cam_idx][1]))){
					moreData = false;
					break;
				}
				printf("Light at %lf %lf\n", (currLight->view[cam_idx][0]),(currLight->view[cam_idx][1]));
				//find vec
				double dirAngle = currLight->view[cam_idx][0]*0.5*FOV;
				double dirPitch = currLight->view[cam_idx][1]*0.5*VERT_FOV;
				printf("%lf degrees rotation\n%lf degrees elevation\n", 180*dirAngle/M_PI, 180*dirPitch/M_PI);
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
				printf("final vector: %lf %lf %lf\n", tempVec[0], tempVec[1], tempVec[2]);
			}
			if(!moreData) break;
			//find loc. This is a REALLY dumb way of doing it.
			int64_t *loc = currLight->loc;
			loc[0] = 1;
			loc[1] = 1;
			loc[2] = 1;
			int bestx = 0, besty = 0, bestz = 0;
			while(!(loc[0] == bestx && loc[1] == besty && loc[2] == bestz)){
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
			printf("FINAL LOCATION %d: %ld %ld %ld\nOFFSET %d\n", light_idx, loc[0], loc[1], loc[2], findTotalOffset(currLight, bestx, besty, bestz));
		}
	}
	for(int cam = 0; cam < CAMERA_COUNT; cam++){
		fclose(in[cam]);
	}
	return 0;
}
//finds the distance between vector v(xyz) and point xyz
double offset(double vx, double vy, double vz, int x, int y, int z){
	double nPV[3] = {(double)x, (double)y, (double)z};//normal point vector
	norm(nPV);//the vector that points the same direction as the xyz point.
	double angle = acos(vx*nPV[0]+vy*nPV[1]+vz*nPV[2]);
//	printf("ANGLE %lf\n", angle);
	double ret = fabs(sin(angle)*sqrt(x*x+y*y+z*z));
//	printf("%lf %lf %lf\nOFFSET: %lf\n", vx, vy, vz, ret);
	return ret;
}
int findTotalOffset(light* targ, int x, int y, int z){
//	printf("BEGINNING OF OFFSET CALCULATION\n");
	//the old method just added the offsets. This one adds the squares of the offsets. tends towards smaller equal numbers.
	int ret = 0;
	for(int cam_idx = 0; cam_idx < CAMERA_COUNT; cam_idx++){
		ret += pow(offset(targ->vec[cam_idx][0], targ->vec[cam_idx][1], targ->vec[cam_idx][2], x-CAMX[cam_idx], y-CAMY[cam_idx], z), 2);
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
