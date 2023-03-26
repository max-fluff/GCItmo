#include "MyDebugPlane.h"
#include "../../Game.h"
#include <memory>

MyDebugPlane::MyDebugPlane(std::shared_ptr<Game> game)
{
    this->game=game;
}

void MyDebugPlane::Initialize()
{
    plane=DirectX::GeometricPrimitive::CreateBox(game->context.Get(),DirectX::XMFLOAT3(10.0f,0.001f,10.0f),false,true);

}

void MyDebugPlane::Render()
{
    game->context->OMSetRenderTargets(1, game->rtv.GetAddressOf(), game->depthStencilView.Get());

    D3D11_DEPTH_STENCIL_DESC depthstencildesc;
    ZeroMemory(&depthstencildesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

    depthstencildesc.DepthEnable = true;
    depthstencildesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    depthstencildesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

    HRESULT hr = game->device->CreateDepthStencilState(&depthstencildesc, game->depthStencilState.GetAddressOf());

    

    plane->Draw(DirectX::XMMatrixIdentity(),game->camera->GetViewMatrix(),game->camera->GetProjectionMatrix(),DirectX::Colors::Aqua);
    game->context->OMSetRenderTargets(0, nullptr, nullptr);
}

void MyDebugPlane::Update(float deltaSec)
{
    
}


