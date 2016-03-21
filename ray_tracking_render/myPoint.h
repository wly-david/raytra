#pragma once
#include "myVector.h"
class myPoint
{
	double xyz_[3];
public:
	myPoint(void) { }

	myPoint (double x, double y, double z) {
		xyz_[0] = x; xyz_[1] = y; xyz_[2] = z;
	}

	void set (double x, double y, double z) {
		xyz_[0] = x; xyz_[1] = y; xyz_[2] = z;
	}

	double operator[] (const int i) const;

	~myPoint(void) { }
	
	// subtracting one point from another returns a vector
	myVector operator- (const myPoint &p) const {
		return myVector (xyz_[0] - p.xyz_[0],
						 xyz_[1] - p.xyz_[1],
						 xyz_[2] - p.xyz_[2]);
	}
	// these two functions are used to allow us to read in
	// and print out (or save to a file). they are used by
	// the parser code.
	friend istream &operator>>(istream &is, myPoint &p) {
		return is >> p.xyz_[0] >> p.xyz_[1] >> p.xyz_[2];
	}
	
	friend ostream &operator<<(ostream &os, myPoint &p) {
		return os<<"<"<<p.xyz_[0]<<","<<p.xyz_[1]<<","<<p.xyz_[2]<<">";
	}
};

inline myPoint operator+ (const myPoint & p,const myVector & v) {
	return myPoint(p[0]+v[0], p[1]+v[1], p[2]+v[2]); 
}

inline double myPoint::operator[]
(const int i) const
{
	assert(i >= 0 && i < 3);
	return xyz_[i];
}