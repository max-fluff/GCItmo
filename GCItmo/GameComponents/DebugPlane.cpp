#include "DebugPlane.h"
#include "../Game.h"
#include "../DXSDK/GeometricPrimitive.h"
#include <memory>

DebugPlane::DebugPlane(Game* game)
{
    this->game=game;
}

void DebugPlane::Init()
{
    plane=DirectX::GeometricPrimitive::CreateBox(game->context,DirectX::XMFLOAT3(15.0f,0.001f,15.0f),false,true);

}

void DebugPlane::Draw()
{
    game->context->OMSetRenderTargets(1, &game->renderTargetView, game->depthStencilView);

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

    HRESULT hr = game->device->CreateDepthStencilState(&depthStencilDesc, &game->depthStencilState);

    plane->Draw(DirectX::XMMatrixIdentity(),game->camera->GetViewMatrix(),game->camera->GetProjectionMatrix(),DirectX::Colors::Firebrick);
    game->context->OMSetRenderTargets(0, nullptr, nullptr);
}


