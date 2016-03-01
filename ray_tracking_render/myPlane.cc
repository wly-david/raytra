#include "myPlane.h"

bool myPlane::intersect(const myRay & ray) {
	if (-((ray.getOrigin() - myPoint(0,0,0)) * Norm + d) / (ray.getDir() * Norm) > 0)
		return true;
	else
		return false;
}
