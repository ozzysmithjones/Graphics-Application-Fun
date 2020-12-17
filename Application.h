#pragma once
/*
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"

#include "Structures.h"

//#include "ComponentTypes.h"
//#include "GameObject.h"
#include "Camera.h"

#include "DDSTextureLoader.h"
#include "OBJLoader.h"

#include "Time.h"
#include "Shader.h"
#include "Game.h"

using namespace DirectX;

class Application
{
private:

	HINSTANCE               _hInst;
	HWND                    _hWnd;
	D3D_DRIVER_TYPE         _driverType;
	D3D_FEATURE_LEVEL       _featureLevel;

	ID3D11Device* _pd3dDevice;
	ID3D11DeviceContext*    _pImmediateContext;
	IDXGISwapChain*         _pSwapChain;
	ID3D11RenderTargetView* _pRenderTargetView;

	ID3D11Buffer*           _pConstantBuffer;
	ID3D11Buffer*			_pLightBuffer;

	ID3D11DepthStencilView* _depthStencilView;
	ID3D11Texture2D*		_depthStencilBuffer;

	ID3D11RasterizerState*  _wireFrame;
	ID3D11RasterizerState*  _filled;

	//Textures:
	ID3D11ShaderResourceView* _pTextureRV = nullptr;
	ID3D11SamplerState*		_pSamplerLinear = nullptr;

	//shader:
	Shader* shader;

	//Game:
	Game* game;

	XMFLOAT4X4              _world;

private:

	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitDevice();

	HRESULT CreateBuffer(ID3D11Buffer** buffer,UINT bufferSize,UINT bindFlags = D3D11_BIND_CONSTANT_BUFFER);
	void BindConstantBuffer(ID3D11Buffer** buffer, int slot, bool isVSBuffer, bool isPSBuffer);

	void Cleanup();

	HRESULT InitRenderers();
	HRESULT LoadTextures();

	UINT _WindowHeight;
	UINT _WindowWidth;

public:
	Application();
	~Application();

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	void Update();
	void Draw();
};
*/


