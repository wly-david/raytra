#pragma once
#include "myVector.h"
class myPoint
{
public:
	float x,y,z;
	myPoint(void);
	myPoint(float, float, float);
	~myPoint(void);
};

myVector operator- (const myPoint &p1, const myPoint &p2);

