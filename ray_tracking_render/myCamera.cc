#include "myCamera.h"
#include<limits>
void myCamera::init(myPoint p, myVector v, double d, double iw, double ih, int pw, int ph)
{
	this->eye = p;
	this->w = ((-1) * v).normalize();
	if (1.0 - fabs (w[1]) < .0001) {
		this->u = myVector(1,0,0);
		this->v = crossProduct(w, u);
	}
	else {
		this->u = crossProduct(v, myVector(0,1,0)).normalize();
		this->v = crossProduct(w, u).normalize();
	}
	this->d = d;
	l = -iw / 2;
	r = iw / 2;
	t = ih / 2;
	b = -ih / 2;
	this->nx = pw;
	this->ny = ph;

    // set up the image:
    // OpenEXR uses exceptions - catch and print on error

    try {
		image.resizeErase(ny, nx);    
    }
    catch (const std::exception &exc) {
        std::cerr << exc.what() << std::endl;
        return;
    }
}


mySurface* myCamera::findIntersection(const myRay &ray, const double min_t, const double max_t,
									  const int ray_type, double &distance, 
									  std::vector<BVH_Node*> & nodes, std::vector< mySurface * > &planes){
	mySurface* intersection = NULL;
	double min_d = max_t;
	for(std::vector<BVH_Node*>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
		double current;
		mySurface * surface = NULL;
		if (render_model == 1)
			surface = (*it)->getSurface();
		else if (render_model == 0)
			surface = (*it)->getLeft()->getSurface();
		if (surface->intersect(ray, current)) {
			if (current > min_t && current < min_d) {
				intersection = surface;
				min_d = current;
				if (ray_type == SHADOW_RAY)
					break;
			}
		}
	}
	for(std::vector<mySurface*>::iterator it = planes.begin(); it != planes.end(); ++it) {
		double current;
		if ((*it)->intersect(ray, current)) {
			if (current > min_t && current < min_d) {
				intersection = *it;
				min_d = current;
				if (ray_type == SHADOW_RAY)
					break;
			}
		}
	}
	distance = min_d;
	return intersection;
}

myVector myCamera::recursive_L (const myRay &ray, double min_t, double max_t,
								int recurse_limit, int ray_type, 
								std::vector<BVH_Node*> & nodes, std::vector< mySurface * > &planes,
								std::vector< myLight * > &lights, ALight * ambient) {
	if (recurse_limit == 0)
		return myVector(0,0,0);
	
	double distance = max_t;
	mySurface* intersectedSurface = findIntersection(ray, min_t, max_t, ray_type, distance, nodes, planes);
	if (intersectedSurface == NULL)
		return myVector(0,0,0);	

	myPoint intersectPos = ray.getOrigin() + distance * ray.getDir();
	myVector norm = intersectedSurface->getNorm(intersectPos);
	if (norm * ray.getDir() > 0)
		return myVector(0,0,0);	

	myVector color(0,0,0);
	for(std::vector<myLight*>::iterator it = lights.begin(); it != lights.end(); ++it) {
		myRay lightRay(intersectPos, ((*it)->getPos() - intersectPos));
		bool shadowed = false;
#ifndef shadowoff
		double dis2light = ((*it)->getPos() - intersectPos) * lightRay.getDir();
		double dis2obs;
		mySurface* obstruction =  findIntersection(lightRay, 0.0001, dis2light, SHADOW_RAY, dis2obs, nodes, planes);		
		shadowed = (obstruction != NULL);
#endif
		if (!shadowed){
			color = color + intersectedSurface->getMaterial()->getPhongShading(
				lightRay.getDir(),
				(-1) * ray.getDir(),
				norm,
				(*it)->getColor());
		}
	}
	if (ambient != NULL) {
		myVector kd = intersectedSurface->getMaterial()->getDiff();
		myVector l = ambient->getColor();
		color = color + myVector(kd[0] * l[0], kd[1] * l[1], kd[2] * l[2]);
	}
	if (intersectedSurface->getMaterial()->getRefl().length() != 0) {
		myVector reflDir = ray.getDir() + (-2) * (ray.getDir() * norm) * norm;
		myRay reflRay(intersectPos, reflDir);

		myVector reflLight = recursive_L(reflRay,
			0.0001, std::numeric_limits<double>::infinity(),
			recurse_limit - 1, REFLECTION_RAY, nodes, planes, lights, ambient);

		myVector km = intersectedSurface->getMaterial()->getRefl();
		color = color + myVector(km[0] * reflLight[0], km[1] * reflLight[1], km[2] * reflLight[2]);
	}
	return color;
}

void myCamera::renderScene (std::vector<BVH_Node*> & nodes, std::vector< mySurface * > &surfaces,
							std::vector< myLight * > &lights, ALight * ambient) {
	
    // std::cout << "rendering";    
    //int printProgress = ny * nx / 10.0;
	for(int j = 0; j < ny; j ++) {
		for(int i = 0; i < nx; i ++) {
			
            // print one of these for every 1/10th of the image:
            //if ((j * nx + i) % printProgress == 0)
            //    std::cout << ".";
			myRay ray = generateRay(i, j);
			myVector color = recursive_L(ray, 0, std::numeric_limits<double>::infinity(), REFL_TIMES, CAMERA_RAY, nodes, surfaces, lights, ambient);
			setPixel (i, j, color[0], color[1], color[2]);
		}
     }
}

void myCamera::writeImage (const char *sceneFile) {
	
    // write the image data as an openEXR file
    // OpenEXR uses exceptions - catch and print on error
    try {
		RgbaOutputFile file (sceneFile, nx, ny, WRITE_RGBA);
		file.setFrameBuffer (&image[0][0], 1, nx);
		file.writePixels (ny);
    }
    catch (const std::exception &exc) {
        std::cerr << exc.what() << std::endl;
        return;
    }
}