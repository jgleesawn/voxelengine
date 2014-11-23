#ifndef BVLEAF_H
#define BVLEAF_H

#include "bvnode.h"

#include <vector>
#include <glm/glm.hpp>


template<typename T>
class BVLeaf : public BVNode<T> {
protected:
public:
	std::vector<int> indices;
	BVLeaf() {}
	BVLeaf(T min_in, T max_in, BVNode<T> * parent_in = NULL) : BVNode<T>(min_in, max_in, parent_in) {}

	void setValues(const std::vector<T> &);
};


#endif


