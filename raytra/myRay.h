#pragma once
#include"myPoint.h"
#include"myVector.h"
#include<vector>

class myRay
{
	myPoint origin;
	myVector dir;
public:
	myRay(void);

	myRay(const myPoint &p, const myVector &v);

//	myRay(double, double, double, double, double, double);
	
    // Get and set functions, for the direction and origin:
    void setDir (const myVector &dir)	{ this->dir = dir; }
    void setOrigin (const myPoint &pnt)	{ this->origin = pnt; }
    myVector getDir () const { return dir; }
    myPoint  getOrigin () const { return origin; }
	~myRay(void);
};

