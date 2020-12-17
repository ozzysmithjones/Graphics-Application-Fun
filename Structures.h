#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <directxmath.h>
#include "Vector.h"

using namespace DirectX;

#define NUM_LIGHTS 2
#define PI 3.14159265

class MeshData
{
public:

	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;
	UINT VBStride;
	UINT VBOffset;
	UINT IndexCount;
};

struct Light 
{
	XMFLOAT4 Diffuse, Ambient, Specular;
	XMFLOAT4 Transform;
};

struct SpotLight 
{
	XMFLOAT4 Diffuse, Ambient, Specular;
	XMFLOAT4 Position;
	XMFLOAT4 Direction;
};

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 TexC;

	bool operator<(const SimpleVertex other) const
	{
		return memcmp((void*)this, (void*)&other, sizeof(SimpleVertex)) > 0;
	};
};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;

	XMFLOAT4 DiffuseMtrl;
	XMFLOAT4 AmbientMtrl;
	XMFLOAT4 specularMtrl;

	XMFLOAT3 EyePosW;
	float time;
};

struct LightBuffer
{
	Light Lights[NUM_LIGHTS];
	SpotLight spotLight;
};


