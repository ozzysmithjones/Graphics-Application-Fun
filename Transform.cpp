#include "Transform.h"

void Transform::CalculateWorld()
{
	if (_pParent != nullptr) 
	{
		XMStoreFloat4x4(_pWorld, XMLoadFloat4x4(_pParent->GetWorldTransform()) * XMLoadFloat4x4(_pLocal));
	}else
	{
		XMStoreFloat4x4(_pWorld, XMLoadFloat4x4(_pLocal));
	}
}

void Transform::SetParent(Transform* parent)
{
	_pParent = parent;
	CalculateWorld();
}

void Transform::Translate(XMFLOAT3 translation, bool isLocal)
{
	if (isLocal) 
	{
		Vector up = Vector(_pUp);
		Vector forward = Vector(_pForward);//(Vector(_at) - Vector(_eye)).Normalised();
		Vector right = Vector::Cross(up, forward);
		Vector vec = Vector(translation);

		translation = right * vec.x + up * vec.y + forward * vec.z;
	}

	XMStoreFloat4x4(_pLocal, XMLoadFloat4x4(_pLocal) * XMMatrixTranslation(translation.x,translation.y,translation.z));
	CalculateWorld();
}

void Transform::Rotate(XMFLOAT3 rotation)
{
	//update forward direction.
	_pRotation = XMFLOAT3(_pRotation.x + rotation.x, _pRotation.y + rotation.y, _pRotation.z + rotation.z);
	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(_pRotation.x, _pRotation.y, _pRotation.z);
	XMStoreFloat3(&_pForward, XMVector3TransformCoord(XMVectorSet(0, 0, 1, 0), camRotationMatrix));

	//rotate
	XMStoreFloat4x4(_pLocal, XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) * XMLoadFloat4x4(_pLocal));
	CalculateWorld();
}

void Transform::SetLocalTransform(XMFLOAT4X4& local)
{
	*_pLocal = local;
	CalculateWorld();
}

void Transform::SetWorldTransform(XMFLOAT4X4& transform)
{
	XMFLOAT4X4 diff;
	XMStoreFloat4x4(&diff, XMMatrixInverse(nullptr, XMLoadFloat4x4(_pWorld) * XMLoadFloat4x4(&transform)));
	XMStoreFloat4x4(_pLocal, XMLoadFloat4x4(_pLocal) * XMLoadFloat4x4(&diff));
	CalculateWorld();
}

Transform::Transform()
{
	_pRotation = XMFLOAT3(0, 0, 0);
	_pRight = XMFLOAT3(1, 0, 0);
	_pUp = XMFLOAT3(0, 1, 0);
	_pForward = XMFLOAT3(0, 0, 1);


	_pParent = nullptr;
	_pLocal = new XMFLOAT4X4();
	_pWorld = new XMFLOAT4X4();
	XMStoreFloat4x4(_pLocal, XMMatrixIdentity());
	XMStoreFloat4x4(_pWorld, XMMatrixIdentity());
}

Transform::~Transform()
{
	delete _pLocal;
	delete _pWorld;
}
