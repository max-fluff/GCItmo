#include "MyCube.h"
#include "../../Game.h"
#include <memory>

MyCube::MyCube(std::shared_ptr<Game> _game,DirectX::SimpleMath::Vector3 _pos)
{
    this->game=_game;
    this->pos=_pos;
}

void MyCube::Initialize()
{
    cube=DirectX::GeometricPrimitive::CreateCube(game->context.Get(),0.5f,false);
    collider.Center=pos;
    collider.Extents.x=0.25f;
    collider.Extents.y=0.25f;
    collider.Extents.z=0.25f;
    worldMatrix=DirectX::XMMatrixTranslation(pos.x,pos.y,pos.z);
}

void MyCube::Render()
{
    game->context->OMSetRenderTargets(1, game->rtv.GetAddressOf(), game->depthStencilView.Get());

    D3D11_DEPTH_STENCIL_DESC depthstencildesc;
    ZeroMemory(&depthstencildesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

    depthstencildesc.DepthEnable = true;
    depthstencildesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    depthstencildesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

    HRESULT hr = game->device->CreateDepthStencilState(&depthstencildesc, game->depthStencilState.GetAddressOf());

    

    cube->Draw(worldMatrix*playerCoords,game->camera->GetViewMatrix(),game->camera->GetProjectionMatrix(),DirectX::Colors::Red);
    game->context->OMSetRenderTargets(0, nullptr, nullptr);
}

void MyCube::Update(float deltaSec)
{
    
}

