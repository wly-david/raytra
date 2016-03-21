#pragma once
#include "mySurface.h"
#include"myBBox.h"
class myPlane :
	public mySurface
{
	myVector Norm; // normal
	double d; // scalar
	myPlane(const myPlane& that);
	myPlane& operator=(const myPlane& that);
public:
	myPlane(void) { }
	myPlane(const myVector dir, const double dis) : Norm(dir), d(dis) {
		Norm.normalize();
	}
	virtual bool intersect(const myRay &, double & distance);
	//virtual double minIntersectPos(const myRay &);
	myVector virtual getNorm (const myPoint &) const;
	virtual ~myPlane(void) { }
};
inline myVector myPlane::getNorm(const myPoint &pos) const {
	return Norm;
}
inline bool myPlane::intersect(const myRay & ray, double & distance) {
	if (ray.getDir() * Norm == 0)
		return false;
	double dis = -((ray.getOrigin() - myPoint(0,0,0)) * Norm + d) / (ray.getDir() * Norm);
	if (dis <= 0)
		return false;	
	distance = dis;
	return true;
}
/*
inline double myPlane::minIntersectPos(const myRay & ray) {
	if (ray.getDir() * Norm == 0)
		return -1;
	double dis = -((ray.getOrigin() - myPoint(0,0,0)) * Norm + d) / (ray.getDir() * Norm);
	return (dis > 0) ? dis : -1;
}*/