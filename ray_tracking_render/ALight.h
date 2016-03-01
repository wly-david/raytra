#pragma once
#include"myVector.h"
class ALight
{
	myVector color; // [0..1] for each channel
public:
	ALight(void) { }
	
	ALight( const double r, const double g, const double b) {
		color = myVector(r,g,b);
	}
	
	myVector getColor() const{
		return color;
	}
	
	~ALight(void) { }
};

