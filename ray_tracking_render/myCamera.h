#pragma once

#include "mySurface.h"
#include "p_light.h"
#include "s_light.h"
#include "ALight.h"
#include "BVH_Node.h"

#include <ImfRgbaFile.h>
#include <ImfStringAttribute.h>
#include <ImfMatrixAttribute.h>
#include <ImfArray.h>
#include <vector>

#ifndef REFL_TIMES
	#define REFL_TIMES 20
#endif
#define SHADOW_RAY 1
#define CAMERA_RAY 0
#define REFLECTION_RAY 2

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

	static const int render_model = 3;
	int primary_num, shadow_num;

	Array2D<Rgba> image;

public:
	// if film is centered
	myCamera(void) { }

	void init(myPoint, myVector, double, double, double, int, int);

	~myCamera(void) { }

    myRay generateRay (const double i, const double j);
    
	void renderScene (BVH_Node *, std::vector<BVH_Node*> &,
		std::vector< mySurface * > &, std::vector< p_light * > &, std::vector< s_light * > &, ALight *);
	
	mySurface* findIntersection(const myRay &, const double, double &, const int,
		std::vector<BVH_Node*> &);

	mySurface* findIntersection(const myRay &, const double, double &, const int,
		BVH_Node *);

	mySurface* findIntersection(const myRay &, const double, double &, const int, 
		std::vector< mySurface * > &);

	myVector recursive_L (const myRay &, double, double, int, int,
		BVH_Node *, std::vector<BVH_Node*> &,
		std::vector< mySurface * > &, std::vector< p_light * > &, std::vector< s_light * > &, ALight *);

    void writeImage (const char *sceneFile);
    
    void setPixel (int pix_x, int pix_y, float r, float g, float b) {
        Rgba &px = image[pix_y][pix_x];
        px.r = r;
        px.g = g;
        px.b = b;
        px.a = 1.0;
    }
	
    void setModel (int pn, int sn) {
		primary_num = pn;
		shadow_num = sn;
    }
};


inline myRay myCamera::generateRay (const double i, const double j) {
	double u = this->l + (this->r - this->l) * i / this->nx;
	double v = this->t - (this->t - this->b) * j / this->ny;
	myVector dir = -this->d * this->w + u * this->u + v * this->v;
	return myRay(this->eye, dir);
}