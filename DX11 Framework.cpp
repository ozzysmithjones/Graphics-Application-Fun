//#include "Application.h"
//#include "Graphics.h";
//#include "Window.h";
#include "Game.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    Game* game = new Game();
	//Application * theApp = new Application();

    /*
	if (FAILED(theApp->Initialise(hInstance, nCmdShow)))
	{
		return -1;
	}
    */

    if (FAILED(game->Init(hInstance,nCmdShow)))
    {
        delete game;
        return -1;
    }

    // Main message loop
    MSG msg = {0};

    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            game->Update();
            game->Draw();

			//theApp->Update();
            //theApp->Draw();
        }
    }

    delete game;
	//delete theApp;
	//theApp = nullptr;

    return (int) msg.wParam;
}