#pragma once
#include <iostream>
#include <vector>

#include "Component.h"
#include "Renderer.h"

class RenderComponent :
	public Component
{
public:

	Renderer renderer;

public:

	RenderComponent(GameObject* holder);

	// Inherited via Component
	void Update(float deltaTime) override;
};

