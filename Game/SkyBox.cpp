#include "SkyBox.h"


SkyBox::SkyBox(void)
{
	x = 0;
	y = 0;
	z = 0;
	width = 10000;
	height = 10000;
	length = 10000;

	x = x - width /2;
	y = y - height /2;
	z = z - length /2;

	loadTex();
}


SkyBox::~SkyBox(void)
{
	delete _skyUp;
	delete _skyDown;
	delete _skyFront;
	delete _skyBack;
	delete _skyLeft;
	delete _skyRight;
}
void SkyBox::Draw()
{
	// Draw Front side
	glBindTexture(GL_TEXTURE_2D, _skyFront->GetID());
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height, z+length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height, z+length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z+length);
	glEnd();
	

	// Draw Back side
	glBindTexture(GL_TEXTURE_2D, _skyBack->GetID());
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height, z); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
	glEnd();
	

	// Draw Left side
	glBindTexture(GL_TEXTURE_2D, _skyLeft->GetID());
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z+length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z);		
	glEnd();
	

	// Draw Right side
	glBindTexture(GL_TEXTURE_2D, _skyRight->GetID());
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height,	z+length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height,	z);
	glEnd();
	

	// Draw Up side
	glBindTexture(GL_TEXTURE_2D, _skyUp->GetID());
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y+height, z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y+height, z+length); 
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z+length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z);
	glEnd();
	

	// Draw Down side
	glBindTexture(GL_TEXTURE_2D, _skyDown->GetID());
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y,		z+length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y,		z);
	glEnd();
}
void SkyBox::Update(float posx, float posy, float posz)
{
	x = posx - width /2;
	y = posy - height /2;
	z = posz - length /2;
}
void SkyBox::loadTex()
{
	_skyUp = new Texture2D();
	_skyUp->LoadBMP("Textures/txStormyDays_up.bmp");

	_skyDown = new Texture2D();
	_skyDown->LoadBMP("Textures/txStormyDays_down.bmp");

	_skyFront = new Texture2D();
	_skyFront->LoadBMP("Textures/txStormyDays_front.bmp");

	_skyBack = new Texture2D();
	_skyBack->LoadBMP("Textures/txStormyDays_back.bmp");

	_skyLeft = new Texture2D();
	_skyLeft->LoadBMP("Textures/txStormyDays_left.bmp");

	_skyRight = new Texture2D();
	_skyRight->LoadBMP("Textures/txStormyDays_right.bmp");
}