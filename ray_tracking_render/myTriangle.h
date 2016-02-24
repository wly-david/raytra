#pragma once
#include "mySurface.h"
class myTriangle :
	public mySurface
{
	myPoint a;
	myPoint b;
	myPoint c;
	myVector Norm;
public:
	myTriangle(void) { }
	myTriangle(const myPoint p1, const myPoint p2, const myPoint p3):a(p1), b(p2), c(p3) {
		myVector ab = p2 - p1;
		myVector ac = p3 - p1;
		Norm = crossProduct(ab,ac);
		Norm.normalize();
	}
	virtual bool intersect(const myRay &);
	virtual double minIntersectPos(const myRay &);
	virtual myVector getNorm(const myPoint &);

	~myTriangle(void) { }
};

