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
	mySurface * surface = root->surface;
	if (!surface->intersect(ray, current))
		return NULL;
	if (current >= distance)
		return NULL;
	if (root->right != NULL) {
		mySurface * left_s = findIntersection(ray, min_t, distance, ray_type, root->left);
		if (ray_type == SHADOW_RAY && left_s != NULL) {
			return left_s;
		}
		double right_d = distance;
		mySurface * right_s = findIntersection(ray, min_t, right_d, ray_type, root->right);
		if (right_d < distance) {
			distance = right_d;
			return right_s;
		}
		return left_s;
	}
	if (render_model == 3 && root->left != NULL) {
		return findIntersection(ray, min_t, distance, ray_type, root->left);
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
			surface = (*it)->surface;
			if (surface->intersect(ray, current))
			if (current > min_t && current < distance) {
				intersection = surface;
				distance = current;
				if (ray_type == SHADOW_RAY)
					break;
			}
		}
		else if (render_model == 0) {
			surface = (*it)->surface;
			if (surface->intersect(ray, current))
			if (current < distance) {
				surface = (*it)->left->surface;
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

myVector myCamera:: generateShading(const myRay &ray, const myRay &lightRay, const double dis2light,
									const myVector &norm, const myMaterial *material, const myVector &color) {
		
		bool shadowed = false;
#ifndef shadowoff
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
		if (!shadowed) {
			double weight = 1.0 / ((1 + dis2light) * (1 + dis2light));
			return material->getPhongShading(
				lightRay.getDir(),
				(-1) * ray.getDir(),
				norm,
				weight * color);
		}
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
	const myMaterial * material = intersectedSurface->getMaterial();
	if (norm * ray.getDir() > 0)
		return myVector(0,0,0);	

	myVector color(0,0,0);
	for(std::vector<p_light*>::iterator it = PLights.begin(); it != PLights.end(); ++it) {
		myPoint lightPos = (*it)->getPos();
		myRay lightRay(intersectPos, lightPos - intersectPos);
		double dis2light = (lightPos - intersectPos) * lightRay.getDir();
		color += generateShading(ray, lightRay, dis2light, norm, material, (*it)->getColor());
	}
	for(std::vector<s_light*>::iterator it = SLights.begin(); it != SLights.end(); ++it) {
		myVector tmp(0,0,0);
		int N = shadow_num * shadow_num;
		if (shadow_num ==1) {
			myPoint lightPos = (*it)->getPos();
			myRay lightRay(intersectPos, lightPos - intersectPos);
			double weight = -(lightRay.getDir() * (*it)->getNorm());
			if (weight <= 0)
				continue;
			double dis2light = (lightPos - intersectPos) * lightRay.getDir();
			tmp += generateShading(ray, lightRay, dis2light, norm, material, weight * (*it)->getColor());
		}
		else {
			for(int p = 0; p < shadow_num; p ++)
			for (int q = 0; q < shadow_num; q ++) {
				double i = (p + rand() / double(RAND_MAX)) / shadow_num;
				double j = (q + rand() / double(RAND_MAX)) / shadow_num;
				myPoint lightPos = (*it)->getPos(i, j);
				myRay lightRay(intersectPos, lightPos - intersectPos);
				double weight = -(lightRay.getDir() * (*it)->getNorm());
				if (weight <= 0)
					continue;
				double dis2light = (lightPos - intersectPos) * lightRay.getDir();
				tmp += generateShading(ray, lightRay, dis2light, norm, material, weight * (*it)->getColor());
			}
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
			if (primary_num == 1) {
				myRay rayList = generateRay(i + 0.5, j + 0.5);
				color += recursive_L(rayList, 0, std::numeric_limits<double>::infinity(), REFL_TIMES, CAMERA_RAY);
			}
			else {
				for(int p = 0; p < primary_num; p ++)
				for (int q = 0; q < primary_num; q ++) {
					myRay rayList = generateRay(i + (p + rand() / double(RAND_MAX)) / primary_num, j + (q + rand() / double(RAND_MAX)) / primary_num);
					color += recursive_L(rayList, 0, std::numeric_limits<double>::infinity(), REFL_TIMES, CAMERA_RAY);
				}
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