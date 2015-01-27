#pragma once
class GameGL; // Forward declare Game

namespace GLUTCallbacks
{
	void Init(GameGL * gl);
	void Display();
	void Timer(int preferredRefresh);
	void Keyboard(unsigned char key, int x, int y);
	void KeyboardUp(unsigned char key, int x, int y);
	void Reshape(int w, int h);
}

