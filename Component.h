#pragma once
#include <iostream>

#include "Structures.h"
#include "GameObject.h"


//this static method keeps track of a static integer ID, and increments it by 1.
inline int NextComponentID() 
{
	static int lastID = 0;
	return lastID++;
}

//this template function will return a unique integer ID for every component type that uses it.
template<typename T> inline int GetComponentID() 
{
	static int id = NextComponentID();
	return id;
}

class Component 
{
protected:
	GameObject* gameObject;
public:

	Component(GameObject* gameObject) { this->gameObject = gameObject; };

	virtual void Update(float deltaTime) = 0;
};