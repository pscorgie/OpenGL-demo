#include "Tank.h"
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

Tank::Tank(LinkList* parts)
{
	_parts = parts;
	_move = false; 
	_moveRot = 0.0f; _speed = 0.2f; _propRot = 0.0f;

	_position.x = 0.0; _position.y = 0.0f; _position.z = 0.0f;
	_direction.x = 0.0; _direction.y = 0.0f; _direction.z = 1.0f;
	_rotationDir.x = 0.0; _rotationDir.y = 0.0f; _rotationDir.z = 0.0f;
	_location.x = 0.0; _location.y = 0.0f; _location.z = 0.0f;
	_size.x = 0.0; _size.y = 0.0f; _size.z = 0.0f;
	_route = 0;
	_rotation = 0.0f; _rotation2 = 0.0f; _rotation3 = 0.0f;
}


Tank::~Tank(void)
{
	_parts->deleteList(_parts->returnHead());
	delete _parts;
}

void Tank::Draw()
{
	glPushMatrix();
		glTranslatef(_position.x, _position.y, _position.z);
		glRotatef(_rotation, 0, _rotationDir.y, 0); 
		_parts->draw(_parts->returnHead());
	glPopMatrix();
}
void Tank::Update()
{
	if(_move)
	{
		if(_route == 0)
		{

			_position.z -= _speed;
			_rotation = 0;
			if(_position.z < 50)
			{
				_route =1;
				_position.z  = 50;
			}
		}
		if(_route == 1)
		{
			_position.x += _speed;
			_rotation = -90;
			if(_position.x > 100)
			{
				_route = 2;
				_position.x = 100;
			}
		}
		if(_route == 2)
		{
			_position.z += _speed;
			_rotation = 180;
			if(_position.z > 100)
			{
				_route = 3;
				_position.z == 100;
			}
		}
		if(_route == 3)
		{
			_position.x -= _speed;
			_rotation = 90;
			if(_position.x < 50)
			{
				_route =0;
				_position.x = 50;
			}
		}
	}

	_location.x = -2 + _position.x;
	_location.z = -3.3 + _position.z;
	_size.x = 4;
	_size.z = 7;
	if (_rotation == 90 || _rotation == -90)
	{
		_location.z = -2 + _position.z;
		_location.x = -3.3 + _position.x;
		_size.x = 7;
		_size.z = 4;
	}
}
void Tank::childTransform(int type)
{

}
void Tank::setPosition(Vector3 position)
{
	_position = position;
}
void Tank::setRotation(GLfloat rotation, Vector3 rotationDir)
{
	_rotation = rotation;
	_rotationDir = rotationDir;
}

Vector3 Tank::getPosition()
{
	return _position;
}
Vector3 Tank::getDirection()
{
	return _direction;
}
GLfloat Tank::getRotation()
{
	return _rotation2;
}
float Tank::getAngle()
{
	return _moveRot;
}
float Tank::getSpeed()
{
	return _speed;
}
void Tank::reset()
{
	_move = false; 
	_moveRot = 0.0f; _speed = 0.0f; _propRot = 0.0f;

	_position.x = 0.0; _position.y = 0.0f; _position.z = 0.0f;
	_direction.x = 0.0; _direction.y = 0.0f; _direction.z = 1.0f;
	_rotationDir.x = 0.0; _rotationDir.y = 0.9f; _rotationDir.z = 0.0f;
	
	_rotation = 0.0f; _rotation2 = 0.0f; _rotation3 = 0.0f;
}
