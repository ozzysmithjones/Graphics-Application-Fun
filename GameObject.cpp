#include "GameObject.h"
#include "Component.h"
void GameObject::SetParent(GameObject* parent)
{
	if (_pParent != nullptr) 
	{
		for (int i = 0; i < _pParent->children.size(); i++) 
		{
			if (_pParent->children[i] == this)
			{
				_pParent->children.erase(_pParent->children.begin() + i);
				break;
			}
		}
	}

	if (parent != nullptr)
	{
		parent->children.push_back(this);
		transform->SetParent(parent->GetTransform());
	}
	else {
		transform->SetParent(nullptr);
	}

	_pParent = parent;
	
}

void GameObject::Update(float deltaTime)
{
	if (!components.empty()) 
	{
		//worldTransform = CalculateWorldTransform();
		for (int i = 0; i < components.size(); i++)
		{
			components[i]->Update(deltaTime);
		}
	}

	if (!children.empty()) 
	{
		for (int i = 0; i < children.size(); i++)
		{
			children[i]->Update(deltaTime);
		}
	}
}

GameObject::GameObject()
{
	transform = new Transform();

}

GameObject::~GameObject()
{
	for (int i = 0; i < children.size(); i++) 
	{
		delete children[i];
	}

	for (int i = 0; i < components.size(); i++) 
	{
		delete components[i];
	}

	components.clear();
	componentsByID.clear();

	delete transform;
}
