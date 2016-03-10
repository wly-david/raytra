#pragma once
#include"myRay.h"
#include"myMaterial.h"
class mySurface
{
protected:
	myMaterial * material;
public:
	mySurface(void) { material = NULL;}
	void setMaterial(myMaterial * m) {
		this->material = m;
	}
	myMaterial * const getMaterial() const {
		return this->material;
	}
	virtual bool intersect(const myRay &, double & distance) = 0;
	virtual double minIntersectPos(const myRay &) = 0;
	virtual myVector getNorm(const myPoint &) = 0;
	virtual ~mySurface(void) { }
};

