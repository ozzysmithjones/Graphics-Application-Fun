#include "Graphics.h"

#include "Structures.h"
#include "DDSTextureLoader.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Time.h"

#include "GameObject.h"
#include "Renderer.h"
#include "OBJLoader.h"

void Graphics::LoadDefaults()
{
    //create shader:
    _pDefaultShader = new Shader();
    _pDefaultShader->CompileShader(L"DX11 Framework.fx", _pd3dDevice);
    _pDefaultShader->Use(_pImmediateContext);

    //textures:
    _pDefaultTexture = new Texture();
    _pDefaultTexture->CompileTexture(L"Textures/Crate_COLOR.dds", _pd3dDevice);
    _pDefaultTexture->Use(_pImmediateContext);

    _pDefaultSampler = CreateSampler(D3D11_FILTER_MIN_MAG_MIP_LINEAR, true);
}

HRESULT Graphics::CreateBuffer(ID3D11Buffer** buffer, UINT bufferSize, UINT bindFlags)
{
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = bufferSize;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    return _pd3dDevice->CreateBuffer(&bd, nullptr, *&buffer);
}

void Graphics::BindConstantBuffer(ID3D11Buffer** buffer, int slot, bool isVSBuffer, bool isPSBuffer)
{
    if (isVSBuffer)
    {
        _pImmediateContext->VSSetConstantBuffers(slot, 1, *&buffer);
    }

    if (isPSBuffer)
    {
        _pImmediateContext->PSSetConstantBuffers(slot, 1, *&buffer);
    }
}

void Graphics::Cleanup()
{
    if (_pd3dDevice) _pd3dDevice->Release();
    if (_pImmediateContext) _pImmediateContext->ClearState();
    if (_pImmediateContext) _pImmediateContext->Release();

    if (_depthStencilView)_depthStencilView->Release();
    if (_depthStencilBuffer)_depthStencilBuffer->Release();

    if (_wireFrame)_wireFrame->Release();
    if (_filled)_filled->Release();

    if (_pConstantBuffer) _pConstantBuffer->Release();
    if (_pLightBuffer) _pLightBuffer->Release();

    if (_pRenderTargetView) _pRenderTargetView->Release();
    if (_pSwapChain) _pSwapChain->Release();

    DeleteEntities();

    delete _pDefaultShader;
    delete _pDefaultTexture;
}

void Graphics::DeleteEntities()
{
    for (auto p : _pRenderers)
    {
        delete p;
    }
    _pRenderers.clear();

    for (auto p : _pShaders)
    {
        delete p;
    }
    _pShaders.clear();

    for (auto p : _pTextures)
    {
        delete p;
    }
    _pTextures.clear();

    for (auto p : _pSamplers) {
        if (p) p->Release();
    }
    _pSamplers.clear();

    for (auto p : _pMeshes)
    {
        if (p->IndexBuffer) p->IndexBuffer->Release();
        if (p->VertexBuffer)p->VertexBuffer->Release();
        delete p;
    }
    _pMeshes.clear();
}

HRESULT Graphics::Init(HWND window,UINT windowWidth, UINT windowHeight)
{
    HRESULT hr = S_OK;
    UINT createDeviceFlags = 0;

#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif


    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };

    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };

    //create the swap-chain 
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = windowWidth;
    sd.BufferDesc.Height = windowHeight;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = window;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        _driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain(nullptr, _driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
            D3D11_SDK_VERSION, &sd, &_pSwapChain, &_pd3dDevice, &_featureLevel, &_pImmediateContext);
        if (SUCCEEDED(hr))
            break;
    }

    if (FAILED(hr))
        return hr;

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    if (FAILED(hr))
        return hr;

    hr = _pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pRenderTargetView);
    pBackBuffer->Release();

    if (FAILED(hr))
        return hr;

    //create the stentil buffer/texture.
    D3D11_TEXTURE2D_DESC depthStencilDesc;

    depthStencilDesc.Width = windowWidth;
    depthStencilDesc.Height = windowHeight;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    _pd3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &_depthStencilBuffer);
    _pd3dDevice->CreateDepthStencilView(_depthStencilBuffer, nullptr, &_depthStencilView);

    //Apply render target view:
    _pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView, _depthStencilView);

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)windowWidth;
    vp.Height = (FLOAT)windowHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    _pImmediateContext->RSSetViewports(1, &vp);

    // Set primitive topology
    _pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Create the constant buffers
    hr = CreateBuffer(&_pConstantBuffer, sizeof(ConstantBuffer));
    BindConstantBuffer(&_pConstantBuffer, 0, true, true);
    if (FAILED(hr))
        return hr;

    hr = CreateBuffer(&_pLightBuffer, sizeof(LightBuffer));
    BindConstantBuffer(&_pLightBuffer, 1, true, true);
    if (FAILED(hr))
        return hr;

    //create the wireframe render state
    D3D11_RASTERIZER_DESC wfdesc;
    ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
    wfdesc.FillMode = D3D11_FILL_WIREFRAME;
    wfdesc.CullMode = D3D11_CULL_NONE;
    hr = _pd3dDevice->CreateRasterizerState(&wfdesc, &_wireFrame);

    //create the filled render state
    D3D11_RASTERIZER_DESC ffdesc;
    ZeroMemory(&ffdesc, sizeof(D3D11_RASTERIZER_DESC));
    ffdesc.FillMode = D3D11_FILL_SOLID;
    ffdesc.CullMode = D3D11_CULL_NONE;
    hr = _pd3dDevice->CreateRasterizerState(&ffdesc, &_filled);

    _pImmediateContext->RSSetState(_filled);//use filled render state.

    LoadDefaults();
}

Shader* Graphics::CreateShader(WCHAR* file, BOOL bind)
{
    Shader* shader = new Shader();
    shader->CompileShader(file, _pd3dDevice);
    _pShaders.push_back(shader);

    if (bind) 
    {
        shader->Use(_pImmediateContext);
    }

    return shader;
}

Texture* Graphics::CreateTexture(WCHAR* file)
{
    Texture* texture = new Texture();
    texture->CompileTexture(file, _pd3dDevice);
    _pTextures.push_back(texture);

    return texture;
}

MeshData* Graphics::CreateMesh(char* file)
{
    MeshData* mesh = OBJLoader::Load(file, _pd3dDevice, false);
    _pMeshes.push_back(mesh);
    return mesh;
}

ID3D11SamplerState* Graphics::CreateSampler(D3D11_FILTER filter, BOOL bind)
{

    ID3D11SamplerState* sampler;

    // Create the sample state
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = filter; //D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    _pd3dDevice->CreateSamplerState(&sampDesc, &sampler);
    if (bind) 
    {
        _pImmediateContext->PSSetSamplers(0, 1, &sampler);
    }

    _pSamplers.push_back(sampler);
    return sampler;
}



void Graphics::BindTexture(ID3D11ShaderResourceView* texture, int slot)
{
    _pImmediateContext->PSSetShaderResources(slot, 1, &texture);
}

void Graphics::Update()
{
    Time::Update(_driverType);
    //Camera::Main->Update(Time::deltaTime);
}

void Graphics::Draw()
{
    // Clear the back buffer
    float ClearColor[4] = { 0.0f, 0.5f, 0.5f, 0.1f }; // red,green,blue,alpha
    _pImmediateContext->ClearRenderTargetView(_pRenderTargetView, ClearColor);

    //clear stencil buffer
    _pImmediateContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0F, 0);

    //world/view/projection matrix
    XMMATRIX world = XMLoadFloat4x4(&_pWorld);
    XMMATRIX view = XMLoadFloat4x4(&Camera::Main->GetView());
    XMMATRIX projection = XMLoadFloat4x4(&Camera::Main->GetProjection());

    //constant buffer:
    ConstantBuffer cb;
    cb.mWorld = XMMatrixTranspose(world);
    cb.mView = XMMatrixTranspose(view);
    cb.mProjection = XMMatrixTranspose(projection);
    cb.time = Time::totalTime;
    cb.EyePosW = Camera::Main->GetPosition();

    //light buffer:
    LightBuffer lb;
    lb.Lights[0] = { XMFLOAT4(1, 0.2, 1, 1), XMFLOAT4(0, 0, 0.5, 1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(100, 5, 0, 0) };
    lb.Lights[1] = { XMFLOAT4(0, 1, 0.2, 1), XMFLOAT4(0, 0, 0.5, 1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(-100, 5, 0, 0) };

    lb.spotLight = { XMFLOAT4(0.5, 0.5, 0, 1), XMFLOAT4(0.1, 0, 0.1, 1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(0, 5, 0, 0), XMFLOAT4(0,-1,0,0)};

    _pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
    _pImmediateContext->UpdateSubresource(_pLightBuffer, 0, nullptr, &lb, 0, 0);

    for (int i = 0; i < _pRenderers.size(); i++)
    {
        //update world position.
        XMMATRIX world = XMLoadFloat4x4(_pRenderers[i]->_Transform);
        cb.mWorld = XMMatrixTranspose(world);

        //get draw data from renderer.
        MeshData mesh = *_pRenderers[i]->_Mesh;
        Shader* shader = _pRenderers[i]->_Shader;
        Texture* texture = _pRenderers[i]->_Texture;
        Material& material = _pRenderers[i]->_Material;

        //use shader & texture from renderer.
        shader->Use(_pImmediateContext);
        texture->Use(_pImmediateContext);

        //use material:
        cb.DiffuseMtrl = material.diffuse;
        cb.AmbientMtrl = material.ambient;
        cb.specularMtrl = material.specular;

        //render mesh:
        _pImmediateContext->IASetVertexBuffers(0, 1, &mesh.VertexBuffer, &mesh.VBStride, &mesh.VBOffset);
        _pImmediateContext->IASetIndexBuffer(mesh.IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
        _pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
        _pImmediateContext->DrawIndexed(mesh.IndexCount, 0, 0);
    }


    _pSwapChain->Present(0, 0);
}

Graphics::~Graphics()
{
    Cleanup();
}


void Graphics::AddRenderer(Renderer* renderer)
{
    _pRenderers.push_back(renderer);
}

void Graphics::RemoveRenderer(Renderer* renderer, bool deleteAfter)
{
    for (int i = 0; i < _pRenderers.size(); i++) {
        if (_pRenderers[i] == renderer) {
            _pRenderers.erase(_pRenderers.begin() + i);
            break;
        }
    }

    if (deleteAfter) 
        delete renderer;
}



