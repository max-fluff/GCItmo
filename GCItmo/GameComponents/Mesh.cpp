#include "Mesh.h"
#include "../Game.h"


Mesh::Mesh(Game* _game, std::vector<DirectX::XMFLOAT4>& vertices, std::vector<int>& indices,ModelObject* _sh)
{
    this->game=_game;

    D3D11_BUFFER_DESC vertexBufDesc;
    vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufDesc.CPUAccessFlags = 0;
    vertexBufDesc.MiscFlags = 0;
    vertexBufDesc.StructureByteStride = 0;
    vertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * vertices.size();

    D3D11_SUBRESOURCE_DATA vertexData;

    vertexData.pSysMem = static_cast<void*>(vertices.data());
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    game->device->CreateBuffer(&vertexBufDesc, &vertexData, &pVertexBuffer);

    D3D11_BUFFER_DESC indexBufDesc;
    indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufDesc.CPUAccessFlags = 0;
    indexBufDesc.MiscFlags = 0;
    indexBufDesc.StructureByteStride = 0;
    indexBufDesc.ByteWidth = sizeof(int) * indices.size();

    D3D11_SUBRESOURCE_DATA indexData;

    indexData.pSysMem = static_cast<void*>(indices.data());
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    game->device->CreateBuffer(&indexBufDesc, &indexData, &pIndexBuffer);

    indicesSize = indices.size();

    shader = _sh;
}

void Mesh::Initialize()
{
    
}


void Mesh::Render()
{
	constexpr UINT offset[1]={0};
	constexpr UINT stride[1]={32};
    
    game->context->IASetInputLayout(shader->layout.Get());
    game->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    game->context->PSSetShaderResources(0, 1, shader->myTexture.GetAddressOf());
    game->context->PSSetSamplers(0,1,shader->samplerState.GetAddressOf());
    //game->context->OMSetDepthStencilState(game->depthStencilState.Get(), 0);
    game->context->VSSetShader(shader->vertexShader.Get(), nullptr, 0);
    game->context->PSSetShader(shader->pixelShader.Get(), nullptr, 0);

    game->context->IASetVertexBuffers(0, 1, &pVertexBuffer,
        stride, offset);
    game->context->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    game->context->OMSetRenderTargets(1, &game->renderTargetView,game->depthStencilView);
  
    game->context->DrawIndexed(indicesSize, 0, 0);
    
    game->context->OMSetRenderTargets(0, nullptr, nullptr);
}

