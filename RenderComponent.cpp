#include "RenderComponent.h"

RenderComponent::RenderComponent(GameObject* holder) : Component(holder)
{
	renderer._Transform = holder->GetTransform()->GetWorldTransform();
}

void RenderComponent::Update(float deltaTime)
{

}





