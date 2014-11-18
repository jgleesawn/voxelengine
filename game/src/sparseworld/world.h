#ifndef WORLD_H
#define WORLD_H

//Hack of a fix
#undef Success
#include <pcl/point_cloud.h>
#include <pcl/octree/octree.h>

#include <iostream>

#include "object/object.h"
#include "object/renderable.h"
#include "terrain.h"

class exposedOPCS : public pcl::octree::OctreePointCloudSearch<pcl::PointXYZ> {
public:
	exposedOPCS(float);
	void genKey(const pcl::PointXYZ & point, pcl::octree::OctreeKey & ) const;
	void genKey(const float & point_x, const float & point_y, const float & point_z, pcl::octree::OctreeKey & ) const;
};

class World {
public:
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud;
//	pcl::octree::OctreePointCloudSearch<pcl::PointXYZ> octree;
	exposedOPCS octree;

	std::vector<Object *> objects;
	std::vector<Renderable *> renObjs;
	Terrain terrain;

	std::vector<int> selection;

	int camera, focus;

public:
	World();
	~World();
	int addObject( Object *, const glm::vec4 & pos );
	int makeRenderable( int );
	void update();

	void MoveObject( Object *, const glm::vec4 & );
	
	void MoveFocusForward();
	void MoveFocusLeft();
	void MoveFocusRight();
	void MoveFocusBack();

	void RotFocusRight();
	void RotFocusLeft();
	void RotFocusUp();
	void RotFocusDown();

	void focusCamera();
	void focusNext();

	void Wiggle();	//remove
};


#endif
