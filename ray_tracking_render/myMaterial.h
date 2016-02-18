#pragma once
#include"myVector.h"
class myMaterial
{
	myVector diff,spec,refl;
	double r;
public:
	myMaterial(void) { }

	myMaterial( const myVector & diff, const myVector & spec, const myVector & refl, double r) {
		this->diff = diff;
		this->spec = spec;
		this->refl = refl;
		this->r = r;
	}
	void set ( const myVector & diff, const myVector & spec, const myVector & refl, double r) {
		this->diff = diff;
		this->spec = spec;
		this->refl = refl;
		this->r = r;
	}

	myVector getDiff() const{
		return diff;
	}
	myVector getSpec() const{
		return spec;
	}
	
	double getPhong() const{
		return r;
	}
	~myMaterial(void) { }
};

