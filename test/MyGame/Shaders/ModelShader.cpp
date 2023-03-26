#include "ModelShader.h"


#include <corecrt_math_defines.h>

#include "WICTextureLoader.h"
#include "../../Game.h"
#include "../Models/Model3D.h"

ModelShader::ModelShader(std::shared_ptr<Game> _game,Model3D* _gc,LPCWSTR _filepath)
{
    this->game=_game;
    this->gc=_gc;
    this->filepath=_filepath;
}




void ModelShader::Initialize(LPCWSTR shaderPath)
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
        std::cout<<"OP"<<std::endl;
    }

    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory(&samplerDesc, sizeof(samplerDesc));
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

    // Create the texture sampler state.
    auto res2=game->device->CreateSamplerState(&samplerDesc, samplerDepthState.GetAddressOf());
    

    
    
    D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };

    HRESULT res = D3DCompileFromFile(shaderPath,
        nullptr /*macros*/,
        nullptr /*include*/,
        "VSMain",
        "vs_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        vertexBC.GetAddressOf(),
        errorVertexCode.GetAddressOf());
    
    if (FAILED(res)) {
        // If the shader failed to compile it should have written something to the error message.
        if (errorVertexCode) {
            char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

            std::cout << compileErrors << std::endl;
        }
        // If there was  nothing in the error message then it simply could not find the shader file itself.
        else
        {
            MessageBox(game->window->GetHWND(), L"./Shaders/Racket.hlsl", L"Missing Shader File", MB_OK);
        }

    }

    res = D3DCompileFromFile(shaderPath, Shader_Macros /*macros*/, nullptr /*include*/,
        "PSMain", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0, pixelBC.GetAddressOf(), errorPixelCode.GetAddressOf());


    res = game->device->CreateVertexShader(
        this->vertexBC->GetBufferPointer(),
        this->vertexBC->GetBufferSize(),
        nullptr, this->vertexShader.GetAddressOf());

    res = game->device->CreatePixelShader(
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
    


    hr = DirectX::CreateWICTextureFromFile(game->device.Get(),filepath, nullptr, myTexture.GetAddressOf());
    if (FAILED(hr))
    {
      int q=0;
    }

    CD3D11_RASTERIZER_DESC rastDesc = {};
    rastDesc.CullMode = D3D11_CULL_NONE;
    rastDesc.FillMode = D3D11_FILL_SOLID;

    res = game->device->CreateRasterizerState(&rastDesc, rastState.GetAddressOf());

    game->context->RSSetState(rastState.Get());
}

void ModelShader::Render()
{
    
}

