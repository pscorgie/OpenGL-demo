#include "StaticObject.h"


StaticObject::StaticObject(TexturedMesh * mesh, Texture2D * texture) : SceneObject(mesh->Mesh)
{
	_mesh = mesh;
	_texture = texture;

	_material = new Material();
	_material->Ambient.x = 1.00; _material->Ambient.y = 1.00; _material->Ambient.z = 1.00; _material->Ambient.w = 1.0;
	_material->Diffuse.x = 1.00; _material->Diffuse.y = 1.00; _material->Diffuse.z = 1.00; _material->Diffuse.w = 1.0;
	_material->Specular.x = 1.0; _material->Specular.y = 1.0; _material->Specular.z = 1.0; _material->Specular.w = 1.0;
	_material->shininess = 100.0f;

	_rotation = 0;
	_rotationDir.x = 0;
	_rotationDir.y = 0;
	_rotationDir.z = 1;

	_position.x = 0;
	_position.y = 0;
	_position.z = 0;

	_location.x = -2; _location.y = 0; _location.z= -3; 
	_size.x = 4; _size.y = 3; _size.z = 7;

}


StaticObject::~StaticObject(void)
{
	delete _mesh->Mesh;
	delete _mesh;
	delete _texture;
	delete _material;
}
void StaticObject::Draw()
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
void StaticObject::Update()
{
	_location.x = -2 + _position.x;
	_location.z = -3.3 + _position.z;
	_size.x = 4;
	_size.z = 7;
	if (_rotation == 90)
	{
		_location.z = -2 + _position.z;
		_location.x = -3.3 + _position.x;
		_size.x = 7;
		_size.z = 4;
	}
}
void StaticObject::setRotation(GLfloat rotAng, Vector3 rotation)
{
	_rotation = rotAng;
	_rotationDir = rotation;
}
void StaticObject::setPosition(Vector3 position)
{
	_position = position;
}