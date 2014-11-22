#ifndef BVH_H
#define BVH_H

#include "bvnode.h"
#include "bvbranch.h"
#include "bvleaf.h"

template<typename T>
class BVH {
	BVNode<T> * root;
	int num_per_leaf;
public:
	BVH( std::vector<T>, int );
	
};

template<typename T>
BVH<T>::BVH( std::vector<T> v_in, int limit ) {
	num_per_leaf = limit;
	root = new BVBranch<T>();
	((BVBranch<T> *)root)->splitVolume( v_in, limit );
}

#endif
