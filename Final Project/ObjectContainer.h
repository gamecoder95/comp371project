#ifndef OBJECTCONTAINER_H
#define OBJECTCONTAINER_H

#include "stdafx.h"
#include "BaseObject.h"
#include <vector>

class ObjectContainer
{
private:
	std::vector<BaseObject*> object_container;

	void testAllCollisions(int index);

public:
	ObjectContainer();
	~ObjectContainer();

	void addObject(BaseObject* new_obj);
	void removeObject(int index);
	void removeAll();
	BaseObject* getObject(int index);
	void updateAll();

};

#endif
