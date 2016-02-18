#pragma once
#include<cmath>
class myVector
{
public:
	float x,y,z;
	myVector(void);
	myVector(float, float, float);
	myVector normalize(void);
	~myVector(void);
};

myVector crossProduct(myVector, myVector);
myVector operator* (const float scalar, const myVector &v);
float operator* (const myVector &v1, const myVector &v2);
myVector operator+ (const myVector &v1, const myVector &v2);