#pragma once
#include "mySurface.h"
#include "mySphere.h"
class myPlane :
	public mySurface
{
	myVector Norm; // normal
	double d; // scalar
public:
	myPlane(void) { }
	myPlane(const myVector dir, const double dis) {
		Norm = dir;
		Norm.normalize();
		d = dis;
	}
	
	virtual bool intersect(const myRay &);
	virtual double minIntersectPos(const myRay &);
	virtual myVector getNorm(const myPoint &);

	/*myPlane(const mySphere sphere, const myPoint pos) {
		this->setMaterial(sphere.getMaterial());
		Norm = pos - sphere.getOrigin();
		Norm.normalize();
		d = (pos - myPoint(0, 0, 0)) * Norm;
	}*/
	~myPlane(void) { }
};

