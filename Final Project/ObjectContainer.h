#ifndef OBJECTCONTAINER_H
#define OBJECTCONTAINER_H

#include "VirtualWorldContent.h"

// Make fully static?

class ObjectContainer
{
private:
	std::vector<Object*> object_container;

public:
	ObjectContainer();
	~ObjectContainer();

	void addObject(Object* new_obj);
	void removeObject(int index);
	void removeAll();
	Object* getObject(int index);
	void updateAll();

};

#endif
