#pragma once
#include"mySurface.h"
#include"myBBox.h"
class BVH_Node
{
	mySurface * surface;
	bool leafnode;
	BVH_Node * left;
	BVH_Node * right;

public:
	BVH_Node(void) { }
	BVH_Node(mySurface * s, myBBox * b) : surface(s), leafnode(true), left(NULL), right(NULL) { }
	void setLeft(BVH_Node * l) { left = l; }
	void setRight(BVH_Node * r) { right = r; }

	~BVH_Node(void) {
		delete surface;
	}
};

