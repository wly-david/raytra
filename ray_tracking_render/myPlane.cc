#include "myPlane.h"

bool myPlane::intersect(const myRay & ray) {
	if (-((ray.getOrigin() - myPoint(0,0,0)) * Norm + d) / (ray.getDir() * Norm) > 0)
		return true;
	else
		return false;
}

double myPlane::minIntersectPos(const myRay & ray) {
	if (ray.getDir() * Norm == 0)
		return -1;
	double dis = -((ray.getOrigin() - myPoint(0,0,0)) * Norm + d) / (ray.getDir() * Norm);
	return (dis > 0) ? dis : -1;
}


myVector myPlane::getNorm(const myPoint &pos) {
	return Norm;
}