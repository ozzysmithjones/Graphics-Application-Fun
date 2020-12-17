#pragma once
#include <vector>
#include "Graphics.h"
#include "Window.h"

#include "GameObject.h"
#include "CameraManager.h"
#include "Camera.h"
#include "Time.h"
#include "Renderer.h"


class Game
{

private:

	CameraManager* cameraManager;
	GameObject* spaceship;
	GameObject* prop;

	MeshData* carModel;

	Window* window;
	Graphics* graphics;
	GameObject* scene;

	HRESULT InitEntities();
	void CleanUp();

public:

	HRESULT Init(HINSTANCE hInstance, int nCmdShow);
	void Start();
	void Update();
	void Draw();

	~Game();
};




