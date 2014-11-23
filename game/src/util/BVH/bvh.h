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
	BVH( const std::vector<T> &, int );
	
};

template<typename T>
BVH<T>::BVH( const std::vector<T> & v_in, int limit ) {
	num_per_leaf = limit;
	root = new BVBranch<T>();
	std::vector<int> indices;
	indices.resize(v_in.size());
	for( int i=0; i<indices.size(); i++ )
		indices[i] = i;
	((BVBranch<T> *)root)->splitVolume( v_in, indices, limit );
}

#endif
