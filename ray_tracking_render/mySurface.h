#pragma once
#include"myRay.h"
#include"myMaterial.h"
class mySurface
{
protected:
	myMaterial * material;
public:
	mySurface(void) { }
	void setMaterial(const myMaterial & m) {
		this->material = new myMaterial(m);
	}
	myMaterial getMaterial() const {
		return myMaterial (*(this->material));
	}
	virtual bool intersect(const myRay &) = 0;
	virtual double minIntersectPos(const myRay &) = 0;
	virtual myVector getNorm(const myPoint &) = 0;
	virtual ~mySurface(void) { }
};

