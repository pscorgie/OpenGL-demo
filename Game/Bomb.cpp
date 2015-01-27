#include "Bomb.h"
#include <iostream>


#define _USE_MATH_DEFINES
#include <math.h>


Bomb::Bomb(TexturedMesh * mesh, Texture2D * texture, int type) : MovingObject(mesh, texture, type)
{
	_material = new Material();
	_material->Ambient.x = 1.00; _material->Ambient.y = 1.00; _material->Ambient.z = 1.00; _material->Ambient.w = 1.0;
	_material->Diffuse.x = 1.00; _material->Diffuse.y = 1.00; _material->Diffuse.z = 1.00; _material->Diffuse.w = 1.0;
	_material->Specular.x = 1.0; _material->Specular.y = 1.0; _material->Specular.z = 1.0; _material->Specular.w = 1.0;
	_material->shininess = 100.0f;

	_rotation = 0; _moveRot = 0.0;
	_position.x = 0.0; _position.y = 0.0f; _position.z = 0.0f;
	_rotationDir.x = 0.0; _rotationDir.y = 0.9f; _rotationDir.z = 0.0f;
	_direction.x = 0.0; _direction.y = 0.0f; _direction.z = 1.0f;

	_position.x = 0;
	_position.y = 0;
	_position.z = 0;
	_speed = 0.05;
	_dropped = false;
	_hit = false;
}

Bomb::~Bomb(void)
{
	delete _material;
}
void Bomb::Draw()
{
	glBindTexture(GL_TEXTURE_2D, _texture->GetID());
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glMaterialfv(GL_FRONT, GL_AMBIENT, &(_material->Ambient.x));
	glMaterialfv(GL_FRONT, GL_DIFFUSE, &(_material->Diffuse.x));
	glMaterialfv(GL_FRONT, GL_SPECULAR, &(_material->Specular.x));
	glMaterialf(GL_FRONT, GL_SHININESS, _material->shininess);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, _mesh->Mesh->vertices.data());
	glTexCoordPointer(2, GL_FLOAT, 0, _mesh->Mesh->uvs.data());
	glNormalPointer(GL_FLOAT, 0, _mesh->Mesh->normals.data());


	glPushMatrix();
		glTranslatef(_position.x, _position.y, _position.z);
		glRotatef(_rotation, 0, _rotationDir.y, 0);
		glDrawArrays(GL_TRIANGLES, 0, _mesh->Mesh->vertices.size());
	glPopMatrix();
	

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D, 0);
}
void Bomb::Update()
{
	if(_position.y >= 0)
	{
		_direction.x = sin(_moveRot);
		_direction.z = cos(_moveRot);
		_position.x += _direction.x * _speed;
		_position.z += _direction.z * _speed;
	}

	_position.y -= 0.2;
	switch(_type)
	{
	case 4:
		if(_position.y <= -1.0)
			_position.y = -1.0;
		break;
	case 5:
		if(_position.y <= -1.4)
			_position.y = -1.4;
		break;
	case 6:
		if(_position.y <= -1.4)
			_position.y = -1.4;
		break;
	}
	_rotation = 180 + (_moveRot * (180 / M_PI));
}
