#include "objloader.h"

ObjModel loadObjFile(const std::string & fileName) {
	std::fstream fs(fileName);
	if( fs.is_open() );

	ObjModel om;	

	std::string line;
	std::stringstream sstr;

	glm::vec4 vertex;
	glm::ivec4 faceInd;

	std::string qualifier;

	int indOffset = 0;
	vertex[3] = 0;
	while( !fs.eof() ) {
		std::getline(fs, line);
		sstr.str(line);
		sstr.clear();
		
		sstr >> qualifier;
		if( qualifier == "o" ) {
			sstr >> line;
//			indOffset = om.vbo.size();
//Obj format has indoffset built in.
		} else if ( qualifier == "v" ) {
			sstr >> vertex[0] >> vertex[1] >> vertex[2];
			om.vbo.push_back(vertex);
		} else if ( qualifier == "f" ) {
			sstr >> faceInd[0] >> faceInd[1] >> faceInd[2] >> faceInd[3];
/*
			for( int i=4; i>=2; i-- )
				om.ibo.push_back(faceInd[i%4] - 1);
			for( int i=2; i>=0; i-- )
				om.ibo.push_back(faceInd[i] - 1);
*/

			for( int i=0; i<3; i++ )
				om.ibo.push_back(faceInd[i] - 1);
			for( int i=2; i<5; i++ )
				om.ibo.push_back(faceInd[i%4] - 1);

/*
			for( int i=0; i<3; i++ )
				om.ibo.push_back(faceInd[i]-1);
*/
		} else {
		}
	}
	return om;
}


