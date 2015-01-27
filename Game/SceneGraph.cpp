#include "SceneGraph.h"
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

SceneGraph::SceneGraph(SceneNode* m)
{
	_parent = m;
}


SceneGraph::~SceneGraph(void)
{
	delete _parent;
}

void SceneGraph::addChild(SceneNode* child)
{
	_parent->children.insert(_parent->children.end(), child);
}
void SceneGraph::addChild(SceneNode* parent, SceneNode* child)
{
	parent->children.push_back(child);
}
SceneNode* SceneGraph::removeChild()
{
	SceneNode* p = _parent->children.back();
	_parent->children.pop_back();
	return p;
}