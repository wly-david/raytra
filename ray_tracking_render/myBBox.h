#pragma once
#include"myPoint.h"
#include"myRay.h"
#include"mySurface.h"
#include<limits>
class myBBox :
	public mySurface
{
	myPoint min_, max_;
	static const myVector norm[3];
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
	virtual double minIntersectPos(const myRay &);
	virtual myVector getNorm (const myPoint &) const;

	~myBBox(void) { }
};

inline bool myBBox::intersect(const myRay & ray, double & distance) {
	
#ifdef BB_DEBUGGING
	std::cout << min_ <<std::endl;
	std::cout << max_ << std::endl;
#endif
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
	
	double maxOfMin = - std::numeric_limits<double>::infinity();
	double minOfMax = std::numeric_limits<double>::infinity();
	for(int i = 0; i < 3; i ++) {
		if (tmin[i] > maxOfMin)
			maxOfMin = tmin[i];
		if (tmax[i] < minOfMax)
			minOfMax = tmax[i];	
	}
	if (minOfMax < maxOfMin)
		return false;
#ifdef BB_DEBUGGING
	std::cout << minOfMax <<" " << maxOfMin <<std::endl;
	std::cout <<std::endl;
	for (int i = 0; i < 3; i ++) {
	std::cout << tmin[i] <<" " << tmax[i] <<std::endl;
	}
#endif
	if (minOfMax <= 0)
		return false;
	distance = maxOfMin;
	return true;
}
inline double myBBox::minIntersectPos(const myRay &ray) {
	return -1;
}


inline myVector myBBox::getNorm(const myPoint &pos) const {
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