#pragma once
#include "SceneObject.h"
#include "Structures.h"
#include "Texture2D.h"

class MovingObject : public SceneObject
{
protected:
	TexturedMesh * _mesh;
	Texture2D * _texture;
	Material * _material;
	int _type;
	Vector3 _position, _direction;
	GLfloat _rotation, _moveRot;
	Vector3 _rotationDir;
	float _speed;
		
public:
	MovingObject(TexturedMesh * mesh, Texture2D * texture);
	MovingObject(TexturedMesh * mesh, Texture2D * texture, int type);
	~MovingObject(void);

	void Draw();
	void Update();
	int getType(){return _type;}
	void setRotation(GLfloat rotAng, Vector3 rotation);
	void setPosition(Vector3 position);
	void setDirection(Vector3 direction);
	void setMoveRot(float r){_moveRot = r;}
	void setSpeed(float speed){_speed = speed;}
	Vector3 getPosition(){return _position;}
	
};

