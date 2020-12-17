#pragma once
#include <vector>
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"

using namespace DirectX;

class GameObject;
class Renderer;
class Shader;
class Texture;
class Camera;
class MeshData;

class Graphics
{
private:

	D3D_DRIVER_TYPE         _driverType;
	D3D_FEATURE_LEVEL       _featureLevel;

	//Important rendering stuff:
	ID3D11Device* _pd3dDevice;
	ID3D11DeviceContext* _pImmediateContext;
	IDXGISwapChain* _pSwapChain;
	ID3D11RenderTargetView* _pRenderTargetView;

	//depth stencil buffer:
	ID3D11DepthStencilView* _depthStencilView;
	ID3D11Texture2D* _depthStencilBuffer;

	//wireframe/filled render state.
	ID3D11RasterizerState* _wireFrame;
	ID3D11RasterizerState* _filled;

	//main buffers for render pipeline.
	ID3D11Buffer* _pConstantBuffer;
	ID3D11Buffer* _pLightBuffer;

	//default shader, texture and sampler
	Shader* _pDefaultShader;
	Texture* _pDefaultTexture;
	ID3D11SamplerState* _pDefaultSampler;

	//Draw matrices
	XMFLOAT4X4              _pWorld;

	//objects to Draw:
	std::vector<Renderer*> _pRenderers;

	//textures/shaders/models (to delete at clean up step).
	std::vector<MeshData*> _pMeshes;
	std::vector<Texture*> _pTextures;
	std::vector<Shader*> _pShaders;
	std::vector<ID3D11SamplerState*> _pSamplers;

private:

	//test run:
	MeshData* carModel;
	GameObject* car;

	void LoadDefaults();

	HRESULT CreateBuffer(ID3D11Buffer** buffer, UINT bufferSize, UINT bindFlags = D3D11_BIND_CONSTANT_BUFFER);
	void BindConstantBuffer(ID3D11Buffer** buffer, int slot, bool isVSBuffer, bool isPSBuffer);

	//cleanup:
	void Cleanup();
	void DeleteEntities();

public:

	HRESULT Init(HWND window,UINT windowWidth, UINT windowHeight);

	Shader* CreateShader(WCHAR* file, BOOL bind = false);
	Texture* CreateTexture(WCHAR* file);
	MeshData* CreateMesh(char* file);
	ID3D11SamplerState* CreateSampler(D3D11_FILTER filter, BOOL bind = false);

	Shader* GetDefaultShader() { return _pDefaultShader; };
	Texture* GetDefaultTexture() { return _pDefaultTexture; };

	void BindTexture(ID3D11ShaderResourceView* texture, int slot);
	XMFLOAT4X4 SetWorld(XMFLOAT4X4& world) { _pWorld = world; };

public:

	void Update();
	void Draw();

	~Graphics();
	void AddRenderer(Renderer* renderer);
	void RemoveRenderer(Renderer* renderer, bool deleteAfter);
};

