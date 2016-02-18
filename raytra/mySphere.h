#pragma once
#include"myPoint.h"
#include"mySurface.h"
class mySphere : public mySurface
{
	myPoint O; // origin
	double r; // radius
public:
	mySphere(void) { }

	mySphere (const myPoint & pos,const double radius) : O(pos), r(radius){ }
	
//	mySphere(double, double, double, double);
	virtual bool intersect(const myRay &);
	virtual double minIntersectPos(const myRay &);
	virtual myVector getNorm(const myPoint &);

	~mySphere(void) { }
};

