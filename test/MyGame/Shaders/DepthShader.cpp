#include "DepthShader.h"
#include "../../Game.h"

DepthShader::DepthShader(std::shared_ptr<Game> _game)
{
    this->game=_game;
}


void DepthShader::Initialize(LPCWSTR shaderPath)
{
    D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };
    
    HRESULT res = D3DCompileFromFile(shaderPath,
        nullptr /*macros*/,
        nullptr /*include*/,
        "DepthVertexShader",
        "vs_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        vertexBC.GetAddressOf(),
        errorVertexCode.GetAddressOf());

    res = D3DCompileFromFile(shaderPath, Shader_Macros /*macros*/, nullptr /*include*/,
        "DepthPixelShader", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0, pixelBC.GetAddressOf(), errorPixelCode.GetAddressOf());

    res = game->device->CreateVertexShader(
        this->vertexBC->GetBufferPointer(),
        this->vertexBC->GetBufferSize(),
        nullptr, this->vertexShader.GetAddressOf());

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
            0},
        D3D11_INPUT_ELEMENT_DESC {
            "NORMAL",
            0,
            DXGI_FORMAT_R32G32B32A32_FLOAT,
            0,
            D3D11_APPEND_ALIGNED_ELEMENT,
            D3D11_INPUT_PER_VERTEX_DATA,
            0}
    };

    res = game->device->CreateInputLayout(
        inputElements,
        3,
        vertexBC->GetBufferPointer(),
        vertexBC->GetBufferSize(),
        layout.GetAddressOf());
    
    
  

    CD3D11_RASTERIZER_DESC rastDesc = {};
    rastDesc.CullMode = D3D11_CULL_NONE;
    rastDesc.FillMode = D3D11_FILL_SOLID;

    res = game->device->CreateRasterizerState(&rastDesc, rastState.GetAddressOf());

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

void DepthShader::Render()
{
    
}
