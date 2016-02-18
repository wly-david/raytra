#include "myRay.h"


myRay::myRay(void)
{
}

myRay::myRay(const myPoint &p, const myVector &v) :	origin(p), dir(v) { }

/*
myRay::myRay(double x, double y, double z, double vx, double vy, double vz) {
	this->origin = myPoint(x, y, z);
	this->dir = myVector(vx, vy, vz);
}
*/


myRay::~myRay(void)
{
}
