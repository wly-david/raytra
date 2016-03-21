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

mySurface* myCamera::findIntersection(const myRay &ray, const double min_t, double &distance,
									  const int ray_type,
									  BVH_Node * root){
	double current;
	mySurface * surface = root->getSurface();
	if (!surface->intersect(ray, current))
		return NULL;
	if (current >= distance)
		return NULL;
	if (root->getRight() != NULL) {
		mySurface * left_s = findIntersection(ray, min_t, distance, ray_type, root->getLeft());
		double right_d = distance;
		mySurface * right_s = findIntersection(ray, min_t, right_d, ray_type, root->getRight());
		if (right_d < distance) {
			distance = right_d;
			return right_s;
		}
		else {
			return left_s;
		}
	}
	if (render_model == 3 && root->getLeft() != NULL) {
		return findIntersection(ray, min_t, distance, ray_type, root->getLeft());
	}
	if (current > min_t && current < distance) {
		distance = current;
		return surface;
	}
	return NULL;
}

mySurface* myCamera::findIntersection(const myRay &ray, const double min_t, double &distance,
									  const int ray_type, 
									  std::vector<BVH_Node*> & nodes){
	mySurface* intersection = NULL;
	for(std::vector<BVH_Node*>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
		double current;
		mySurface * surface = NULL;
		if (render_model == 1) {
			surface = (*it)->getSurface();
			if (surface->intersect(ray, current))
			if (current > min_t && current < distance) {
				intersection = surface;
				distance = current;
				if (ray_type == SHADOW_RAY)
					break;
			}
		}
		else if (render_model == 0) {
			surface = (*it)->getSurface();
			if (surface->intersect(ray, current))
			if (current < distance) {
				surface = (*it)->getLeft()->getSurface();
				if (surface->intersect(ray, current))
				if (current > min_t && current < distance) {
					intersection = surface;
					distance = current;
					if (ray_type == SHADOW_RAY)
						break;
				}
			}
		}
	}
	return intersection;
}


mySurface* myCamera::findIntersection(const myRay &ray, const double min_t, double &distance,
									  const int ray_type,
									  std::vector< mySurface * > &planes){
	mySurface* intersection = NULL;
	for(std::vector<mySurface*>::iterator it = planes.begin(); it != planes.end(); ++it) {
		double current;
		if ((*it)->intersect(ray, current)) {
			if (current > min_t && current < distance) {
				intersection = *it;
				distance = current;
				if (ray_type == SHADOW_RAY)
					break;
			}
		}
	}
	return intersection;
}

myVector myCamera:: generateShading(const myRay &ray, const myPoint &lightPos, const mySurface *intersectedSurface,
									const myPoint &intersectPos, const myVector &norm, const myVector &color) {
		myRay lightRay(intersectPos, lightPos - intersectPos);
		bool shadowed = false;
#ifndef shadowoff
		double dis2light = (lightPos - intersectPos) * lightRay.getDir();
		double dis2obs = dis2light;
		mySurface* obstruction = (render_model < 2) ? 
			findIntersection(lightRay, 0.0001, dis2obs, SHADOW_RAY, nodes) : 
			findIntersection(lightRay, 0.0001, dis2obs, SHADOW_RAY, root);
		if (planes.size() > 0) {
			mySurface* plane = findIntersection(lightRay, 0.0001, dis2obs, SHADOW_RAY, planes);
			if (plane != NULL)
				obstruction = plane;
		}
		shadowed = (obstruction != NULL);
#endif
		if (!shadowed)
			return intersectedSurface->getMaterial()->getPhongShading(
				lightRay.getDir(),
				(-1) * ray.getDir(),
				norm,
				color);
		else
			return myVector(0,0,0);

}


myVector myCamera::recursive_L (const myRay &ray, double min_t, double max_t,
								int recurse_limit, int ray_type) {
	if (recurse_limit == 0)
		return myVector(0,0,0);
	
	double distance = max_t;
	mySurface* intersectedSurface = (render_model < 2) ? 
		findIntersection(ray, min_t, distance, ray_type, nodes) : 
		findIntersection(ray, min_t, distance, ray_type, root);
	if (planes.size() > 0) {
		mySurface* plane = findIntersection(ray, min_t, distance, ray_type, planes);
		if (plane != NULL)
			intersectedSurface = plane;
	}
	if (intersectedSurface == NULL)
		return myVector(0,0,0);	

	myPoint intersectPos = ray.getOrigin() + distance * ray.getDir();
	myVector norm = intersectedSurface->getNorm(intersectPos);
	if (norm * ray.getDir() > 0)
		return myVector(0,0,0);	

	myVector color(0,0,0);
	for(std::vector<p_light*>::iterator it = PLights.begin(); it != PLights.end(); ++it) {
		color += generateShading(ray, (*it)->getPos(), intersectedSurface, intersectPos, norm, (*it)->getColor());
	}
	for(std::vector<s_light*>::iterator it = SLights.begin(); it != SLights.end(); ++it) {
		myVector tmp(0,0,0);
		int N = shadow_num * shadow_num;
		for(int p = 0; p < shadow_num; p ++)
		for (int q = 0; q < shadow_num; q ++) {
			double i = (p + rand() / double(RAND_MAX)) / shadow_num;
			double j = (q + rand() / double(RAND_MAX)) / shadow_num;
			tmp += generateShading(ray, (*it)->getPos(i, j), intersectedSurface, intersectPos, norm, (*it)->getColor());
		}
		tmp = 1.0 / N  * tmp;
		color += tmp;
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
			recurse_limit - 1, REFLECTION_RAY);

		myVector km = intersectedSurface->getMaterial()->getRefl();
		color = color + myVector(km[0] * reflLight[0], km[1] * reflLight[1], km[2] * reflLight[2]);
	}
	return color;
}

void myCamera::renderScene () {
	
	// std::cout << "rendering";
	//int printProgress = ny * nx / 10.0;
	for(int j = 0; j < ny; j ++) {
		for(int i = 0; i < nx; i ++) {
			
	// print one of these for every 1/10th of the image:
	//if ((j * nx + i) % printProgress == 0)
	// std::cout << ".";
			myVector color(0,0,0);
			int N = primary_num * primary_num;
			for(int p = 0; p < primary_num; p ++)
				for (int q = 0; q < primary_num; q ++) {
					myRay rayList = generateRay(i + (p + rand() / double(RAND_MAX)) / primary_num, j + (q + rand() / double(RAND_MAX)) / primary_num);
					color += recursive_L(rayList, 0, std::numeric_limits<double>::infinity(), REFL_TIMES, CAMERA_RAY);
				} 
			setPixel (i, j, color[0]/ N, color[1] / N, color[2] / N);
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