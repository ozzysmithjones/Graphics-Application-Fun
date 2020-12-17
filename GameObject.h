#pragma once

#include "Structures.h"
#include "Transform.h"
#include <iostream>
#include <map>
#include <vector>

//Component based > OOP based.
class Component;

class GameObject
{
private:

	std::map<int, Component*> componentsByID;
	std::vector<Component*> components;
	std::vector<GameObject*> children;

	GameObject* _pParent;
	Transform* transform;

public:

	Transform* GetTransform() { return transform; };

	GameObject* GetParent() { return _pParent; };
	void SetParent(GameObject* parent);

	void Update(float deltaTime);

	GameObject();
	~GameObject();

	template<typename T> void AddComponent(T* component) 
	{
		int id = GetComponentID<T>();

		componentsByID[id] = component;
		components.push_back(component);
	}

	template<typename T> T* RemoveComponent(bool deleteAfter = true)
	{
		int id = GetComponentID<T>();
		T* component = componentsByID[id];

		componentsByID.erase(id);
		components.erase(id);

		if (deleteAfter) 
		{
			delete component;
			return nullptr;
		}

		return component;
	}

	template<typename T> T* GetComponent() const 
	{
		int id = GetComponentID<T>();

		if (componentsByID.find(id) != componentsByID.end())
		{
			return componentsByID[id];
		}

		return nullptr;
	}

	template<typename T> bool HasComponent() const
	{
		int id = GetComponentID<T>();
		return componentsByID.find(id) != componentsByID.end();
	}
};

