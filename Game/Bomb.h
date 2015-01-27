#pragma once
#include "MovingObject.h"

class Bomb : public MovingObject
{
private:
	bool _dropped, _hit;
public:
	Bomb(TexturedMesh * mesh, Texture2D * texture, int type);
	~Bomb(void);

	void Draw();
	void Update();
	void drop(bool b){_dropped = b;}
	bool dropped(){return _dropped;}
	void hasHit(bool b){_hit = b;}
	bool hit(){return _hit;}
	
};

