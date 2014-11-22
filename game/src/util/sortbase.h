#ifndef SORTBASE_H
#define SORTBASE_H

#include <vector>
#include <iostream>

template<typename T>
class SortBase : public std::vector<int> {
protected:
	T * data;
public:
	SortBase(std::vector<T> & v_in) : data(v_in.data()) {
		for( int i=0; i<v_in.size(); i++ )
			this->push_back(i);
	}
	virtual bool operator() (int i, int j) = 0;
};

template<typename T>
class SortX : public SortBase<T> {
public:
	SortX(std::vector<T> & v_in) : SortBase<T>(v_in) { }
	virtual bool operator() (int i, int j) {
		return this->data[i].x < this->data[j].x;	//Tells compiler that the variables rely explicitly on the instantiation of the parent.
	}
};

template<typename T>
class SortY : public SortBase<T> {
public:
	SortY(std::vector<T> v_in) : SortBase<T>(v_in) {}
	virtual bool operator() (int i, int j) {
		return this->data[i].y < this->data[j].y;
	}
};

template<typename T>
class SortZ : public SortBase<T> {
public:
	SortZ(std::vector<T> v_in) : SortBase<T>(v_in) {}
	virtual bool operator() (int i, int j) {
		return this->data[i].z < this->data[j].z;
	}
};

#endif
