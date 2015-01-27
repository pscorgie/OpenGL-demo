#pragma once

#include "GL\freeglut.h"
#include <vector>

struct Vector3
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
};
struct Camera
{
	Vector3 eye;
	Vector3 center;
	Vector3 up;
	Vector3 direction;
	float angle, deltaAngle, deltaMove;
	
};
struct Color
{
	GLfloat r, g, b;
};
struct Vertex
{
	GLfloat x, y, z;
};
struct TexCoord
{
	GLfloat u, v;
};
struct Mesh
{
	Vertex* Vertices;
	Vector3* Normal;
	GLushort* Indices;
	int VertexCount, NormalCount, IndexCount;

	std::vector< Vector3 > vertices;
	std::vector< TexCoord > uvs;
	std::vector< Vector3 > normals;
};
struct TexturedMesh
{
	Mesh* Mesh;
	TexCoord* TexCoords;
	int TexCoordCount;
};
struct Vector4
{
	float x, y, z, w;
};
struct Lighting
{
	Vector4 Ambient, Diffuse, Specular;
};
struct Material
{
	Vector4 Ambient, Diffuse, Specular;
	GLfloat shininess;
};
