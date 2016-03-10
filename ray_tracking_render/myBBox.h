#pragma once
#include"myPoint.h"
#include"myRay.h"
#include"mySurface.h"
class myBBox :
	public mySurface
{
	myPoint min_, max_;
	static const myVector norm[3];
public:
	myBBox(void) { }
	myBBox(myPoint minP, myPoint maxP) :min_(minP), max_(maxP) { }
	virtual mySurface * generateBBox() { return NULL; }
	virtual bool intersect(const myRay &, double & distance);
	virtual double minIntersectPos(const myRay &);
	virtual myVector getNorm(const myPoint &);
	~myBBox(void) { }
};

inline bool myBBox::intersect(const myRay & ray, double & distance) {
	double tmin[3], tmax[3];
	for (int i = 0; i < 3; i ++) {
		double a = 1 / ray.getDir()[i];
		if (a >=0) {
			tmin[i] = a * (this->min_[i] - ray.getOrigin()[i]);
			tmax[i] = a * (this->max_[i] - ray.getOrigin()[i]);
		}
		else {
			tmin[i] = a * (this->max_[i] - ray.getOrigin()[i]);
			tmax[i] = a * (this->min_[i] - ray.getOrigin()[i]);
		}
	}
	double maxOfMin = tmin[0];
	double minOfMax = tmax[0];
	for(int i = 1; i < 3; i ++) {
		if (tmin[i] > maxOfMin)
			maxOfMin = tmin[i];
		if (tmax[i] > minOfMax)
			minOfMax = tmax[i];	
	}
	if (minOfMax < maxOfMin)
		return false;
	if (minOfMax <= 0)
		return false;
		distance = (maxOfMin <=0) ? minOfMax : maxOfMin;
	return false;
}
inline double myBBox::minIntersectPos(const myRay &ray) {
	return -1;
}


inline myVector myBBox::getNorm(const myPoint &pos) {
	for(int i = 0; i < 3; i ++) {	
		if (fabs(pos[i] - min_[i]) < 0.0001)
			return (-1) * norm[i];
	}
	for(int i = 0; i < 3; i ++) {	
		if (fabs(pos[i] - max_[i]) < 0.0001)
			return norm[i];
	}
	return norm[0];
}