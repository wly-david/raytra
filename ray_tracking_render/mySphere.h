#pragma once
#include"myPoint.h"
#include"mySurface.h"
#include"myBBox.h"
class mySphere : public mySurface
{
	myPoint O; // origin
	double r; // radius
public:
	mySphere(void) { }

	mySphere (const myPoint & pos,const double radius) : O(pos), r(radius){ }
	
//	mySphere(double, double, double, double);
	virtual mySurface * generateBBox(){
		myPoint minP(O[0] - r - 0.0001, O[1] - r - 0.0001, O[2] - r - 0.0001);
		myPoint maxP(O[0] + r + 0.0001, O[1] + r + 0.0001, O[2] + r + 0.0001);
		myBBox * bbox = new myBBox(minP, maxP);
		bbox->setMaterial(this->getMaterial());
		return bbox;
	}
	virtual bool intersect(const myRay &, double &);
	virtual double minIntersectPos(const myRay &);
	virtual myVector getNorm(const myPoint &);

	virtual ~mySphere(void) { }
};

inline myVector mySphere::getNorm(const myPoint &pos) {
	return (pos - this->O).normalize();
}

inline double mySphere::minIntersectPos(const myRay & ray) {
	double root = (ray.getDir() * (ray.getOrigin() - this->O)) * (ray.getDir() * (ray.getOrigin() - this->O))
		- (ray.getDir() * ray.getDir()) * ((ray.getOrigin() - this->O) * (ray.getOrigin() - this->O) - this->r * this->r);
	
	if (root < 0) return -1;
	
	double min_d = (-1) * ray.getDir() * (ray.getOrigin() - this->O) - sqrt(root);
	return (min_d > 0) ? min_d : -1;
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