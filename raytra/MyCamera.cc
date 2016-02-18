#include "myCamera.h"


myCamera::myCamera(void)
{
}

myCamera::myCamera(float x, float y, float z, float vx, float vy, float vz, float d, float iw, float ih, int pw, int ph)
{
	this->eye = myPoint(x,y,z);
	
	if (vx == 0 && vz == 0) {
		this->u = myVector(-1,0,0);
		this->u = myVector(0,0,-1);
		this->w = myVector(-vx, -vy, -vz).normalize();
	}
	else {
		myVector dir = myVector(vx, vy, vz);
		this->u = crossProduct(dir, myVector(0,1,0)).normalize();
		this->v = crossProduct(this->u, dir).normalize();
		this->w = myVector(-vx, -vy, -vz).normalize();
	}
	this->d = d;
	l = -iw / 2;
	r = iw / 2;
	t = ih / 2;
	b = -ih / 2;
	this->nx = pw;
	this->ny = ph;
}


myCamera::~myCamera(void)
{
}
