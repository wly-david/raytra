#include "myRay.h"


myRay::myRay(void)
{
}

myRay::myRay(myPoint p, myVector v) {
	this->origin = p;
	this->dir = v;
}
myRay::myRay(float x, float y, float z, float vx, float vy, float vz) {
	this->origin = myPoint(x, y, z);
	this->dir = myVector(vx, vy, vz);
}


myRay::~myRay(void)
{
}
