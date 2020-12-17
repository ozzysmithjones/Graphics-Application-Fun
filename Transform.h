#pragma once
#include "Structures.h"

using namespace DirectX;

class Transform
{
private:

	Transform* _pParent;
	XMFLOAT4X4* _pLocal;
	XMFLOAT4X4* _pWorld;

	XMFLOAT3 _pRotation;
	XMFLOAT3 _pForward, _pRight, _pUp;


private:

	void CalculateWorld();

private:


public:

	void Translate(XMFLOAT3 translation, bool isLocal);
	void Rotate(XMFLOAT3 rotation);

	void SetParent(Transform* parent);

	XMFLOAT4X4* GetLocalTransform() { return _pLocal; };
	void SetLocalTransform(XMFLOAT4X4& local);

	XMFLOAT4X4* GetWorldTransform() { return _pWorld; };
	void SetWorldTransform(XMFLOAT4X4& transform);

	XMFLOAT3 GetForwardVector() { return _pForward; };
	XMFLOAT3 GetRotation() { return _pRotation; };
	XMFLOAT3 GetPosition() { return XMFLOAT3((*_pWorld)(3,0), (*_pWorld)(3, 1), (*_pWorld)(3, 2)); };

	Transform();
	~Transform();
};

