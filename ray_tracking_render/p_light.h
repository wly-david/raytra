#pragma once
#include"myLight.h"
class p_light : public myLight
{
	myPoint position;
	myVector color; // [0..1] for each channel
	double intensity; // simple multiplier on color
public:
	p_light(void) { };

	p_light(const myPoint &p, const myVector &rgb) : position(p), color(rgb), intensity(1) { }

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
		return color;
	}
	myPoint virtual getPos(int i = 0, int j = 0) const {
		return position;
	}

	virtual ~p_light(void) { };
};

