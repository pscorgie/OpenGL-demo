#pragma once
#include <stdio.h>
#include "MovingObject.h"

struct node
{
	SceneObject* object;
	node * next;
};

class LinkList
{
private:
	node * _head;
public:
	LinkList(SceneObject* data);
	~LinkList();
	node * makeNode(SceneObject *object);
	void deleteList(node * p);
	node * insertFirst(node * newHead);
	node * getNode(node* p, int pos);
	void insertNode(node * p, node * newNode);
	void deleteAfter(node * p);
	void update(node* p);
	void draw(node* p);
	node * returnHead();
};

