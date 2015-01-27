#pragma once
#include "Structures.h"
#include "MovingObject.h"
#include <vector>

struct SceneNode
{
	MovingObject* object;
	std::vector<SceneNode*> children;
	SceneNode(MovingObject* m){object = m;}
};

class SceneGraph
{
protected:
	SceneNode* _parent;

public:
	SceneGraph(SceneNode* m);
	~SceneGraph(void);
	void addChild(SceneNode* child);
	void addChild(SceneNode* parent, SceneNode* child);
	SceneNode* removeChild();

};

