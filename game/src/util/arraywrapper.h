#ifndef ARRAYWRAPPER_H
#define ARRAYWRAPPER_H

#include <vector>
#include <type_traits>

template<typename T, typename Ptr>
class ArrayWrapperStatic {
public:
	static Ptr split_array(T arr, int size, std::vector<int> dims) {
		int cur_size = size/dims.back();
		T* out = new T[cur_size];
		for( int i=0; i<cur_size; i++ )
			out[i] = &arr[i*dims.back()];
		dims.pop_back();
		return ArrayWrapperStatic<T*, Ptr>::split_array(out, cur_size, dims);
	}
	static void delete_wrapper(Ptr arr, int offset, const std::vector<int> & dims) {
		int count = dims[offset];
		for( int i=0; i<count; i++ ) {
			ArrayWrapperStatic<T, typename std::remove_pointer<Ptr>::type >::delete_wrapper( arr[i], offset+1, dims );
		}
		delete arr;
	}
};

template<typename T>
class ArrayWrapperStatic<T, T> {
public:
	static T split_array(T arr, int size, std::vector<int> dims) {
		return arr;
	}
	static void delete_wrapper(T arr, int offset, const std::vector<int> & dims) { }
};

template<typename T, typename Ptr>
class ArrayWrapper {
	Ptr ptr;
	std::vector<int> dims;
public:
	ArrayWrapper(T, std::vector<int> );
	~ArrayWrapper();
	typename std::remove_pointer<Ptr>::type operator[](int);
};
template<typename T, typename Ptr>
ArrayWrapper<T, Ptr>::ArrayWrapper(T arr, std::vector<int> dims_in) {
	dims = dims_in;
	int size = 1;
	for( int i=0; i<dims.size(); i++ )
		size *= dims[i];
	ptr = ArrayWrapperStatic<T,Ptr>::split_array(arr, size, dims);
}
template<typename T, typename Ptr>
ArrayWrapper<T, Ptr>::~ArrayWrapper() {
	int count = 1;
	for( int i=0; i<dims.size(); i++ )
		count *= dims[i];
	ArrayWrapperStatic<T, Ptr>::delete_wrapper(ptr, count, dims);
}
template<typename T, typename Ptr>
typename std::remove_pointer<Ptr>::type ArrayWrapper<T, Ptr>::operator[](int ind) {
	return ptr[ind];
}



#endif
