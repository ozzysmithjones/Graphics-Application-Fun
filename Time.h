#pragma once
#include <d3dcommon.h>
#include <DirectXMath.h>

static class Time
{
private:
	static float lastFrameTime;

public:
	static float deltaTime;
	static float totalTime;

	static void Update(D3D_DRIVER_TYPE driverType);
};

