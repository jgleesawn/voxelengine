#include "objmodel.h"

void ObjModel::compress() {
	std::vector<bool> used;
	std::map<int, int> m;
	for( int i=0; i<ibo.size(); i++ ) {
		if( ibo[i] >= used.size() )
			used.resize(ibo[i]+1);
		used[ibo[i]] = 1;
		m[ibo[i]] = ibo[i];
	}

	
	used.push_back(0);
	used.push_back(0);
	int pos = 0;
	int count = 0;
	std::map<int, int>::iterator it = m.end();
	it--;
	while( it->first > pos ) {
		while( used[pos] )
			pos++;
		it->second = pos;
		used[pos] = 1;
		used[it->first] = 0;
		while( used[pos] )
			pos++;
		it--;
	}
	for( int i=0; i<ibo.size(); i++ ) {
		ibo[i] = m[ibo[i]];
	}
	int max = -1;
	for( it = m.begin(); it != m.end(); it++ ) {
		vbo[it->second] = vbo[it->first];
		if( it->first > max )
			max = it->first;
	}
	vbo.resize(max+1);
	vbo.shrink_to_fit();	
}



