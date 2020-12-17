#pragma once
#include "Structures.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"

#include <windows.h>
#include <d3d11_1.h>


struct Renderer
{
	XMFLOAT4X4* _Transform;
	Material _Material;

	MeshData* _Mesh;
	Shader* _Shader;
	Texture* _Texture;
};

