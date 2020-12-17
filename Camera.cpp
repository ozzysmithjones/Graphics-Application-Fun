#include "Camera.h"

Camera* Camera::Main = nullptr;

Camera::Camera(XMFLOAT3 position, XMFLOAT3 at, XMFLOAT3 up, FLOAT windowWidth, FLOAT windowHeight, FLOAT neardepth, FLOAT farDepth) : Component(nullptr)
{
	maxFollowDistance = 5.0f;
	_rotation = XMFLOAT3(0, 0, 0);
	_offset = XMFLOAT3(0, 0, 0);
	_eye = position;
	_at = at;
	_up = up;


	_windowWidth = windowWidth;
	_windowHeight = windowHeight;
	_nearDepth = neardepth;
	_farDepth = farDepth;
}

Camera::Camera(GameObject* holder, XMFLOAT3 position, XMFLOAT3 at, XMFLOAT3 up, FLOAT windowWidth, FLOAT windowHeight, FLOAT neardepth, FLOAT farDepth) : Component(holder)
{
	maxFollowDistance = 5.0f;
	_rotation = XMFLOAT3(0, 0, 0);
	_offset = XMFLOAT3(0, 0, 0);
	_eye = position;
	_at = at;
	_up = up;

	_windowWidth = windowWidth;
	_windowHeight = windowHeight;
	_nearDepth = neardepth;
	_farDepth = farDepth;

	if (holder != nullptr) {
		isFollowCamera = true;
	}
}



Camera::~Camera()
{

}

void Camera::Translate(XMFLOAT3 translation, bool isLocal)
{
	if (isLocal) 
	{
		Vector up = Vector(_up);
		Vector forward = Vector(_at);//(Vector(_at) - Vector(_eye)).Normalised();
		Vector right = Vector::Cross(up,forward);
		Vector vec = Vector(translation);

		translation = right * vec.x + up * vec.y + forward * vec.z;
	}
	_offset = XMFLOAT3(_offset.x + translation.x, _offset.y + translation.y, _offset.z + translation.z);
}

void Camera::Rotate(XMFLOAT3 rotation)
{
	_rotation = XMFLOAT3(_rotation.x + rotation.x, _rotation.y + rotation.y, _rotation.z + rotation.z);
	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(_rotation.x, _rotation.y, 0);
	XMStoreFloat3(&_at,XMVector3TransformCoord(XMVectorSet(0, 0, 1, 0), camRotationMatrix));
}


XMFLOAT4X4 Camera::GetViewProjection()
{
	XMFLOAT4X4 viewProjection;
	XMStoreFloat4x4(&viewProjection, XMLoadFloat4x4(&_projection) * XMLoadFloat4x4(&_view));
	return viewProjection;
}

void Camera::ReShape(FLOAT WindowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth)
{
	_windowHeight = windowHeight;
	_windowWidth = WindowWidth;

	_nearDepth = nearDepth;
	_farDepth = farDepth;

	XMStoreFloat4x4(&_projection, XMMatrixPerspectiveFovLH(XM_PIDIV2, _windowWidth / (FLOAT)_windowHeight, nearDepth, farDepth));
}

void Camera::Update(float deltaTime)
{
	if (isFollowCamera)
	{
		Transform* trans = gameObject->GetTransform();
		Vector diff = (Vector(trans->GetPosition()) - Vector(_eye));
		Vector max = Vector::Clamp(diff, 20.0f);

		_eye = Vector(trans->GetPosition()) - (Vector(trans->GetForwardVector()) * 20.0f);
		_at = trans->GetForwardVector();
	}

	//Update view & projection matrices.

	XMVECTOR Eye = XMVectorSet(_eye.x + _offset.x, _eye.y + _offset.y, _eye.z + _offset.z, 0.0f);
	XMVECTOR At = XMVectorSet(_at.x, _at.y, _at.z, 0.0f);
	XMVECTOR Up = XMVectorSet(_up.x, _up.y, _up.z, 0.0f);

	XMStoreFloat4x4(&_view, XMMatrixLookToLH(Eye, At, Up));
	XMStoreFloat4x4(&_projection, XMMatrixPerspectiveFovLH(XM_PIDIV2, _windowWidth / (FLOAT)_windowHeight, _nearDepth, _farDepth));
}


