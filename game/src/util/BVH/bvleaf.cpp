#include "bvleaf.h"

#include <glm/glm.hpp>


template<>
void BVLeaf<glm::vec4>::setValues() {
	if( items.size() )
		this->min = this->max = items[0];
	for( int i=0; i<items.size(); i++ ) {
		this->min = glm::min(items[i], this->min);
		this->max = glm::max(items[i], this->max);
	}
}



