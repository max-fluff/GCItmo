#include "Mesh.h"
#include "../Game.h"


Mesh::Mesh(std::shared_ptr<Game> _game, std::vector<DirectX::XMFLOAT4>& vertices, std::vector<int>& indices,ModelShader* _sh, DepthShader* _ds)
{
    this->game=_game;
    vertexBuffer=std::make_shared<Buffer>(_game);
    vertexBuffer->Initialize(vertices);
    indexBuffer=std::make_shared<Buffer>(_game);
    indexBuffer->Initialize(indices,D3D11_USAGE_DEFAULT,D3D11_BIND_INDEX_BUFFER);
    shader=_sh;
    depthShader=_ds;
    
}

Mesh::Mesh(const Mesh& mesh)
{
    this->indexBuffer=mesh.indexBuffer;
    this->vertexBuffer=mesh.vertexBuffer;
    this->transformMatrix=mesh.transformMatrix;
    this->shader=mesh.shader;
}

void Mesh::Initialize()
{
    
}


void Mesh::Render()
{
    UINT offset[1]={0};
    UINT stride[1]={48};
    
    game->context->IASetInputLayout(shader->layout.Get());
    game->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    game->context->PSSetShaderResources(0, 1, shader->myTexture.GetAddressOf());
    game->context->PSSetSamplers(0,1,shader->samplerState.GetAddressOf());
    game->context->VSSetShader(shader->vertexShader.Get(), nullptr, 0);
    game->context->PSSetShader(shader->pixelShader.Get(), nullptr, 0);
        
    game->context->IASetVertexBuffers(0, 1, vertexBuffer->buffer.GetAddressOf(),
        stride, offset);
    game->context->IASetIndexBuffer(indexBuffer->buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    game->context->OMSetRenderTargets(1, game->rtv.GetAddressOf(),game->depthStencilView.Get());
  
    game->context->DrawIndexed(indexBuffer->size, 0, 0);
    
    game->context->OMSetRenderTargets(0, nullptr, nullptr);
}

void Mesh::RenderToTexture()
{
    UINT offset[1]={0};
    UINT stride[1]={48};

    D3D11_VIEWPORT viewport = {};
    viewport.Width = static_cast<float>(800);
    viewport.Height = static_cast<float>(800);
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MinDepth = 0;
    viewport.MaxDepth = 1.0f;

    game->context->RSSetViewports(1, &viewport);

    game->context->IASetInputLayout(shader->layout.Get());
    game->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    game->context->PSSetShaderResources(0, 1, shader->myTexture.GetAddressOf());
    game->context->PSSetShaderResources(1, 1, &game->shadowResourceView);
    game->context->VSSetShader(shader->vertexShader.Get(), nullptr, 0);
    game->context->PSSetShader(shader->pixelShader.Get(), nullptr, 0);
    game->context->PSSetSamplers(0,1,shader->samplerState.GetAddressOf());
    game->context->PSSetSamplers(1,1,shader->samplerDepthState.GetAddressOf());
    
    game->context->IASetVertexBuffers(0, 1, vertexBuffer->buffer.GetAddressOf(),
        stride, offset);
    game->context->IASetIndexBuffer(indexBuffer->buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    game->context->DrawIndexed(indexBuffer->size, 0, 0);

    game->context->PSSetShader(0, nullptr, 0);
    game->context->VSSetShader(0, nullptr, 0);

}

void Mesh::DepthPass()
{
    UINT offset[1]={0};
    UINT stride[1]={48};

    D3D11_VIEWPORT viewport = {};
    viewport.Width = static_cast<float>(game->SHADOWMAP_WIDTH);
    viewport.Height = static_cast<float>(game->SHADOWMAP_HEIGHT);
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MinDepth = 0;
    viewport.MaxDepth = 1.0f;

    game->context->RSSetViewports(1, &viewport);
    
    game->context->IASetInputLayout(depthShader->layout.Get());
    game->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    game->context->VSSetShader(depthShader->vertexShader.Get(), nullptr, 0);
    
    game->context->IASetVertexBuffers(0, 1, vertexBuffer->buffer.GetAddressOf(),
        stride, offset);
    game->context->IASetIndexBuffer(indexBuffer->buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    game->context->DrawIndexed(indexBuffer->size,0,0);
}
