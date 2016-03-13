#pragma once
#include"mySurface.h"
#include<limits>
class myBBox :
	public mySurface
{
	myPoint min_, max_;
	static const myVector norm[3];
    myBBox(const myBBox& that);
    myBBox& operator=(const myBBox& that);
public:
	myBBox(void) { }
	myBBox(myPoint minP, myPoint maxP) :min_(minP), max_(maxP) { }
	myPoint getMin() const {
		return min_;
	}
	myPoint getMax() const {
		return max_;
	}
	virtual bool intersect(const myRay &, double & distance);
	//virtual double minIntersectPos(const myRay &);
	virtual myVector getNorm (const myPoint &) const;

	~myBBox(void) { }
};

inline bool myBBox::intersect(const myRay & ray, double & distance) {	
	double tmin[3], tmax[3];
	for (int i = 0; i < 3; i ++) {
		if ( ray.getDir()[i] != 0 ) {
			double a = 1 / ray.getDir()[i];
			if (a >= 0) {
				tmin[i] = a * (this->min_[i] - ray.getOrigin()[i]);
				tmax[i] = a * (this->max_[i] - ray.getOrigin()[i]);
			}
			else {
				tmin[i] = a * (this->max_[i] - ray.getOrigin()[i]);
				tmax[i] = a * (this->min_[i] - ray.getOrigin()[i]);
			}
		}
		else {
			tmin[i] = - std::numeric_limits<double>::infinity();
			tmax[i] = std::numeric_limits<double>::infinity();
		}
	}
	double maxOfMin = tmin[0];
	double minOfMax = tmax[0];
	for(int i = 1; i < 3; i ++) {
		if (tmin[i] > maxOfMin)
			maxOfMin = tmin[i];
		if (tmax[i] < minOfMax)
			minOfMax = tmax[i];	
	}
	if (minOfMax < maxOfMin)
		return false;
#ifdef BB_DEBUGGING
	std::cout << maxOfMin <<" " << minOfMax <<std::endl;
	std::cout <<std::endl;
	for (int i = 0; i < 3; i ++)
		std::cout << tmin[i] <<"," << tmax[i] <<std::endl;
	std::cout <<std::endl;
#endif
	if (minOfMax <= 0)
		return false;
	distance = maxOfMin;
	return true;
}

inline myVector myBBox::getNorm(const myPoint &pos) const {
	for(int i = 0; i < 3; i ++) {	
		if (fabs(pos[i] - min_[i]) < 1e-10)
			return (-1) * norm[i];
	}
	for(int i = 0; i < 3; i ++) {	
		if (fabs(pos[i] - max_[i]) < 1e-10)
			return norm[i];
	}
	return norm[0];
}