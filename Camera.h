#pragma once
#include "Structures.h"
#include "Component.h"

class Camera : public Component
{
private:

	XMFLOAT3 _offset;
	XMFLOAT3 _eye, _at, _up;
	XMFLOAT3 _rotation;

	FLOAT _windowWidth;
	FLOAT _windowHeight;
	FLOAT _nearDepth;
	FLOAT _farDepth;

	XMFLOAT4X4 _view, _projection;

	float maxFollowDistance = 5.0f;
	bool isFollowCamera = false;

public:

	static Camera* Main;

	Camera(XMFLOAT3 position, XMFLOAT3 at, XMFLOAT3 up, FLOAT windowWidth, FLOAT windowHeight, FLOAT neardepth, FLOAT farDepth);
	Camera(GameObject* holder,XMFLOAT3 position, XMFLOAT3 at, XMFLOAT3 up, FLOAT windowWidth, FLOAT windowHeight, FLOAT neardepth, FLOAT farDepth);
	~Camera();

	void SetPosition(XMFLOAT3 position) { _eye = position; };
	XMFLOAT3 GetPosition() { return _eye; };

	void Translate(XMFLOAT3 translation, bool isLocal);
	void Rotate(XMFLOAT3 rotation);

	void SetLookAtPosition(XMFLOAT3 at) { _at = at; };
	XMFLOAT3 GetLookAtPosition() { return _at; };

	void SetUpDirection(XMFLOAT3 up) { _up = up; };
	XMFLOAT3 GetUpDirection() { return _up; };

	XMFLOAT4X4 GetProjection() { return _projection; };
	XMFLOAT4X4 GetView() { return _view; };
	XMFLOAT4X4 GetViewProjection();

	void ReShape(FLOAT WindowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth);

	// Inherited via Component
	virtual void Update(float deltaTime) override;

};

