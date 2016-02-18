#include "mySphere.h"
#include<cassert>

/*
mySphere::mySphere(double x, double y, double z, double r)
{
	this->O = myPoint(x, y, z);
	this->r = r;
}
*/

bool mySphere::intersect(const myRay & ray) {

	double root = (ray.getDir() * (ray.getOrigin() - this->O)) * (ray.getDir() * (ray.getOrigin() - this->O))
		- (ray.getDir() * ray.getDir()) * ((ray.getOrigin() - this->O) * (ray.getOrigin() - this->O) - this->r * this->r);
	return (root >= 0);
}

double mySphere::minIntersectPos(const myRay & ray) {

	double root = (ray.getDir() * (ray.getOrigin() - this->O)) * (ray.getDir() * (ray.getOrigin() - this->O))
		- (ray.getDir() * ray.getDir()) * ((ray.getOrigin() - this->O) * (ray.getOrigin() - this->O) - this->r * this->r);
	assert (root >= 0);
	
	double min_d = (-1) * ray.getDir() * (ray.getOrigin() - this->O) - sqrt(root);
	return min_d;
}


myVector mySphere::getNorm(const myPoint &pos) {
	return (pos - this->O).normalize();
}