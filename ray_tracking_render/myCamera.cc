#include "myCamera.h"


myCamera::myCamera(void)
{
}

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


myCamera::~myCamera(void)
{
}


myRay myCamera::generateRay (const int i, const int j) {
	double u = this->l + (this->r - this->l) * (i + 0.5) / this->nx;
	double v = this->t - (this->t - this->b) * (j + 0.5) / this->ny;
	myVector dir = -this->d * this->w + u * this->u + v * this->v;
	dir.normalize();
	return myRay(this->eye, dir);
}

mySurface* myCamera::findIntersection(const myRay &ray, std::vector< mySurface * > &surfaces, double &distance){
	mySurface* intersection = NULL;
	double min_d = DBL_MAX;
	for(std::vector<mySurface*>::iterator it = surfaces.begin(); it != surfaces.end(); ++it) {
		double current = (*it)->minIntersectPos(ray);
		if (current > 0 && current < min_d) {
			intersection = *it;
			min_d = current;
		}
	}
	distance = min_d;
	return intersection;
}


void myCamera::renderScene (std::vector< mySurface * > &surfaces, std::vector< myLight * > &lights) {
	
    // std::cout << "rendering";    
    //int printProgress = ny * nx / 10.0;

	for(int j = 0; j < ny; j ++) {
		for(int i = 0; i < nx; i ++) {
			
            // print one of these for every 1/10th of the image:
            //if ((j * nx + i) % printProgress == 0)
            //    std::cout << ".";

			myRay ray = generateRay(i, j);			
			setPixel (i, j, 0, 0, 0);

			double distance = DBL_MAX;
			mySurface* intersectedSurface = findIntersection(ray, surfaces, distance);
			
			if (intersectedSurface != NULL) {
				myPoint intersectPos = ray.getOrigin() + distance * ray.getDir();
				myVector norm = intersectedSurface->getNorm(intersectPos);
				if (norm * ray.getDir() > 0)
					continue;
				myVector color(0,0,0);
				for(std::vector<myLight*>::iterator it = lights.begin(); it != lights.end(); ++it) {
					myRay lightRay = (*it)->genrateRay(intersectPos);
					double dis2light = DBL_MAX;
					/*mySurface* obstruction = */ findIntersection(lightRay, surfaces, dis2light);
					double diff = fabs((intersectPos - lightRay.getOrigin()) * lightRay.getDir() - dis2light);
					if (/*obstruction == intersectedSurface && */ diff < 0.0001){
						color = color + intersectedSurface->getMaterial()->getPhongShading(
							(-1) * lightRay.getDir(),
							(-1) * ray.getDir(),
							norm,
							(*it)->getColor());
						
						/*double n_l = (-1) * lightRay.getDir() * norm > 0 ? (-1) * lightRay.getDir() * norm : 0;
						myVector diffRes = myVector(intersectedSurface->getMaterial()->getDiff()[0] * (*it)->getColor()[0],
							intersectedSurface->getMaterial()->getDiff()[1] * (*it)->getColor()[1],
							intersectedSurface->getMaterial()->getDiff()[2] * (*it)->getColor()[2]);
						color = color + n_l * diffRes;

						myVector h = (-1) * (lightRay.getDir() + ray.getDir()).normalize();
						double n_h = h * norm > 0 ? h * norm : 0;
						n_h = pow(n_h, intersectedSurface->getMaterial()->getPhong());
						myVector specRes = myVector(intersectedSurface->getMaterial()->getSpec()[0] * (*it)->getColor()[0],
							intersectedSurface->getMaterial()->getSpec()[1] * (*it)->getColor()[1],
							intersectedSurface->getMaterial()->getSpec()[2] * (*it)->getColor()[2]);
						color = color + n_h * specRes;*/
					}
				}
				setPixel(i, j, color[0], color[1], color[2]);
			}
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