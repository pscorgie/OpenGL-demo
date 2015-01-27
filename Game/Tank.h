#pragma once
#include "SceneGraph.h"
#include "LinkList.h"

#define BODY1 0
#define BODY2 1
#define TRACK 2
#define TURRET 3
#define TURRET2 4


class Tank
{
private:

	LinkList* _parts;

	bool _move;
	float _moveRot, _speed, _propRot;
	int _route;

	Vector3 _position, _direction, _rotationDir, _location, _size;
	GLfloat _rotation, _rotation2, _rotation3;

public:
	Tank(LinkList* parts);
	~Tank(void);
	
	void Draw();
	void Update();

	void setPosition(Vector3 position);
	void setRotation(GLfloat rotation, Vector3 rotationDir);

	Vector3 getPosition();
	Vector3 getDirection();
	GLfloat getRotation();
	float getAngle();
	float getSpeed();
	void setMove(bool b){_move = b;}
	void reset();
	void childTransform(int type);
	Vector3 getSize(){return _size;}
	Vector3 getLocation(){return _location;}

};

