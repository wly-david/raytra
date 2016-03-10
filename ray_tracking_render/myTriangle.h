#pragma once
#include "mySurface.h"
#include "myBBox.h"
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
		assert (ab.length () != 0.);
		assert (ac.length () != 0.);
		Norm = crossProduct(ab,ac);
		assert (Norm.length () != 0.0);
		Norm.normalize();
	}
	virtual mySurface * generateBBox(){
		double minp[3], maxp[3];
		for(int i = 0; i < 3; i ++) {
			minp[i] = a[i];
			maxp[i] = a[i];
			if (b[i] < minp[i])
				minp[i] = b[i];
			if (b[i] > minp[i])
				maxp[i] = b[i];
			if (c[i] < minp[i])
				minp[i] = b[i];
			if (c[i] > minp[i])
				maxp[i] = b[i];
		};
		
		myPoint minP(minp[0], minp[1], minp[2]);
		myPoint maxP(maxp[0], maxp[1], maxp[2]);
		myBBox * bbox = new myBBox(minP, maxP);
		bbox->setMaterial(this->getMaterial());
		return bbox;
	}
	virtual bool intersect(const myRay &, double &);
	virtual double minIntersectPos(const myRay &);
	virtual myVector getNorm(const myPoint &);

	virtual ~myTriangle(void) { }
};

inline myVector myTriangle::getNorm(const myPoint &pos) {
	return Norm;
}

inline double myTriangle::minIntersectPos(const myRay & ray) {
	double dis = (a - ray.getOrigin()) * Norm / (ray.getDir() * Norm);
	if (dis <= 0)
		return -1;
	myPoint x = ray.getOrigin() + dis * ray.getDir();
	if ((crossProduct(b - a, x - a) * Norm) < 0)
		return -1;
	if ((crossProduct(c - b, x - b) * Norm) < 0)
		return -1;
	if ((crossProduct(a - c, x - c) * Norm) < 0)
		return -1;
	return dis;
}

inline bool myTriangle::intersect(const myRay & ray, double & distance) {
	double dis = (a - ray.getOrigin()) * Norm / (ray.getDir() * Norm);
	if (dis <= 0)
		return false;
	myPoint x = ray.getOrigin() + dis * ray.getDir();
	if ((crossProduct(b - a, x - a) * Norm) < 0)
		return false;
	if ((crossProduct(c - b, x - b) * Norm) < 0)
		return false;
	if ((crossProduct(a - c, x - c) * Norm) < 0)
		return false;
	distance = dis;
	return true;
}