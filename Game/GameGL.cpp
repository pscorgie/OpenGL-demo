#include "GameGL.h"
#include "GLUTCallbacks.h"
#include <iostream>
#include <sstream>

#define _USE_MATH_DEFINES
#include <math.h>


GameGL::GameGL(int argc, char* argv[])
{
	camera = new Camera();
	InitCamera();
	_speed = 0.0;
	_view = 's';
	_textPos.x = 20.0f; _textPos.y = 0.0f; _textPos.z = 0.0f; _textColour.r = 0.0f; _textColour.g = 0.0f; _textColour.b = 1.0f;
	_fog = false;
	_fKeyDown = true;
	_bKeyDown = false;
	_enterDown = false;
	_score = 0;
	_state = 0;
	_difficulty = 0;
	_width = 800; _height = 800;

	InitGL(argc, argv);
	InitObjects();
	InitLighting();

	glutMainLoop();
}
GameGL::~GameGL(void)
{
	delete _player;
	
	delete _tank;
	delete _lightPosition;
	delete _lightData;
	delete _skyBox;
	delete gtexture; 
	delete rtexture;
	delete rtexture2;
	for (int i = 0; i < 3; i++)
	{
		delete _bomb[i];
	}
	delete [] _bomb;
}
void GameGL::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clears the scene so we start fresh each time

	HUD();

	_skyBox->Draw();
	DrawFloor();
	if(_fog)
		DrawFog();

	_tank->Draw();
	_player->Draw();

	for (int i = 0; i < 3; i++)
	{
		if(_bomb[i]->dropped() == true)
		{
			_bomb[i]->Draw();
		}
	}
	glutSwapBuffers();
	//glFlush(); // Flushes the scene we just drew to the graphics card
}
void GameGL::Update()
{
	glLoadIdentity(); // Reset Matrix - prevents transformations from previous frame being drawn in this frame

	switch(_state)
	{
	case 0:
		break;
	case 1:
		break;
	case 3:
		_playerPos = _player->getPosition();
		_speed = _player->getSpeed();

		_player->Update();
		camera->direction = _player->getDirection();
		camera->angle = _player->getAngle();
		
	
		for (int i = 0; i < 3; i++)
		{
			if(_bomb[i]->dropped() == true && _bomb[i]->hit() == false)
			{
				_bomb[i]->Update();
			}
		}
		if(_bomb[2]->dropped() == true &&_playerPos.y <= 0.5)
		{
			Ammo();
		}

		if(_difficulty == HARD)
			_tank->Update();

		Collision();
		break;
	}
	

	if(_score < 5)
		OrientCamera(_view);
	gluLookAt(camera->eye.x, camera->eye.y, camera->eye.z, camera->center.x, camera->center.y, camera->center.z, camera->up.x, camera->up.y, camera->up.z);	
	//_skyBox->Update(camera->eye.x, camera->eye.y-100, camera->eye.z);

	glLightfv(GL_LIGHT0, GL_AMBIENT, &(_lightData->Ambient.x));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &(_lightData->Diffuse.x));
	glLightfv(GL_LIGHT0, GL_SPECULAR, &(_lightData->Specular.x));
	glLightfv(GL_LIGHT0, GL_POSITION, &(_lightPosition->x));
	
	
	glutPostRedisplay(); // Redraw the scene
}
void GameGL::InitObjects()
{
	_skyBox = new SkyBox;

	gtexture = new Texture2D();
	gtexture->LoadBMP("Textures/ground_grass_gen_02.BMP");
	rtexture = new Texture2D();
	rtexture->LoadBMP("Textures/ground_asphalt_synth_03.BMP");
	rtexture2 = new Texture2D();
	rtexture2->LoadBMP("Textures/ground_asphalt_synth_12.BMP");

	InitPlayer();
	InitTank();
}
void GameGL::InitPlayer()
{

	// Player airplane body
	Texture2D * ptexture = new Texture2D();
	ptexture->LoadBMP("Textures/MainBody.BMP");

	TexturedMesh * plane = new TexturedMesh();
	plane = MeshLoader::LoadOBJ("Objects/Player/body.obj");

	MovingObject* _plane = new MovingObject(plane, ptexture, 0);

	// propellor
	Texture2D* propTex = new Texture2D();
	propTex->LoadBMP("Textures/controls.bmp");

	TexturedMesh * prop = new TexturedMesh();
	prop = MeshLoader::LoadOBJ("Objects/Player/prop.obj");

	MovingObject* _propellor = new MovingObject(prop, propTex, 2);
	
	// glass
	Texture2D* glassTex = new Texture2D();
	glassTex->LoadTGA("Textures/GlassT.TGA");

	TexturedMesh * glassMesh = new TexturedMesh();
	glassMesh = MeshLoader::LoadOBJ("Objects/Player/glass.obj");

	MovingObject* _glass = new MovingObject(glassMesh, glassTex, 1);

	// controls
	TexturedMesh * controlMesh = new TexturedMesh();
	controlMesh = MeshLoader::LoadOBJ("Objects/Player/controls.obj");

	MovingObject* _control = new MovingObject(controlMesh, propTex, 3);

	// Bombs
	// centre bomb
	Texture2D* bombtex = new Texture2D();
	bombtex->LoadTGA("Textures/bombs2.tga");

	TexturedMesh * Bomb1 = new TexturedMesh();
	Bomb1 = MeshLoader::LoadOBJ("Objects/Player/Bomb1.obj");

	_bomb[0] = new Bomb(Bomb1, bombtex, 4);
	
	//side bombs
	Bomb1 = MeshLoader::LoadOBJ("Objects/Player/Bomb2.obj");

	_bomb[1] = new Bomb(Bomb1, bombtex, 5);
	_bomb[2] = new Bomb(Bomb1, bombtex, 6);
	_bombRot.x = 0.0f; _bombRot.y = 0.9f; _bombRot.z = 0.0f;
	
	// wheels
	TexturedMesh * Wheel = new TexturedMesh();
	Wheel = MeshLoader::LoadOBJ("Objects/Player/LeftWheel.obj");

	MovingObject* _leftWheel = new MovingObject(Wheel, ptexture, 7);
	Wheel = MeshLoader::LoadOBJ("Objects/Player/RightWheel.obj");

	MovingObject* _rightWheel = new MovingObject(Wheel, ptexture, 8);

	// add parts to the scenegraph
	SceneNode* splane =  new SceneNode(_plane);
	_player = new Player(splane);
	SceneNode* spropellor = new SceneNode(_propellor);
	_player->addChild(spropellor);
	SceneNode* scontrol = new SceneNode(_control);
	_player->addChild(scontrol);
	SceneNode* slWheel = new SceneNode(_leftWheel);
	_player->addChild(slWheel);
	SceneNode* srWheel = new SceneNode(_rightWheel);
	_player->addChild(srWheel);
	SceneNode* sglass = new SceneNode(_glass);
	_player->addChild(sglass);
	SceneNode* sbomb1 = new SceneNode(_bomb[0]);
	_player->addChild(sbomb1);
	SceneNode* sbomb2 = new SceneNode(_bomb[1]);
	_player->addChild(sbomb2);
	SceneNode* sbomb3 = new SceneNode(_bomb[2]);
	_player->addChild(sbomb3);

	// bomb fired and hit variables
	for (int i = 0; i < 3; i++)
	{
		_bomb[i]->drop(false);
		_bomb[i]->hasHit(false);
	}
	_player->Update();
}

void GameGL::InitTank()
{
	// body1
	Texture2D * tex1 = new Texture2D();
	tex1->LoadBMP("Objects/Tank/Body1.BMP");

	TexturedMesh * mesh1 = new TexturedMesh();
	mesh1 = MeshLoader::LoadOBJ("Objects/Tank/body1.obj");

	StaticObject* _part1 = new StaticObject(mesh1, tex1);
	LinkList* _tankParts = new LinkList(_part1);

	// body2
	Texture2D * tex2 = new Texture2D();
	tex2->LoadBMP("Objects/Tank/Body2.BMP");
	TexturedMesh * mesh2 = new TexturedMesh();
	mesh2 = MeshLoader::LoadOBJ("Objects/Tank/body2.obj");
	StaticObject* _part2 = new StaticObject(mesh2, tex2);
	node* part2 = _tankParts->makeNode(_part2);
	_tankParts->insertNode(_tankParts->returnHead(), part2);

	// turret
	Texture2D * tex3 = new Texture2D();
	tex3->LoadBMP("Objects/Tank/Turret.BMP");
	TexturedMesh * mesh3 = new TexturedMesh();
	mesh3 = MeshLoader::LoadOBJ("Objects/Tank/turret.obj");
	StaticObject*_part3 = new StaticObject(mesh3, tex3);
	node* part3 = _tankParts->makeNode(_part3);
	_tankParts->insertNode(part2, part3);

	// turret2
	Texture2D * tex4 = new Texture2D();
	tex4->LoadTGA("Objects/Tank/Turret2.tga");
	TexturedMesh * mesh4 = new TexturedMesh();
	mesh4 = MeshLoader::LoadOBJ("Objects/Tank/turret2.obj");
	StaticObject* _part4 = new StaticObject(mesh4, tex4);
	node* part4 = _tankParts->makeNode(_part4);
	_tankParts->insertNode(part3, part4);

	// track
	Texture2D * tex5 = new Texture2D();
	tex5->LoadTGA("Objects/Tank/Track.tga");
	TexturedMesh * mesh5 = new TexturedMesh();
	mesh5 = MeshLoader::LoadOBJ("Objects/Tank/track.obj");
	StaticObject*_part5 = new StaticObject(mesh5, tex5);
	node* part5 = _tankParts->makeNode(_part5);
	_tankParts->insertNode(part4, part5);

	Vector3 position = {50, 0, 100};
	Vector3 rotation = {0, 1, 0};

	_tank = new Tank(_tankParts);
	_tank->setPosition(position);
	_tank->setRotation(90, rotation);
	_tank->Update();

}
void GameGL::InitLighting()
{
	_lightPosition = new Vector4();
	_lightPosition->x = 0.0;
	_lightPosition->y = 1.0;
	_lightPosition->z = 1.0;
	_lightPosition->w = 0.0;

	_lightData = new Lighting();

	_lightData->Ambient.x = 0.0; 
	_lightData->Ambient.y = 0.0; 
	_lightData->Ambient.z = 0.0; 
	_lightData->Ambient.w = 0.0;
	_lightData->Diffuse.x = 1.0; 
	_lightData->Diffuse.y = 1.0; 
	_lightData->Diffuse.z = 1.0; 
	_lightData->Diffuse.w = 1.0;
	_lightData->Specular.x = 1.0; 
	_lightData->Specular.y = 1.0; 
	_lightData->Specular.z = 1.0;
	_lightData->Specular.w = 1.0;

}
void GameGL::InitGL(int argc, char* argv[])
{
	GLUTCallbacks::Init(this);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(_width, _height);
	glutCreateWindow("Peter's OpenGL Game");

	// OpenGL calls for GLUTCallbacks
	glutDisplayFunc(GLUTCallbacks::Display);
	glutTimerFunc(REFRESHRATE, GLUTCallbacks::Timer, REFRESHRATE);
	glutKeyboardFunc(GLUTCallbacks::Keyboard);
	glutKeyboardUpFunc(GLUTCallbacks::KeyboardUp);
	glutReshapeFunc(GLUTCallbacks::Reshape);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glCullFace(GL_BACK);
	
}

void GameGL::Keyboard(unsigned char key, int x, int y)
{
		switch(key)
	{
		
		case 'a': 
			_player->MoveLeft(true);
			break;
		case 'd': 
			_player->MoveRight(true);
			break;
		case 'w': 
			_player->MoveForward(true);
			break;
		case 's': 
			_player->MoveBack(true);
			break;
		case 32:
			_player->MoveUp(true);
			break;
		case 'v':
			_player->MoveDown(true);
			break;
		case 13:
			if(_enterDown == false)
			{
				if(_state == 1)
				{
					_state = 2;
					camera->angle = 0;
				}
				else if(_state == 0)
				{
					_state = 1;
					camera->angle = 0;
				}
				else if(_state == 2)
					_state = 3;

				if(_state ==3 && _score == 5)
					reset();
				_enterDown = true;
			}
			
			break;
		case 'f':
			if(_fKeyDown == true && _fog == false)
			{
				glEnable(GL_FOG);
				_fog = true;
				_fKeyDown = false;

				_lightData->Diffuse.x = 0.3; 
				_lightData->Diffuse.y = 0.3; 
				_lightData->Diffuse.z = 0.3; 
				_lightData->Diffuse.w = 1.0;
				_lightData->Specular.x = 0.5; 
				_lightData->Specular.y = 0.5; 
				_lightData->Specular.z = 0.5;
				_lightData->Specular.w = 1.0;
		
			}
			if(_fKeyDown == true && _fog == true)
			{
				glDisable(GL_FOG);
				_fog = false;
				_fKeyDown = false;

				_lightData->Diffuse.x = 1.0; 
				_lightData->Diffuse.y = 1.0; 
				_lightData->Diffuse.z = 1.0; 
				_lightData->Diffuse.w = 1.0;
				_lightData->Specular.x = 1.0; 
				_lightData->Specular.y = 1.0; 
				_lightData->Specular.z = 1.0;
				_lightData->Specular.w = 1.0;

			}
			break;
		case 'b':
			if(_bKeyDown == false)
			{
				DropBomb();
				_bKeyDown = true;
			}			
			break;
		case 'e':
			if(_state == 0)
			{
				_difficulty = EASY;
				_tank->setMove(false);
			}
			break;
		case 'h':
			if(_state == 0)
			{
				_difficulty = HARD;
				_tank->setMove(true);
			}
			break;
		case 'r':
			if(_bomb[2]->dropped() == true)
			{
				Ammo();
			}
			break;

	}

}
void GameGL::KeyboardUp(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'a': 
			_player->MoveLeft(false);
			break;
		case 'd': 
			_player->MoveRight(false);
			break;
		case 'w': 
			_player->MoveForward(false);
			break;
		case 's': 
			_player->MoveBack(false);
			break;
		case 32:
			_player->MoveUp(false);
			break;
		case 'v':
			_player->MoveDown(false);
			break;
		case 'i':
			_view = 'i';
			break;
		case'o':
			_view = 'o';
			break;
		case'p':
			_view = 'p';
			break;
		case 'f':
			_fKeyDown = true;
			break;
		case 'b':
			_bKeyDown = false;
			break;
		case 13:
			_enterDown = false;
			break;
		
	}
}

void GameGL::DrawFloor()
{
	float x = -1000.0;
	float y = -0.1;
	float z = -1000.0;
	float width = 2000;
	float length = 2000;

	// draws grass
	glBindTexture(GL_TEXTURE_2D, gtexture->GetID());
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
		glTexCoord2f(500.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(500.0f, 500.0f); glVertex3f(x+width, y,		z+length); 
		glTexCoord2f(0.0f, 500.0f); glVertex3f(x+width, y,		z);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	
	// draw runway
	x = 0.0;
	y = 0.0;
	z = 0.0;
	width = 5;
	length = 5;
	for (float x = -17.5; x < 22.5; x+=5)
	{
		for (float z = -10; z < 200; z+=5)
		{
			if(x ==-2.5 && (z!= -10 && z != 195))
			{
			glBindTexture(GL_TEXTURE_2D, rtexture2->GetID());
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, rtexture->GetID());
			}
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z+length);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y,		z+length); 
				glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y,		z);
			glEnd();
		}
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}
void GameGL::DrawFog()
{
	
	GLfloat fogColor[] = {0.5f, 0.5f, 0.5f, 1.0};
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogi(GL_FOG_MODE, GL_EXP);
	glFogf(GL_FOG_DENSITY, 0.03f);

}
void GameGL::InitCamera()
{
	camera->eye.x = 0.0f; camera->eye.y = 2.0f; camera->eye.z = 50.0f;
	camera->center.x = 0.0f; camera->center.y = 0.0f; camera->center.z = -1.0f;
	camera->up.x = 0.0f; camera->up.y = 1.0f; camera->up.z = 0.0f;
	camera->direction.x = 0.0f; camera->direction.y = 0.0f; camera->direction.z = -1.0f;
	camera->angle = M_PI/2;
 
}

void GameGL::OrientCamera(unsigned char key)
{
	
	switch(key)
	{
	case 'i': // inside

		camera->center.x = _playerPos.x + _speed * sin(_player->getAngle());
		camera->center.y = _playerPos.y +2.5;
		camera->center.z = _playerPos.z + _speed * cos(_player->getAngle());
		camera->eye.x = _playerPos.x - (0.5- _speed) * sin(_player->getAngle()) + camera->up.x;
		camera->eye.y = _playerPos.y +2.5;
		camera->eye.z = _playerPos.z  - (0.5- _speed)* cos(_player->getAngle());


		break;
	case'o': // outside
		if(_playerPos.y > 0 && _speed > 0.05)
		{
			camera->eye.x = camera->center.x - camera->direction.x * 7;
			camera->eye.y = _playerPos.y -3;
			camera->eye.z = camera->center.z - camera->direction.z * 7;
			camera->center.x = _playerPos.x;
			camera->center.y = _playerPos.y;
			camera->center.z = _playerPos.z;
			camera->up.y = 1;
			if(camera->eye.y <=0.5)
				camera->eye.y = 0.3;
		}
		else
		{
			camera->eye.x = _playerPos.x + 5;
			camera->eye.y = 1;
			camera->eye.z = _playerPos.z + 3;
			camera->center.x = _playerPos.x;
			camera->center.y = 1;
			camera->center.z = _playerPos.z;
			camera->up.y =1;
		}

		break;
	case'p': // 3rd person
		camera->eye.x = camera->center.x - camera->direction.x * 7;
		camera->eye.y = _playerPos.y +3;
		camera->eye.z = camera->center.z - camera->direction.z * 7;
		camera->center.x = _playerPos.x;
		camera->center.y = _playerPos.y;
		camera->center.z = _playerPos.z;
		camera->up.y = 1;
		break;
	case's':
		switch(_state)
		{
		case 0:
			if(camera->eye.z > 10.0)
			{
				camera->eye.z -= 0.1;
			}
			if(camera->eye.z > 10.0)
			{
				camera->eye.y += 0.01;
			}
			else
			{
				camera->eye.x = 8*cos(camera->angle);
				camera->eye.z = 8*sin(camera->angle);
				camera->angle += 0.005;
			}
			camera->center.x = 0;
			camera->center.y = 0;
			camera->center.z = 0;
			break;
		case 1:
			camera->center.x = 50;
			camera->center.y = 0;
			camera->center.z = 100;
			camera->eye.x = camera->center.x - 7*cos(camera->angle);
			camera->eye.z = camera->center.z - 7*sin(camera->angle);
			camera->angle += 0.005;
			break;
		case 2:
			camera->center.x = 0;
			camera->center.y = 0;
			camera->center.z = 0;
			camera->eye.x = 8*cos(camera->angle);
			camera->eye.z = 8*sin(camera->angle);
			camera->angle += 0.005;
			break;
		case 3:
			_view = 'p';
			break;
		}
		break;
	}
}
void GameGL::DrawString(const char* text, Vector3 * position, Color * colour)
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	
	glMatrixMode(GL_PROJECTION);
    glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0.0, _width, 0.0, _height);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity();
			glTranslatef(position->x, position->y, position->z);
			glRasterPos2f(0.0f, 0.0f);
			glColor3f(colour->r, colour->g, colour->b);
			glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)text);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
  

	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
}
void GameGL::HUD()
{
	ostringstream stream;
    
	switch(_state)
	{
	case 0:
		stream << "Welcome to Flight Simulator"<< endl << "Press Enter to continue...";
		_textPos.x = _width/2 - stream.str().length()*2;
		_textPos.y = _height/2 + 150;
		DrawString(stream.str().c_str(), &_textPos, &_textColour);
		stream.str("");
		if(_difficulty == 0)
			stream << "Difficulty: Easy";
		else if(_difficulty == 1)
			stream << "Difficulty: Hard";
		_textPos.x = _width - 170;
		_textPos.y = _height - 20;
		DrawString(stream.str().c_str(), &_textPos, &_textColour);
		break;
	case 1:
		_textColour.r = 1.0; _textColour.g = 0.0; _textColour.b = 0.0;
		stream.str("");
		stream << "This is your target"<< endl << "To score Hits, drop bombs using 'B'";
		_textPos.x = _width/2 - stream.str().length()*2;
		_textPos.y = _height/2 + 250;
		DrawString(stream.str().c_str(), &_textPos, &_textColour);
		stream.str("");
		stream << "Press Enter to continue...";
		_textPos.x = _width/2 - stream.str().length()*4;
		_textPos.y = _height/2 - 150;
		DrawString(stream.str().c_str(), &_textPos, &_textColour);
	
		break;
	case 2:
		_textColour.r = 1.0; _textColour.g = 0.0; _textColour.b = 0.0;
		stream.str("");
		stream << "This is your plane"<< endl << "Fly using W, A, S & D";
		_textPos.x = _width/2 - stream.str().length()*2;
		_textPos.y = _height/2 + 250;
		DrawString(stream.str().c_str(), &_textPos, &_textColour);
		stream.str("");
		stream << "Press Enter to Start";
		_textPos.x = _width/2 - 130;
		_textPos.y = _height/2 - 150;
		DrawString(stream.str().c_str(), &_textPos, &_textColour);
		break;
	case 3:
		if(_score <5)
		{
			_textColour.r = 0.0; _textColour.g = 0.5; _textColour.b = 1.0;
			stream.str("");
			stream << "Speed: "<< _speed * 100;
			_textPos.x = 15;
			_textPos.y = 0;
			DrawString(stream.str().c_str(), &_textPos, &_textColour);
			stream.str("");
			stream << "Elevation: " << _playerPos.y;
			_textPos.x = _width - 150;
			DrawString(stream.str().c_str(), &_textPos, &_textColour);
			stream.str("");
			stream << "Hits: " << _score;
			_textPos.x = _width - 120;
			_textPos.y = _height - 20;
			DrawString(stream.str().c_str(), &_textPos, &_textColour);

			if(_speed > .35 && _speed < .45 && _playerPos.y <= 0.5)
			{
				stream.str("");
				stream << "When speed reaches 40, take off using SPACE";
				_textPos.x = _width/2 - 200;
				_textPos.y = _height/2 + 250;
				DrawString(stream.str().c_str(), &_textPos, &_textColour);
			}
		}
		else
		{
			stream << "Congratulations!"<< endl << "You have completed the simulator"<< endl<< "Press Enter to restart";
			stream.str().length();
			_textPos.x = _width/2 - stream.str().length()*2;
			_textPos.y = _height - 150;
			DrawString(stream.str().c_str(), &_textPos, &_textColour);

		}
		break;
	}

}
void GameGL::DropBomb()
{
	for (int i = 0; i < 3; i++)
	{
		if(_bomb[i]->dropped() == false)
		{
			_bomb[i]->drop(true);
			_player->removeChild();
			_bomb[i]->setRotation(_player->getRotation(), _bombRot);
			_bomb[i]->setPosition(_player->getPosition());
			_bomb[i]->setSpeed(_player->getSpeed());
			_bomb[i]->setMoveRot(_player->getAngle());
			break;
		}
	}
}
void GameGL::Ammo()
{
	Vector3 p = {0,0,0};
	for (int i = 0; i < 3; i++)
	{
		_bomb[i]->drop(false);
		_bomb[i]->hasHit(false);
		_bomb[i]->setRotation(0, p);
		_bomb[i]->setPosition(p);
		_bomb[i]->setSpeed(0.0);
		_bomb[i]->setMoveRot(0);
	}
	
	SceneNode* sbomb1 = new SceneNode(_bomb[0]);
	_player->addChild(sbomb1);
	SceneNode* sbomb2 = new SceneNode(_bomb[1]);
	_player->addChild(sbomb2);
	SceneNode* sbomb3 = new SceneNode(_bomb[2]);
	_player->addChild(sbomb3);
}
void GameGL::Collision()
{

	for (int i = 0; i < 3; i++)
	{
		if(_bomb[i]->getPosition().y <= 1.0 && _bomb[i]->getPosition().y >= 0.0  && _bomb[i]->dropped() == true && _bomb[i]->hit() == false)
		{
			if(_bomb[i]->getPosition().x < _tank->getLocation().x)
			{}
			else if(_bomb[i]->getPosition().x > _tank->getLocation().x + _tank->getSize().x)
			{}
			else if(_bomb[i]->getPosition().z < _tank->getLocation().z)
			{}
			else if(_bomb[i]->getPosition().z > _tank->getLocation().z + _tank->getSize().z)
			{}
			else
			{
				_score++;
				_bomb[i]->hasHit(true);
			}
		}
		
	}
}
void GameGL::changeSize(int w, int h)
{
	if(h == 0)
		h = 1;
	float ratio = 1.0f * w/h;
	_width = w;
	_height = h;
	_ratio = ratio;

	 // Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);
	// Reset Matrix
	glLoadIdentity();
	// Set the viewport to be the entire window
	glViewport(0,0, w, h);
	// set the correct perspective
	gluPerspective(100, ratio, 0.1, 10000);
	// Get back to the ModelView
	glMatrixMode(GL_MODELVIEW);

}
void GameGL::reset()
{
	if(_bomb[2]->dropped() == true)
		Ammo();
	_score = 0;
	_player->reset();
	_player->Update();

	InitCamera();
	_speed = 0.0;
	_view = 's';
	_state = 0;
}
