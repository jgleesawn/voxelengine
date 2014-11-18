#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "objmodel.h"

//Currently shoves all objects within the model into a single vertex/index combination.
ObjModel loadObjFile(const std::string &);



#endif
