#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include "Shader.h"

class BaseObject
{
protected:
	Shader* shader;
public:
	BaseObject(Shader* shader);
	~BaseObject();
};

#endif