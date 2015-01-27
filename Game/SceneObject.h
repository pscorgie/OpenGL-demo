#pragma once
#include "Structures.h"

class SceneObject
{
protected:
	Mesh * _mesh;
public:
	SceneObject(Mesh* mesh);
	virtual ~SceneObject(void);

	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void setRotation(GLfloat rotAng, Vector3 rotation){};
	virtual void setPosition(Vector3 position){};
};

