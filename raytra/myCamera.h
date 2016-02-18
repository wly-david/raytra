#pragma once

#include "myRay.h"
#include "mySurface.h"
#include "myLight.h"
#include "mySphere.h"

#include <ImfRgbaFile.h>
#include <ImfStringAttribute.h>
#include <ImfMatrixAttribute.h>
#include <ImfArray.h>
#include <vector>


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
	myCamera(void);

	void init(myPoint, myVector, double, double, double, int, int);

	~myCamera(void);

	
    myRay generateRay (const int i, const int j);
    
	void renderScene (std::vector< mySurface * > &surfaces, std::vector< myLight * > &lights);

	
	mySurface* findIntersection(const myRay &ray, std::vector< mySurface * > &surfaces, double &distance);

    void writeImage (const char *sceneFile);
    
    void setPixel (int pix_x, int pix_y, float r, float g, float b) {
        Rgba &px = image[pix_y][pix_x];
        px.r = r;
        px.g = g;
        px.b = b;
        px.a = 1.0;
    }
};

