#ifndef BVNODE_H
#define BVNODE_H

template<typename T>
class BVNode {
protected:
	BVNode<T> * parent;
	T min;
	T max;

public:
	BVNode() {}
	BVNode(T min_in, T max_in, BVNode<T> * parent_in = NULL) : min(min_in), max(max_in), parent(parent_in) { }
	BVNode(BVNode *);
};

template<typename T>
BVNode<T>::BVNode(BVNode<T> * b_copy) : min(b_copy->min), max(b_copy->max) { }



#endif


