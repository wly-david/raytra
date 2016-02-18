#pragma once
#include"myPoint.h"
#include"myVector.h"
class myCamera
{
public:
	myPoint eye;
	float d; // focal length

	myVector u;
	myVector v;
	myVector w;

	int nx;
	int ny;

	float l, r, t, b; // or just w,h
	// if film is centered
	myCamera(void);
	myCamera(float, float, float, float, float, float, float, float, float, int, int);
	~myCamera(void);
};

