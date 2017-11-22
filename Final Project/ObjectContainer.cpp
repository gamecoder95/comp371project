#include "stdafx.h"

#include "ObjectContainer.h"


ObjectContainer::ObjectContainer()
{
}


ObjectContainer::~ObjectContainer()
{
	removeAll();
}

void ObjectContainer::addObject(Object* new_obj)
{
	object_container.push_back(new_obj);
}

Object* ObjectContainer::getObject(int index)
{
	return object_container[index];
}

void ObjectContainer::updateAll()
{
	for (int i = 0; i < object_container.size(); ++i)
	{
		object_container[i]->update();
	}
}

void ObjectContainer::removeAll()
{
	for (int i = 0; i < object_container.size(); ++i)
	{
		object_container[i]->destroy();
		delete object_container[i];
	}
	object_container.erase(object_container.begin(), object_container.end());
}

void ObjectContainer::removeObject(int index)
{
	object_container[index]->destroy();
	delete object_container[index];
	object_container.erase(object_container.begin() + index);
}