#include "myPoint.h"


myPoint::myPoint(void)
{
}

myPoint::myPoint(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

myPoint::~myPoint(void)
{
}


myVector operator- (const myPoint &p1, const myPoint &p2) {
	return myVector(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}