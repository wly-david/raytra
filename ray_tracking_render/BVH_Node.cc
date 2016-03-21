#include "BVH_Node.h"
#include<algorithm>

int BVH_Node::num = 0;
typedef bool (*CompFunc) (const BVH_Node *, const BVH_Node *);
CompFunc comf[3] = {cmp_x, cmp_y, cmp_z};

BVH_Node * createTree(std::vector<BVH_Node*> & nodes, int start, int end, int dim) {
	if (start == end - 1) {
		nodes[start]->num ++;
#ifdef BVH_DEBUGGING
	std::cout << "num" << nodes[start]->num <<std::endl;
	std::cout << "surfaces: " << end - start <<std::endl;
	std::cout << "center" << nodes[start]->center <<std::endl;
	std::cout << "min" << nodes[start]->min_ <<std::endl;
	std::cout << "max" << nodes[start]->max_ <<std::endl;
	std::cout <<std::endl;
#endif
		return nodes[start];
	}
	double minp[3], maxp[3];
	for(int i = 0; i < 3; i ++) {
		minp[i] = (*nodes.begin() + start)->min_[i];
		maxp[i] = (*nodes.begin() + start)->max_[i];
	}
	for(std::vector<BVH_Node*>::iterator it = nodes.begin() + start; it != nodes.begin() + end; ++it) {
		for(int i = 0; i < 3; i ++) {
			double tmp_min = (*it)->min_[i];
			double tmp_max = (*it)->max_[i];
			if (tmp_min < minp[i])
				minp[i] = tmp_min;
			if (tmp_max > maxp[i])
				maxp[i] = tmp_max;
		}
	}
	myBBox * bbox = new myBBox(myPoint(minp[0], minp[1], minp[2]), myPoint(maxp[0], maxp[1], maxp[2]));	
	BVH_Node * node = new BVH_Node(bbox);
#ifdef BVH_DEBUGGING
	std::cout << "num" << nodes[start]->num <<std::endl;
	std::cout << "surfaces: " << end - start <<std::endl;
	std::cout << "center" << node->center <<std::endl;
	std::cout << "min" << node->min_ <<std::endl;
	std::cout << "max" << node->max_ <<std::endl;
	std::cout <<std::endl;
#endif
	int mid = (start + end) / 2;
	std::nth_element(nodes.begin() + start, nodes.begin() + mid, nodes.begin() + end, comf[dim]);
	node->setLeft(createTree(nodes, start, mid, (dim + 1) % 3));
	node->setRight(createTree(nodes, mid, end, (dim + 1) % 3));
	return node;
}