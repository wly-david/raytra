#pragma once
#include"myPoint.h"
#include"mySurface.h"
class mySphere : public mySurface
{
	myPoint O; // origin
	float r; // radius
public:
	mySphere(void);
	mySphere(float, float, float, float);
	virtual bool intersect(myRay);
	~mySphere(void);
};

