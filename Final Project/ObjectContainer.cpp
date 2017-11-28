#include "stdafx.h"

#include "ObjectContainer.h"
#include <iostream>

ObjectContainer::ObjectContainer()
{
}


ObjectContainer::~ObjectContainer()
{
	removeAll();
}

void ObjectContainer::addObject(BaseObject* new_obj)
{
	object_container.push_back(new_obj);
}

BaseObject* ObjectContainer::getObject(int index)
{
	return object_container[index];
}

void ObjectContainer::testAllCollisions(int index)
{
	if (index >= 0 && index < object_container.size())
	{
		for (int i = 0; i < object_container.size(); ++i)
		{
			if (index != i)
			{
				if (object_container[index]->isCollision(*object_container[i]))
				{
					object_container[index]->onCollision(*object_container[i]);
				}
			}
		}
	}
}

void ObjectContainer::updateAll()
{
	for (int i = 0; i < object_container.size(); ++i)
	{
		//if (object_container[i]->isDestroyed())
		//{
		//	//removeObject(i);
		//}
		//else
		//{
			object_container[i]->update();
			
		//}
	}
	for (int i = 0; i < object_container.size(); ++i)
	{
		testAllCollisions(i);
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