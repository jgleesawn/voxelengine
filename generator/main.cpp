#include <iostream>

#include "generator.h"

float plane( const float & x, const float & y, const float & z ) {
	return -y;
}
int main() {
	Generator g;
	Mesh * mesh = g.generate(glm::vec4(-15.3f, -15.3f, -15.3f, 0.0f), 32.0f, &plane);

	std::cout << mesh->indexCount << std::endl;
	for( int i=0; i<20; i++ )
		std::cout << mesh->ibo[i] << " ";
	std::cout << std::endl;

	delete mesh;
	return 0;
}
