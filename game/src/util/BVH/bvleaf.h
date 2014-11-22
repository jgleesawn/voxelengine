#ifndef BVLEAF_H
#define BVLEAF_H

#include "bvnode.h"

#include <vector>
#include <glm/glm.hpp>


template<typename T>
class BVLeaf : public BVNode<T> {
protected:
public:
	std::vector<T> items;
	BVLeaf() {}
	BVLeaf(T min_in, T max_in, BVNode<T> * parent_in = NULL) : BVNode<T>(min_in, max_in, parent_in) {}

	void setValues();
};

template<>
void BVLeaf<glm::vec4>::setValues() {
	if( items.size() )
		this->min = this->max = items[0];
	for( int i=0; i<items.size(); i++ ) {
		this->min = glm::min(items[i], this->min);
		this->max = glm::max(items[i], this->max);
	}
}

#endif


