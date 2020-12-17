#pragma once
#include "DDSTextureLoader.h"

using namespace DirectX;

class Texture
{
private:
	ID3D11ShaderResourceView* _pTexture;
public:

	void Use(ID3D11DeviceContext* context);
	HRESULT CompileTexture(WCHAR* filePath, ID3D11Device* _pd3dDevice);

	~Texture();
};

