#include "Time.h"
using namespace DirectX;

float Time::deltaTime = 0.0f;
float Time::totalTime = 0.0f;
float Time::lastFrameTime = 0.0f;

void Time::Update(D3D_DRIVER_TYPE driverType)
{
    static float t = 0.0f;

    if (driverType == D3D_DRIVER_TYPE_REFERENCE)
    {
        t += (float)XM_PI * 0.0125f;
    }
    else
    {
        static DWORD dwTimeStart = 0;
        DWORD dwTimeCur = GetTickCount();

        if (dwTimeStart == 0)
            dwTimeStart = dwTimeCur;

        t = (dwTimeCur - dwTimeStart) / 1000.0f;
    }

    deltaTime = t - lastFrameTime;
    totalTime = t;
    lastFrameTime = t;
}
