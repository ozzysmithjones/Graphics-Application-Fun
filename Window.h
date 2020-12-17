#pragma once
#include <windows.h>
#include "resource.h"

class Window
{
private:
	HINSTANCE               _hInst;
	HWND                    _hWnd;
public:
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HWND GetWindow() { return _hWnd; };

	UINT _WindowWidth;
	UINT _WindowHeight;
};

