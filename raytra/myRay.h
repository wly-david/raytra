#pragma once
#include"myPoint.h"
#include"myVector.h"
class myRay
{
public:
	myPoint origin;
	myVector dir;
	myRay(void);
	myRay(myPoint p, myVector v);
	myRay(float, float, float, float, float, float);
	~myRay(void);
};

