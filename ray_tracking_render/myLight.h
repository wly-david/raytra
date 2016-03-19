#pragma once
#include"myPoint.h"
#include"myRay.h"
class myLight
{
public:
	myLight(void) { }
	
	myVector virtual getColor() const =0;
	myPoint virtual getPos(int i = 0, int j = 0) const =0;
	virtual ~myLight(void) { }
};

