#include "myVector.h"


myVector::myVector(void)
{
}

myVector::myVector(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

myVector myVector::normalize() {
	float len = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
	this->x = this->x / len;
	this->y = this->y / len;
	this->z = this->z / len;
	return myVector(this->x, this->y, this->z);
}
myVector::~myVector(void)
{
}

myVector crossProduct(myVector v1, myVector v2) {
	float s1, s2 ,s3;
	s1 = v1.y * v2.z - v1.z * v2.y;
	s2 = v1.z * v2.x - v1.x * v2.z;
	s3 = v1.x * v2.y - v1.y * v2.x;
	return myVector(s1, s2, s3);
}

myVector operator* (const float scalar, const myVector &v) {
	return myVector(scalar * v.x, scalar * v.y, scalar * v.z);
}

myVector operator+ (const myVector &v1, const myVector &v2) {
	return myVector(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

float operator* (const myVector &v1, const myVector &v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}