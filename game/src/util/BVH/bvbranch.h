#ifndef BVBRANCH_H
#define BVBRANCH_H

#include "bvnode.h"
#include "bvleaf.h"

#include <glm/glm.hpp>
#include <algorithm>

template<typename T>
class BVBranch : public BVNode<T> {
public:
	BVNode<T> * child[2];

	void splitVolume(std::vector<T> &, int limit);
	BVBranch() { child[0] = NULL; child[1] = NULL; this->parent = NULL; }
	BVBranch(BVNode<T> * b_copy) : BVNode<T>(b_copy) { child[0] = NULL; child[1] = NULL; }
};


#endif


