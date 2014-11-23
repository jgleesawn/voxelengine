#include "bvbranch.h"

#include <glm/glm.hpp>

template<>
void BVBranch<glm::vec4>::splitVolume(const std::vector<glm::vec4> & v_in, std::vector<int> & ind_in, int limit) {
	glm::vec4 mean(0.0f);
	min = max = v_in[0];
	for( int i=0; i<ind_in.size(); i++ ) {
		min = glm::min(v_in[ind_in[i]], min);
		max = glm::max(v_in[ind_in[i]], max);
		mean += v_in[ind_in[i]];
	}
	mean /= (float)ind_in.size();


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

	std::vector<int>::iterator it;

	BVLeaf<glm::vec4> *child1, *child0;
	child1 = (BVLeaf<glm::vec4>*)child[1]; 
	child0 = (BVLeaf<glm::vec4>*)child[0];

	child1->indices.reserve(ind_in.size());
	child1->indices.resize(ind_in.size());
	it = std::copy_if(ind_in.begin(), ind_in.end(), child1->indices.begin(),
			[&](int ind) {
				return v_in[ind][max_axis] > mean[max_axis];
			});
	child1->indices.resize(it-child1->indices.begin());
	child1->indices.shrink_to_fit();

	it = std::copy_if(ind_in.begin(), ind_in.end(), ind_in.begin(),
		[&](int ind) {
			return v_in[ind][max_axis] <= mean[max_axis];
		});
	ind_in.resize(it-ind_in.begin());
	ind_in.shrink_to_fit();
	ind_in.swap(child0->indices);

	BVLeaf<glm::vec4> * temp;
	for( int i=0; i<2; i++ ) {
		temp = (BVLeaf<glm::vec4>*)child[i];
		if( temp->indices.size() > limit ) {
			child[i] = new BVBranch(temp);
			((BVBranch<glm::vec4>*)child[i])->splitVolume(v_in, temp->indices, limit);
			delete temp;
		} else {
			temp->setValues(v_in);
		}
	}
}



