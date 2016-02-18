#pragma once
#include"myPoint.h"
#include"myRay.h"
class myLight
{
public:
	myLight(void) { }
	
	myRay virtual genrateRay(myPoint object)=0;
	
	myVector virtual getColor() const =0;
	~myLight(void) { }
};

