#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include "stdafx.h"

#include "Shader.h"

class BaseObject
{
protected:
	Shader* shader;
public:
	BaseObject(Shader* shader);
	~BaseObject();
	virtual void update() = 0;
	virtual void destroy() = 0;
};

#endif