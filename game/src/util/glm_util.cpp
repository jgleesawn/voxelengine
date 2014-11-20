#include "glm_util.h"

void printv(const glm::vec3 & v) {
	for( int i=0; i<3; i++ )
		std::cout << v[i] << " ";
	std::cout << std::endl;
}
void printv(const glm::vec4 & v) {
	for( int i=0; i<4; i++ )
		std::cout << v[i] << " ";
	std::cout << std::endl;
}
void printq(const glm::quat & q) {
	for( int i=0; i<4; i++ )
		std::cout << q[i] << " ";
	std::cout << std::endl;
}
void printm( const glm::mat4 & m) {
	for( int i=0; i<4; i++ ) {
		for( int j=0; j<4; j++ )
			std::cout << m[i][j] << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
float sumv(const glm::vec4 & v) {
	float sum = 0;
	for( int i=0; i<4; i++ )
		sum += v[i];
	return sum;
}


