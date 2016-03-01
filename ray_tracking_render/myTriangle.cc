#include "myTriangle.h"


bool myTriangle::intersect(const myRay & ray) {
	double dis = (a - ray.getOrigin()) * Norm / (ray.getDir() * Norm);
	if (dis <= 0)
		return false;
	myPoint x = ray.getOrigin() + dis * ray.getDir();
	if ((crossProduct(b - a, x - a) * Norm) < 0)
		return false;
	if ((crossProduct(c - b, x - b) * Norm) < 0)
		return false;
	if ((crossProduct(a - c, x - c) * Norm) < 0)
		return false;
	return true;
}