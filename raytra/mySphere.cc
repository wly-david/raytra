#include "mySphere.h"


mySphere::mySphere(void)
{
}


mySphere::mySphere(float x, float y, float z, float r)
{
	this->O = myPoint(x, y, z);
	this->r = r;
}
mySphere::~mySphere(void)
{
}
bool mySphere::intersect(myRay ray) {

	float root = (ray.dir * (ray.origin - this->O)) * (ray.dir * (ray.origin - this->O))
		- (ray.dir * ray.dir) * ((ray.origin - this->O) * (ray.origin - this->O) - this->r * this->r);
	return (root >= 0);
}