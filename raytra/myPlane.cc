#include "myPlane.h"
#define DBL_MAX         1.7976931348623158e+308 /* max value */

bool myPlane::intersect(const myRay & ray) {
	if (-((ray.getOrigin() - myPoint(0,0,0)) * Norm + d) / (ray.getDir() * Norm) > 0)
		return true;
	else
		return false;
}

double myPlane::minIntersectPos(const myRay & ray) {
	double dis = -((ray.getOrigin() - myPoint(0,0,0)) * Norm + d) / (ray.getDir() * Norm);
	return (dis > 0) ? dis : DBL_MAX;
}


myVector myPlane::getNorm(const myPoint &pos) {
	return Norm;
}