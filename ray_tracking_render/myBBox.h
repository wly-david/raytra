#pragma once
#include "mySurface.h"
#include"myPoint.h"
#include"myRay.h"
class myBBox :
	public mySurface
{
	myPoint min_, max_;
public:
	myBBox(void) { }
	myBBox(myPoint minP, myPoint maxP) :min_(minP), max_(maxP) { }
	virtual bool intersect(const myRay &, double & distance);
	virtual double minIntersectPos(const myRay &);
	virtual myVector getNorm(const myPoint &);
	~myBBox(void) { }
};

inline bool intersect(const myRay &, double & distance) {

}
inline double minIntersectPos(const myRay &) {

}

