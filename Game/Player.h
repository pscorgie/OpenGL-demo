#pragma once
#include "SceneGraph.h"

#define BODY 0
#define GLASS 1
#define PROPELLOR 2
#define CONTROLS 3
#define BOMB1 4
#define BOMB2 5
#define BOMB3 6
#define LEFTWHEEL 7
#define RIGHTWHEEL 8

class Player : public SceneGraph
{
private:

	bool _move, _moveLeft, _moveRight, _moveForward, _moveBack, _moveUp, _moveDown;
	float _moveRot, _speed, _propRot;

	Vector3 _position, _direction, _rotationDir;
	GLfloat _rotation, _rotation2, _rotation3;

public:
	Player(SceneNode* m);
	~Player(void);
	
	void Draw();
	void Update();

	void MoveForward(bool b);
	void MoveBack(bool b);
	void MoveLeft(bool b);
	void MoveRight(bool b);
	void MoveUp(bool b);
	void MoveDown(bool b);

	void setPosition(Vector3 position);
	void setRotation(GLfloat rotation, Vector3 _rotationDir);

	Vector3 getPosition();
	Vector3 getDirection();
	GLfloat getRotation();
	float getAngle();
	float getSpeed();
	void reset();
	void childTransform(int type);

};

