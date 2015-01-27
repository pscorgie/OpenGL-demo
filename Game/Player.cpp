#include "Player.h"
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

Player::Player(SceneNode* m):SceneGraph(m)
{

	_move = false; _moveLeft = false; _moveRight = false; _moveForward = false; _moveBack = false; _moveUp = false; _moveDown = false;
	_moveRot = 0.0f; _speed = 0.0f; _propRot = 0.0f;

	_position.x = 0.0; _position.y = 0.0f; _position.z = 0.0f;
	_direction.x = 0.0; _direction.y = 0.0f; _direction.z = 1.0f;
	_rotationDir.x = 0.0; _rotationDir.y = 0.9f; _rotationDir.z = 0.0f;
	
	_rotation = 0.0f; _rotation2 = 0.0f; _rotation3 = 0.0f;
}


Player::~Player(void)
{
}

void Player::Draw()
{
	glPushMatrix();
		glTranslatef(_position.x, _position.y, _position.z);
		glRotatef(_rotation, 0, _rotationDir.y, 0); // yaw
		glRotatef(_rotation2, 0, 0, 1);	// roll
		glRotatef(_rotation3, 1, 0, 0);	// pitch
		_parent->object->Draw();
		for (int i = 0; i < _parent->children.size(); i++)
		{
			glPushMatrix();
			childTransform(_parent->children[i]->object->getType());
			_parent->children[i]->object->Draw();
			glPopMatrix();
		}
	glPopMatrix();
}
void Player::Update()
{
	// forward movement
	if(_move)
	{
		_position.x += _direction.x * _speed;
		_position.z += _direction.z * _speed;
	}

	if(_moveForward)
	{
		_move = true;
		_speed += .001;
		if(_speed > 1.0)
			_speed =1.0;
	}
	if(_moveBack)
	{
		if(_position.y <= 0.1 && _speed <= 0.055)
		{
			_move = false;
		}

		_speed -= .001;
		if(_speed < 0.05)
			_speed = 0.05;
	}
	if(_moveLeft)
	{
		// roll left
		if(_move)
		{
			if(_position.y > 0)
			{
				if(_rotation2 <0)
					_rotation2 += 1.8 * _speed;
				else
					_rotation2 += 1 * _speed;
			}
			else
				_moveRot += 0.01f;
		}
		if(_rotation2 >= 180)
			_rotation2 = -180;
	}
	if(_moveRight)
	{	
		// roll right
		if(_move)
		{
			if(_position.y > 0)
			{
				if(_rotation2 >0)
					_rotation2 -= 1.8 * _speed;
				else
					_rotation2 -= 1 * _speed;
			}
			else
				_moveRot -= 0.01f;
		}
		if(_rotation2 <= -180)
			_rotation2 = 180;
	}

	// controls yaw - turn based on the rotation of roll
	if(_rotation2 < 90 && _rotation2 >= 0)
		_moveRot += 0.01 * (0.02*_rotation2 );
	else if(_rotation2 > -90 && _rotation2 < 0)
		_moveRot -= 0.01 * (0.02*-_rotation2);
	else if(_rotation2 < 180 && _rotation2 > 90)
		_moveRot += 0.01 * (0.02*(_rotation2/2));
	else if(_rotation2 > -180 && _rotation2 < -90)
		_moveRot -= 0.01 * (0.02*(-_rotation2/2));

	_direction.x = sin(_moveRot);
	_direction.z = cos(_moveRot);

	// if player is not turning return plane to neutral position
	if(_moveRight == false && _moveLeft == false)
	{
		if (_rotation2 < 1 && _rotation2 > -0)
		{
			_rotation2 = 0;
		}
		else if(_rotation2 > 0 && _rotation2 < 180 || _rotation2 < -180 )
		{
			_rotation2 -= .1;
		}
		else if (_rotation2 < 0 && _rotation2 > -180 || _rotation2 > 180)
		{
			_rotation2 += .1;
		}
	}

	// Controls Elevation
	if(_moveUp)
	{
		if(_speed > 0.4)
			_position.y += 0.1;
	}
	else
	{
		if(_speed < 0.4)
			_position.y -= (0.4 - _speed);
		if(_position.y <= 0)
			_position.y  = 0;
	}
	if(_moveDown)
	{
		if(_speed >= 0.4)
			_position.y -= 0.1;
		if(_position.y <= 0)
			_position.y  = 0;
	}

	if(_position.y == 0 && _rotation2 != 0)
	{
		if(_rotation2 > 0)
			_rotation2 -= 1.0;
		else
			_rotation2 += 1.0;
	}

	// Controls Pitch
	if(_speed >0.10)
	{
		_rotation3 -= 0.1*_speed;
		if(_rotation3 < -8)
			_rotation3 = -8;
	}
	else if(_speed < 0.20)
	{
		_rotation3 += 0.1 ;
		if(_rotation3 >= 0)
			_rotation3 = 0;
	}
	
	// propellor rotation
	_propRot += 30;
	if(_propRot > 360)
		_propRot = 0;
	
	// convert _rotation to degrees
	// add 180 so its facing correct direction
	_rotation = 180 + (_moveRot * (180 / M_PI));

	
}
void Player::childTransform(int type)
{

	switch(type)
	{
	case PROPELLOR: 
			glTranslatef(0, 2.4, -2.95);
			glRotatef(_propRot, 0, 0, 1);
		break;
	case GLASS:
		break;
	case CONTROLS:
		break;
	case BOMB1:
		break;
	case BOMB2:
			glTranslatef(2.45, 0, 0);
		break;
	case BOMB3:
			glTranslatef(-2.45, 0, 0);
		break;
	case LEFTWHEEL:
			if(_position.y > 5)
			{
				glTranslatef(-2.22, 1.3,-0.9);
			}
			else
			{
				glTranslatef(-2.6, 1.45,-1.05);
				glRotatef(80, 0.35, 0.25, -0.85);
			}
		break;
	case RIGHTWHEEL:
			if(_position.y > 5)
			{
				glTranslatef(2.25, 1.7,-1.35);
			}
			else
			{
				glTranslatef(2.28, 1.8,-1.25);
				glRotatef(80, 0.3, -.2, 0.65);
			}
		break;
	}
}
void Player::MoveForward(bool b)
{
	_moveForward = b;
}
void Player::MoveBack(bool b)
{
	_moveBack = b;
}
void Player::MoveLeft(bool b)
{
	_moveLeft = b;
}
void Player::MoveRight(bool b)
{
	_moveRight = b;
}
void Player::MoveUp(bool b)
{
	_moveUp = b;
}
void Player::MoveDown(bool b)
{
	_moveDown = b;
}

void Player::setPosition(Vector3 position){}
void Player::setRotation(GLfloat rotation, Vector3 _rotationDir){}

Vector3 Player::getPosition()
{
	return _position;
}
Vector3 Player::getDirection()
{
	return _direction;
}
GLfloat Player::getRotation()
{
	return _rotation2;
}
float Player::getAngle()
{
	return _moveRot;
}
float Player::getSpeed()
{
	return _speed;
}
void Player::reset()
{
	_move = false; _moveLeft = false; _moveRight = false; _moveForward = false; _moveBack = false; _moveUp = false; _moveDown = false;
	_moveRot = 0.0f; _speed = 0.0f; _propRot = 0.0f;

	_position.x = 0.0; _position.y = 0.0f; _position.z = 0.0f;
	_direction.x = 0.0; _direction.y = 0.0f; _direction.z = 1.0f;
	_rotationDir.x = 0.0; _rotationDir.y = 0.9f; _rotationDir.z = 0.0f;
	
	_rotation = 0.0f; _rotation2 = 0.0f; _rotation3 = 0.0f; 
}
