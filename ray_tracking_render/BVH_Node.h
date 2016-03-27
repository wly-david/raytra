#pragma once
#include"myBBox.h"
#include<vector>
class BVH_Node
{
	mySurface * surface;
	BVH_Node * left;
	BVH_Node * right;
	myPoint center, min_, max_;
	static int num;
	BVH_Node(const BVH_Node& that);
	BVH_Node& operator=(const BVH_Node& that);

public:
	BVH_Node(void) { }
	BVH_Node(mySurface * s) : surface(s), left(NULL), right(NULL) { }
	BVH_Node(myBBox * b) : surface(b), left(NULL), right(NULL) {
		min_ = b->getMin();
		max_= b->getMax();
		center = myPoint((min_[0] + max_[0])/2, (min_[1] + max_[1])/2, (min_[2] + max_[2])/2);
	}
	void setLeft(BVH_Node * l) { left = l; }
	void setRight(BVH_Node * r) { right = r; }
	BVH_Node * getLeft() const { return left; }
	BVH_Node * getRight() const { return right; }
	mySurface * getSurface() const { return surface; }
	~BVH_Node(void) {
		delete surface;
		delete left;
		delete right;
	}
	friend bool cmp_x(const BVH_Node *, const BVH_Node *);
	friend bool cmp_y(const BVH_Node *, const BVH_Node *);
	friend bool cmp_z(const BVH_Node *, const BVH_Node *);
	friend BVH_Node * createTree(std::vector<BVH_Node*> &, int, int, int);
	friend void removeTreeTest(BVH_Node* root);
	friend void removeTree(BVH_Node* root);
};

inline bool cmp_x(const BVH_Node * n1, const BVH_Node * n2) {
	return n1->center[0] < n2->center[0];
}
inline bool cmp_y(const BVH_Node * n1, const BVH_Node * n2) {
	return n1->center[1] < n2->center[1];
}
inline bool cmp_z(const BVH_Node * n1, const BVH_Node * n2) {
	return n1->center[2] < n2->center[2];
}