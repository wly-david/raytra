#include "mySphere.h"
#include<cassert>

bool mySphere::intersect(const myRay & ray) {

	double root = (ray.getDir() * (ray.getOrigin() - this->O)) * (ray.getDir() * (ray.getOrigin() - this->O))
		- (ray.getDir() * ray.getDir()) * ((ray.getOrigin() - this->O) * (ray.getOrigin() - this->O) - this->r * this->r);
	return (root >= 0);
}

