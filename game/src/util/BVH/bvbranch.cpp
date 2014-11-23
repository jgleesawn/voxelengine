#include "bvbranch.h"

#include <glm/glm.hpp>

template<>
void BVBranch<glm::vec4>::splitVolume(std::vector<glm::vec4> & v_in, int limit) {
	glm::vec4 mean(0.0f);
	min = max = v_in[0];
	for( int i=0; i<v_in.size(); i++ ) {
		min = glm::min(v_in[i], min);
		max = glm::max(v_in[i], max);
		mean += v_in[i];
	}
	mean /= (float)v_in.size();


	glm::vec4 diff = max-min;
	float max_diff = 0.0f;
	int max_axis = -1;
	for( int i=0; i<4; i++ ) {
		if( diff[i] > max_diff ) {
			max_diff = diff[i];
			max_axis = i;
		}
	}

	if( child[0] == NULL )
		child[0] = new BVLeaf<glm::vec4>;
	if( child[1] == NULL )
		child[1] = new BVLeaf<glm::vec4>;

	std::vector<glm::vec4>::iterator it;

	BVLeaf<glm::vec4> *child1, *child0;
	child1 = (BVLeaf<glm::vec4>*)child[1]; 
	child0 = (BVLeaf<glm::vec4>*)child[0];

	child1->items.reserve(v_in.size());
	child1->items.resize(v_in.size());
	it = std::copy_if(v_in.begin(), v_in.end(), child1->items.begin(),
			[&](glm::vec4 & v) {
				return v[max_axis] > mean[max_axis];
			});
	child1->items.resize(it-child1->items.begin());
	child1->items.shrink_to_fit();

	it = std::copy_if(v_in.begin(), v_in.end(), v_in.begin(),
		[&](glm::vec4 & v) {
			return v[max_axis] <= mean[max_axis];
		});
	v_in.resize(it-v_in.begin());
	v_in.shrink_to_fit();
	v_in.swap(child0->items);

	BVLeaf<glm::vec4> * temp;
	for( int i=0; i<2; i++ ) {
		temp = (BVLeaf<glm::vec4>*)child[i];
		if( temp->items.size() > limit ) {
			child[i] = new BVBranch(temp);
			((BVBranch<glm::vec4>*)child[i])->splitVolume(temp->items, limit);
			delete temp;
		} else {
			temp->setValues();
		}
	}
}



