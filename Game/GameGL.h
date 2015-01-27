#pragma once

#include <Windows.h> // required for OpenGL on windows
#include <gl/GL.h> // OpenGL
#include <gl/GLU.h> // OpenGL Utilities
#include "GL\freeglut.h" // Freeglut library
#include "Structures.h"
#include "SceneObject.h"
#include "StaticObject.h"
#include "MovingObject.h"
#include "MeshLoader.h"
#include "Texture2D.h"
#include "SkyBox.h"
#include "LinkList.h"
#include "Player.h"
#include "Bomb.h"
#include "Tank.h"

#define REFRESHRATE 16
#define EASY 0
#define HARD 1

using namespace std;

class GameGL
{
private:
	Player* _player;
	Vector3 _playerPos;
	float _speed;
	Bomb* _bomb[3];
	bool _bKeyDown; 
	Vector3 _bombRot;
	Tank* _tank;
	Vector3 _tankLoc, _tankSize;
	int _score, _state, _difficulty;
	bool _enterDown;
	int _width, _height;
	float _ratio;

	Vector4* _lightPosition;
	Lighting* _lightData;

	
	Vector3 _textPos;
	Color _textColour;

	unsigned char _view;
	Camera * camera;
	
	bool _fog;
	bool _fKeyDown;
	SkyBox * _skyBox;
	Texture2D * gtexture, * rtexture, * rtexture2;

public:
	GameGL(int argc, char* argv[]);
	~GameGL(void);

	void Display();
	void Update();
	
	//initialise
	void InitObjects();
	void InitPlayer();
	void InitTank();
	void InitLighting();
	void InitGL(int argc, char* argv[]);
	void InitCamera();
	
	//scene
	void DrawFloor();
	void DrawFog();
	void OrientCamera(unsigned char key);

	//bomb mechanics
	void DropBomb();
	void Ammo();
	void Collision();
	
	//on screen txt
	void HUD();
	void DrawString(const char* text, Vector3 * position, Color * colour);


	void Keyboard(unsigned char key, int x, int y);
	void KeyboardUp(unsigned char key, int x, int y);
	void changeSize(int w, int h);
	void reset();
};

