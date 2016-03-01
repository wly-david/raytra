#pragma once

#include "myRay.h"
#include "mySurface.h"
#include "myLight.h"
#include "mySphere.h"
#include "ALight.h"

#include <ImfRgbaFile.h>
#include <ImfStringAttribute.h>
#include <ImfMatrixAttribute.h>
#include <ImfArray.h>
#include <vector>

#ifndef REFL_TIMES
	#define REFL_TIMES 20
#endif
#define SHADOW_RAY 1
#define REGULAR_RAY 0

using namespace Imf;
using namespace Imath;

class myCamera
{
	myPoint eye;
	double d; // focal length

	myVector u;
	myVector v;
	myVector w;

	int nx;
	int ny;

	double l, r, t, b; // or just w,h

	Array2D<Rgba> image;

public:
	// if film is centered
	myCamera(void) { }

	void init(myPoint, myVector, double, double, double, int, int);

	~myCamera(void) { }

    myRay generateRay (const int i, const int j);
    
	void renderScene (std::vector< mySurface * > &, std::vector< myLight * > &, ALight *);

	mySurface* findIntersection(const myRay &, const double, const double, const int, double &, std::vector< mySurface * > &);

	myVector recursive_L (const myRay &, double, double, int, int, std::vector< mySurface * > &, std::vector< myLight * > &, ALight *);

    void writeImage (const char *sceneFile);
    
    void setPixel (int pix_x, int pix_y, float r, float g, float b) {
        Rgba &px = image[pix_y][pix_x];
        px.r = r;
        px.g = g;
        px.b = b;
        px.a = 1.0;
    }
};


inline myRay myCamera::generateRay (const int i, const int j) {
	double u = this->l + (this->r - this->l) * (i + 0.5) / this->nx;
	double v = this->t - (this->t - this->b) * (j + 0.5) / this->ny;
	myVector dir = -this->d * this->w + u * this->u + v * this->v;
	dir.normalize();
	return myRay(this->eye, dir);
}