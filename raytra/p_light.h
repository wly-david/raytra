#pragma once
#include"myLight.h"
#include"myRay.h"
class p_light : public myLight
{
	myPoint position;
	myVector color; // [0..1] for each channel
	double intensity; // simple multiplier on color
public:
	p_light(void) { };

	p_light(const myPoint &p, const double r, const double g, const double b) {
		position = p;
		color = myVector(r,g,b);
		intensity = 1;
	}

	void setPos(const myPoint &p) {
		position = p;
	}
	void setColor(const double r, const double g, const double b) {
		color = myVector(r,g,b);
	}
	void setIntensity(const double I) {
		intensity = I;
	}
	
	myVector virtual getColor() const{
		return myVector(color);
	}

	myRay virtual genrateRay(myPoint object);

	~p_light(void) { };
};

