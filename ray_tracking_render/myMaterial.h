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
	myVector getRefl() const{
		return refl;
	}
	double getPhong() const{
		return r;
	}

	myVector getPhongShading(
    const myVector &L,    // unit vector to the light
    const myVector &e,    // unit vector to the viewer
    const myVector &N,    // unit surface normal
    const myVector &L_color) const{
		myVector ans(0,0,0);
		double n_l = L * N;
		if (n_l > 0) {
			myVector diffRes (diff[0] * L_color[0], diff[1] * L_color[1], diff[2] * L_color[2]);
			ans += n_l * diffRes;
		}
		myVector h = L + e;
		if (h.length() > 0) {
			double n_h = h.normalize() * N;
			if (n_h > 0){
				n_h = pow(n_h, r);
				myVector specRes(spec[0] * L_color[0], spec[1] * L_color[1], spec[2] * L_color[2]);
				ans += n_h * specRes;
			}
		}
		return ans;
	}
	
	~myMaterial(void) { }
};

