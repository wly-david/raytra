#pragma once
#include"myPoint.h"
#include"myRay.h"
class myLight
{
public:
	myLight(void) { }
	
	myVector virtual getColor() const =0;
	myPoint virtual getPos() const =0;
	virtual ~myLight(void) { }
};

