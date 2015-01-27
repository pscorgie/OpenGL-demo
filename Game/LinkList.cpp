#include "LinkList.h"


LinkList::LinkList(SceneObject* data)
{
	_head = new node;
	_head->object = data;
	_head->next = nullptr;

}


LinkList::~LinkList()
{
	deleteList(_head);
	delete _head;
}

node * LinkList::makeNode(SceneObject * data)
{
	node * newNode;
	newNode = new node;
	newNode->object = data;
	newNode->next = nullptr;
	return newNode;
}

void LinkList::deleteList(node * p)
{
	node * tempNode;
	while(p != nullptr)
	{
		tempNode = p;
		p = p->next;
		delete tempNode;
		tempNode = nullptr;
	}
	
}
node * LinkList::insertFirst(node * newHead)
{
	// creates new list head
	newHead->next = _head;
	_head = newHead;
	return newHead;
}
node * LinkList::getNode(node* p, int pos)
{
	int count = 0;
	node * tempNode = p;
	while(tempNode != nullptr)
	{
		if(count != pos)
		{
			return tempNode;
		}
		count++;
		tempNode = tempNode->next;
	}
	return nullptr;
}
void LinkList::insertNode(node * p, node * newNode)
{
	p->next = newNode;
}
void LinkList::deleteAfter(node * p)
{
	node * tempNode;
	if (p != nullptr && p->next != nullptr)
	{
		tempNode = p->next;
		p->next = tempNode->next;
		delete tempNode;
		tempNode = nullptr;
	}
}
void LinkList::update(node * p)
{
	p->object->Update();
	p = p->next;
	if(p != nullptr)
	{
		update(p);
	}

	return;
}
void LinkList::draw(node * p)
{
	p->object->Draw();
	p = p->next;
	if(p != nullptr)
	{
		draw(p);
	}

	return;
}
node * LinkList::returnHead()
{
	return _head;
}
