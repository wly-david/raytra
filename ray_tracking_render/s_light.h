#pragma once
#include "myLight.h"
class s_light :
	public myLight
{
	double len;
	myPoint origin;
	myVector norm, u, v;
	myVector color;
public:
	s_light(void) { }
	s_light(const myPoint& p, const myVector& n, const myVector& u_, const double l, const myVector& rgb) {
		origin = p;
		len = l;
		color = rgb;
		norm = n;
		norm.normalize();
		u = u_;
		u.normalize();
		v = crossProduct(n, u_).normalize();
	}
	
	myVector virtual getColor() const{
		return color;
	}
	myPoint virtual getPos(double i = 0, double j = 0) const {
		return origin + i / len * u + j /len * v;
	}

	~s_light(void) { }
};

