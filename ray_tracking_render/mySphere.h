#pragma once
#include"mySurface.h"
#include"myBBox.h"
class mySphere : public mySurface
{
	myPoint O; // origin
	double r; // radius
	mySphere(const mySphere& that);
	mySphere& operator=(const mySphere& that);
public:
	mySphere(void) { }
	mySphere (const myPoint & pos,const double radius) : O(pos), r(radius){ }	
//	mySphere(double, double, double, double);
	virtual myBBox * generateBBox();
	virtual bool intersect(const myRay &, double &);
	//virtual double minIntersectPos(const myRay &);
	virtual myVector getNorm (const myPoint &) const;

	virtual ~mySphere(void) { }
};

inline myVector mySphere::getNorm(const myPoint &pos) const {
	return (pos - this->O).normalize();
}
inline bool mySphere::intersect(const myRay & ray, double & distance) {
	double root = (ray.getDir() * (ray.getOrigin() - this->O)) * (ray.getDir() * (ray.getOrigin() - this->O))
		- (ray.getDir() * ray.getDir()) * ((ray.getOrigin() - this->O) * (ray.getOrigin() - this->O) - this->r * this->r);
	if (root < 0) return false;
	double min_d = (-1) * ray.getDir() * (ray.getOrigin() - this->O) - sqrt(root);
	if (min_d <= 0)
		return false;	
	distance = min_d;
	return true;
}
inline myBBox * mySphere::generateBBox(){
	myPoint minP(O[0] - r - 1e-10, O[1] - r - 1e-10, O[2] - r - 1e-10);
	myPoint maxP(O[0] + r + 1e-10, O[1] + r + 1e-10, O[2] + r + 1e-10);
	myBBox * bbox = new myBBox(minP, maxP);
	bbox->setMaterial(this->getMaterial());
	return bbox;
}
/*
inline double mySphere::minIntersectPos(const myRay & ray) {
	double root = (ray.getDir() * (ray.getOrigin() - this->O)) * (ray.getDir() * (ray.getOrigin() - this->O))
		- (ray.getDir() * ray.getDir()) * ((ray.getOrigin() - this->O) * (ray.getOrigin() - this->O) - this->r * this->r);
	
	if (root < 0) return -1;
	
	double min_d = (-1) * ray.getDir() * (ray.getOrigin() - this->O) - sqrt(root);
	return (min_d > 0) ? min_d : -1;
}*/