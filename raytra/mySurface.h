#pragma once
#include"myRay.h"
class mySurface
{
public:
	mySurface(void);
	virtual bool intersect(myRay)=0;
	virtual ~mySurface(void);
};

