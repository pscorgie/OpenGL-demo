#include "GLUTCallbacks.h"
#include "GameGL.h"

namespace GLUTCallbacks
{
	namespace
	{
		GameGL * gameGL = nullptr;
	}

	void Init(GameGL * gl)
	{
		gameGL = gl;
	}
	void Display()
	{
		if(gameGL != nullptr)
			gameGL->Display();
	}
	void Timer(int preferredRefresh)
	{
		int updateTime = glutGet(GLUT_ELAPSED_TIME);
		gameGL->Update();
		updateTime = glutGet(GLUT_ELAPSED_TIME) - updateTime;
		glutTimerFunc(preferredRefresh - updateTime, GLUTCallbacks::Timer, preferredRefresh);
	}
	void Keyboard(unsigned char key, int x, int y)
	{
		gameGL->Keyboard(key,x,y);
	}
	void KeyboardUp(unsigned char key, int x, int y)
	{
		gameGL->KeyboardUp(key,x,y);
	}

	void Reshape(int w, int h)
	{
		gameGL->changeSize(w,h);
	}
}
