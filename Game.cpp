#include "Game.h"


HRESULT Game::InitEntities()
{
    scene = new GameObject();

    //spaceship.
    XMFLOAT4X4 trans;
    spaceship = new GameObject();

    XMStoreFloat4x4(&trans, XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixTranslation(0.0f, -5.0f, -5.0f));
    spaceship->GetTransform()->SetLocalTransform(trans);

    //XMStoreFloat4x4(spaceship->GetTransform()->GetWorldTransform(), XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixTranslation(0.0f, 0.0f, -10.0f));

    
    Renderer* renderer = new Renderer();
    renderer->_Transform = spaceship->GetTransform()->GetWorldTransform();

    graphics->AddRenderer(renderer);

    renderer->_Mesh = graphics->CreateMesh("Models/Car.obj");
    renderer->_Shader = graphics->GetDefaultShader();
    renderer->_Texture = graphics->CreateTexture(L"Textures/Boring.dds");

    renderer->_Material.diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    renderer->_Material.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    renderer->_Material.specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    
    //prop
    prop = new GameObject();

    XMFLOAT4X4 propTrans;
    XMStoreFloat4x4(&propTrans, XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixTranslation(0.0f, -10.0f, -5.0f));
    prop->GetTransform()->SetLocalTransform(propTrans);

    Renderer* propRenderer = new Renderer();
    propRenderer->_Transform = prop->GetTransform()->GetWorldTransform();

    graphics->AddRenderer(propRenderer);

    propRenderer->_Mesh = graphics->CreateMesh("Models/City.obj");
    propRenderer->_Shader = graphics->GetDefaultShader();
    propRenderer->_Texture = graphics->CreateTexture(L"Textures/Boring.dds");

    propRenderer->_Material.diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
    propRenderer->_Material.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    propRenderer->_Material.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

    return S_OK;
}

void Game::CleanUp()
{
    delete scene;
    delete spaceship;
    delete prop;
}

HRESULT Game::Init(HINSTANCE hInstance, int nCmdShow)
{
    window = new Window();
    if (FAILED(window->InitWindow(hInstance, nCmdShow)))
    {
        delete window;
        return -1;
    }

    graphics = new Graphics();
    if (FAILED(graphics->Init(window->GetWindow(),window->_WindowWidth, window->_WindowHeight)))
    {
        delete graphics;
        delete window;
        return -1;
    }

    cameraManager = new CameraManager();
    cameraManager->Init(window->_WindowWidth, window->_WindowHeight);
    InitEntities();

    cameraManager->AddCamera(new Camera(spaceship, XMFLOAT3(0, 0, -20),XMFLOAT3(0,0,-1),XMFLOAT3(0, 1, 0), window->_WindowWidth, window->_WindowHeight, 0.01f, 150.0f));
    return S_OK;
}

void Game::Start()
{
}

void Game::Update()
{
    graphics->Update();
    cameraManager->Update(Time::deltaTime);
    spaceship->Update(Time::deltaTime);
    prop->Update(Time::deltaTime);

    
    /*
    Transform* transform = spaceship->GetTransform();
    
    if (GetAsyncKeyState(VK_UP) < 0)
    {
        transform->Translate(XMFLOAT3(0, 0, 30.0f * Time::deltaTime), true);
    }

    if (GetAsyncKeyState(VK_DOWN) < 0)
    {
        transform->Translate(XMFLOAT3(0, 0, -30.0f * Time::deltaTime), true);
    }

    if (GetAsyncKeyState(VK_LEFT) < 0)
    {
        transform->Rotate(XMFLOAT3(0, -10.f * Time::deltaTime, 0));
    }

    if (GetAsyncKeyState(VK_RIGHT) < 0)
    {
        transform->Rotate(XMFLOAT3(0, 10.0f * Time::deltaTime, 0));
    }
    */
    
}

void Game::Draw()
{
    graphics->Draw();
}

Game::~Game()
{
    delete graphics;
    delete window;
    delete cameraManager;
    CleanUp();
}


