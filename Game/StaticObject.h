#pragma once
#include "SceneObject.h"
#include "Structures.h"
#include "Texture2D.h"

class StaticObject : public SceneObject
{
private:
	TexturedMesh * _mesh;
	Texture2D * _texture;
	Material * _material;

	Vector3 _position;
	GLfloat _rotation;
	Vector3 _rotationDir;
	Vector3 _location;
	Vector3 _size; 

public:
	StaticObject(TexturedMesh * mesh, Texture2D * texture);
	~StaticObject(void);

	void Draw();
	void Update();
	void setRotation(GLfloat rotAng, Vector3 rotation);
	void setPosition(Vector3 position);
	Vector3 getSize(){return _size;}
	Vector3 getLocation(){return _location;}
};

