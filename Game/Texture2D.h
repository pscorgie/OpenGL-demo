#pragma once
#include "GL\freeglut.h"

class Texture2D
{
private:
	GLuint _ID; //Texture ID
	int _width, _height;
public:
	Texture2D();
	~Texture2D();

	bool LoadRAW(char* path, int width, int height);

	bool LoadTGA(const char* textureFileName);

	bool LoadBMP(const char * imagepath);

	GLuint GetID() const { return _ID; }
	int GetWidth() const { return _width; }
	int GetHeight() const { return _height; }

};

