#include "Mesh.h"
#include "../Game.h"
#include "../DXSDK/WICTextureLoader.h"

Mesh::Mesh(Game* _game, std::vector<DirectX::XMFLOAT4>& vertices, std::vector<int>& indices, LPCWSTR _filepath)
{
    this->game=_game;
    this->filepath = _filepath;

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
}

void Mesh::Init()
{
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    HRESULT hr = game->device->CreateSamplerState(&sampDesc, this->samplerState.GetAddressOf()); //Create sampler state
    if (FAILED(hr))
    {
        std::cout << "OP" << std::endl;
    }

    D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };

    D3DCompileFromFile(shaderPath,
        nullptr /*macros*/,
        nullptr /*include*/,
        "VSMain",
        "vs_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        vertexBC.GetAddressOf(),
        nullptr);


    D3DCompileFromFile(shaderPath, Shader_Macros /*macros*/, nullptr /*include*/,
        "PSMain", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0, pixelBC.GetAddressOf(), nullptr);


    game->device->CreateVertexShader(
        this->vertexBC->GetBufferPointer(),
        this->vertexBC->GetBufferSize(),
        nullptr, this->vertexShader.GetAddressOf());

    game->device->CreatePixelShader(
        this->pixelBC->GetBufferPointer(),
        this->pixelBC->GetBufferSize(),
        nullptr, this->pixelShader.GetAddressOf());

    D3D11_INPUT_ELEMENT_DESC inputElements[] = {
        D3D11_INPUT_ELEMENT_DESC {
            "POSITION",
            0,
            DXGI_FORMAT_R32G32B32A32_FLOAT,
            0,
            0,
            D3D11_INPUT_PER_VERTEX_DATA,
            0},
        D3D11_INPUT_ELEMENT_DESC {
            "COLOR",
            0,
            DXGI_FORMAT_R32G32B32A32_FLOAT,
            0,
            D3D11_APPEND_ALIGNED_ELEMENT,
            D3D11_INPUT_PER_VERTEX_DATA,
            0}
    };

    game->device->CreateInputLayout(
        inputElements,
        2,
        vertexBC->GetBufferPointer(),
        vertexBC->GetBufferSize(),
        layout.GetAddressOf());



    hr = DirectX::CreateWICTextureFromFile(game->device, filepath, nullptr, myTexture.GetAddressOf());
    if (FAILED(hr))
    {
        int q = 0;
    }

    CD3D11_RASTERIZER_DESC rastDesc = {};
    rastDesc.CullMode = D3D11_CULL_NONE;
    rastDesc.FillMode = D3D11_FILL_SOLID;

    game->device->CreateRasterizerState(&rastDesc, rastState.GetAddressOf());

    game->context->RSSetState(rastState.Get());

    D3D11_VIEWPORT viewport = {};
    viewport.Width = static_cast<float>(800);
    viewport.Height = static_cast<float>(800);
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MinDepth = 0;
    viewport.MaxDepth = 1.0f;

    game->context->RSSetViewports(1, &viewport);
}


void Mesh::Draw()
{
	constexpr UINT offset[1]={0};
	constexpr UINT stride[1]={32};
    
    game->context->IASetInputLayout(layout.Get());
    game->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    game->context->PSSetShaderResources(0, 1, myTexture.GetAddressOf());
    game->context->PSSetSamplers(0,1,samplerState.GetAddressOf());
    //game->context->OMSetDepthStencilState(game->depthStencilState.Get(), 0);
    game->context->VSSetShader(vertexShader.Get(), nullptr, 0);
    game->context->PSSetShader(pixelShader.Get(), nullptr, 0);

    game->context->IASetVertexBuffers(0, 1, &pVertexBuffer,
        stride, offset);
    game->context->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    game->context->OMSetRenderTargets(1, &game->renderTargetView,game->depthStencilView);
  
    game->context->DrawIndexed(indicesSize, 0, 0);
    
    game->context->OMSetRenderTargets(0, nullptr, nullptr);
}

