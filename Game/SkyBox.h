#pragma once
#include "Structures.h"
#include "GL\freeglut.h"
#include "Texture2D.h"

class SkyBox
{
private:
	float x, y, z, width, height, length;
	Texture2D * _skyUp, * _skyDown, * _skyFront, * _skyBack, * _skyLeft, * _skyRight;

public:
	SkyBox(void);
	~SkyBox(void);
	void Draw();
	void Update(float x, float y, float z);
	void loadTex();
};

