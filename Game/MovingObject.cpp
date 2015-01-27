#include "MovingObject.h"
#include <iostream>


#define _USE_MATH_DEFINES
#include <math.h>


MovingObject::MovingObject(TexturedMesh * mesh, Texture2D * texture, int type) : SceneObject(mesh->Mesh)
{
	_mesh = mesh;
	_texture = texture;
	_type = type;

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
}

MovingObject::~MovingObject(void)
{
	delete _mesh->Mesh;
	delete _mesh;
	delete _texture;
	delete _material;
}
void MovingObject::Draw()
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
		glDrawArrays(GL_TRIANGLES, 0, _mesh->Mesh->vertices.size());
	glPopMatrix();
	

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D, 0);
}
void MovingObject::Update()
{
}

void MovingObject::setRotation(GLfloat rotAng, Vector3 rotation)
{
	_rotation = rotAng;
	_rotationDir = rotation;
}
void MovingObject::setPosition(Vector3 position)
{
	_position = position;
}
void MovingObject::setDirection(Vector3 direction)
{
	_direction = direction;
}
