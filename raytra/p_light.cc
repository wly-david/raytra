#include "p_light.h"

myRay p_light::genrateRay(myPoint object){
	myVector dir = object - position;
	dir.normalize();
	return myRay(position, dir);
}