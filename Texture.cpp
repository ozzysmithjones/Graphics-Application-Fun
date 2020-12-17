#include "Texture.h"

void Texture::Use(ID3D11DeviceContext* context)
{
	context->PSSetShaderResources(0, 1, &_pTexture);
}

HRESULT Texture::CompileTexture(WCHAR* filePath, ID3D11Device* _pd3dDevice)
{
	return CreateDDSTextureFromFile(_pd3dDevice, filePath, nullptr, &_pTexture);
}

Texture::~Texture()
{
	if (_pTexture)_pTexture->Release();
}
