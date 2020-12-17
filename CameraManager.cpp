#include "CameraManager.h"
#include "Camera.h"
void CameraManager::SetCameraControl(bool isCameraControl)
{
    _pIsCameraControl = isCameraControl;
}

void CameraManager::Init(UINT windowWidth, UINT windowHeight)
{
    _pIsCameraControl = true;
    _pIsPressingSpace = false;
    _pCameraIndex = 0;
    for (int i = 0; i < 1; i++)
    {
        _pCameras.push_back(new Camera(nullptr, XMFLOAT3(i * 2, 0, -20), XMFLOAT3(0, 0, 1), XMFLOAT3(0, 1, 0), windowWidth, windowHeight, 0.01f, 150.0f));
    }

    Camera::Main = _pCameras[_pCameraIndex];
}

void CameraManager::Update(float deltaTime)
{
    if (_pIsCameraControl) 
    {

        if (GetAsyncKeyState(VK_UP) < 0)
        {
            Camera::Main->Translate(XMFLOAT3(0, 0, 30.0f * deltaTime), true);
        }

        if (GetAsyncKeyState(VK_DOWN) < 0)
        {
            Camera::Main->Translate(XMFLOAT3(0, 0, -30.0f * deltaTime), true);
        }

        if (GetAsyncKeyState(VK_LEFT) < 0)
        {
            Camera::Main->Rotate(XMFLOAT3(0, -10.f * deltaTime, 0));
        }

        if (GetAsyncKeyState(VK_RIGHT) < 0)
        {
            Camera::Main->Rotate(XMFLOAT3(0, 10.0f * deltaTime, 0));
        }

    }

    if (GetAsyncKeyState(VK_SPACE) < 0 && !_pIsPressingSpace)
    {
        _pIsPressingSpace = true;
        _pCameraIndex++;

        if (_pCameraIndex >= _pCameras.size())
            _pCameraIndex = 0;

        Camera::Main = _pCameras[_pCameraIndex];
    }
    else if (GetAsyncKeyState(VK_SPACE) == 0 && _pIsPressingSpace)
    {
        _pIsPressingSpace = false;
    }

    for (int i = 0; i < _pCameras.size(); i++)
    {
        _pCameras[i]->Update(deltaTime);
    }
}

void CameraManager::AddCamera(Camera* camera)
{
    _pCameras.push_back(camera);
}

CameraManager::~CameraManager()
{
    for (auto p : _pCameras)
    {
        delete p;
    }
}
