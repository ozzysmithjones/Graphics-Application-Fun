#pragma once
#include "Window.h"
#include "vector"
class Camera;

class CameraManager
{
private:

	int _pCameraIndex;
	bool _pIsCameraControl;
	bool _pIsPressingSpace;
	std::vector<Camera*> _pCameras;

public:

	void SetCameraControl(bool isCameraControl);
	void Init(UINT windowWidth, UINT windowHeight);
	void Update(float deltaTime);

	void AddCamera(Camera* camera);

	~CameraManager();
};

