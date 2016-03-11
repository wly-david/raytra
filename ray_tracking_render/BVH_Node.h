#pragma once
#include"mySurface.h"
#include"myBBox.h"
class BVH_Node
{
	mySurface * surface;
	bool lastbbox;
	BVH_Node * left;
	BVH_Node * right;

public:
	BVH_Node(void) { }
	BVH_Node(mySurface * s) : surface(s), lastbbox(true), left(NULL), right(NULL) { }
	void setLeft(BVH_Node * l) { left = l; }
	void setRight(BVH_Node * r) { right = r; }
	BVH_Node * getLeft() { return left; }
	BVH_Node * getRight() { return right; }
	bool isLeaf() { return lastbbox; };
	mySurface * getSurface() { return surface; }
	~BVH_Node(void) {
		delete surface;
	}
};

