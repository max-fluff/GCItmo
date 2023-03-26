#include "SphereShader.h"


#include <corecrt_math_defines.h>

#include "WICTextureLoader.h"
#include "../../Game.h"
#include "../Components/Player.h"
#include "../Graphics/Mesh.h"

SphereShader::SphereShader(std::shared_ptr<Game> _game,Player* _gc,LPCWSTR _filepath)
{
    this->game=_game;
    this->gc=_gc;
    this->filepath=_filepath;
}

void SphereShader::Load(std::string& filepath)
{
    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(filepath,aiProcess_Triangulate|aiProcess_SortByPType|aiProcess_ConvertToLeftHanded);
    
    
    this->ProcessNode(pScene->mRootNode, pScene);
}

Mesh* SphereShader::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
   

    for (UINT i=0; i<mesh->mNumVertices;i++)
    {
        DirectX::XMFLOAT4 vertex;
        DirectX::XMFLOAT4 color;
        DirectX::XMFLOAT4 normals=DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f);
        vertex.x=mesh->mVertices[i].x;
        vertex.y=mesh->mVertices[i].y;
        vertex.z=mesh->mVertices[i].z;
        vertex.w=1.0f;
        

        if (mesh->mTextureCoords[0])
        {
            color.x= (float)mesh->mTextureCoords[0][i].x;
            color.y= (float)mesh->mTextureCoords[0][i].y;
        }

        normals.x= mesh->mNormals[i].x;
        normals.y=mesh->mNormals[i].y;
        normals.z=mesh->mNormals[i].z;
        

        verts.push_back(vertex);
        verts.push_back(DirectX::XMFLOAT4(color.x,color.y,1.0f,1));
        verts.push_back(normals);
        
    }

    for (UINT i =0; i<mesh->mNumFaces;i++)
    {
        aiFace face=mesh->mFaces[i];

        for (UINT j=0;j<face.mNumIndices;j++)
            index.push_back(face.mIndices[j]);
    }
        
    return new Mesh(game,verts,index,nullptr,nullptr);

}

void SphereShader::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (UINT i=0;i<node->mNumMeshes;i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(this->ProcessMesh(mesh,scene));
    }

    for (UINT i=0;i< node->mNumChildren;i++)
    {
        this->ProcessNode(node->mChildren[i],scene);
    }
}



void SphereShader::GenerateSphere(float r, int segments, int slices)
{
    
    int count   = 0;
    int numSlices=slices;
    int numSegments=segments;
    int step=0;
   
    //
    
    
    for (float verticalAngle=-M_PI/2.0f+M_PI/(2.0f*(numSlices*1.0f));verticalAngle<M_PI/2.0f;verticalAngle+=M_PI/(2.0f*(numSlices*1.0f)))
    {
        step=0;
        for (float horizontalAngle=0;horizontalAngle<M_PI*2;horizontalAngle+=M_PI*2.0f/(numSegments*1.0f))
        {
            DirectX::XMFLOAT4 vertex(1.0f,0.0f,0.0f,1.0f);
            vertex.x=cos(horizontalAngle)*r * cos( verticalAngle);
            vertex.y = sin(horizontalAngle)*r * cos( verticalAngle);
            vertex.z = r*sin( verticalAngle);
            verts.push_back(vertex);
            
            

            DirectX::XMFLOAT3 tmp(vertex.x,vertex.y,vertex.z);
            DirectX::XMVECTOR vvertex=DirectX::XMLoadFloat3(&tmp);
            vvertex=DirectX::XMVector3Normalize(vvertex);
            DirectX::XMStoreFloat4(&vertex,vvertex);
            vertex.x=atan2(vertex.x,vertex.z) / (2*M_PI) +0.5f;
            vertex.y=vertex.y*0.5f+0.5f;
            
            verts.push_back(vertex);
            
            step++;
        }
    }

    verts.push_back(DirectX::XMFLOAT4(0,0,-r,1.0f));
    DirectX::XMFLOAT4 tmp(0,0,-1,1.0f);
    tmp.x=atan2(0,-1)/(2*M_PI) +0.5f;
    tmp.y=0;
    
    
    verts.push_back(tmp);
    
    for (int startIndex=0;startIndex<step-1;startIndex++)
    {
        index.push_back(startIndex);
        index.push_back(verts.size()/2-1);
        index.push_back(startIndex+1);
        
    }
    index.push_back(step-1);
    index.push_back(verts.size()/2-1);
    index.push_back(0);

    
    for (int startIndex=0;startIndex<step*(numSlices-1)*2;startIndex+=step)
    {
        for (int i=startIndex;i<startIndex+step-1;i++)
        {
            
            index.push_back(i);
            index.push_back(i+1);
            index.push_back(i+step);

            index.push_back(i+1);
            index.push_back(i+1+step);
            index.push_back(i+step);
        }

        index.push_back(startIndex+step-1);
        index.push_back(startIndex+step);
        index.push_back(startIndex+step*2-1);
            
        index.push_back(startIndex);
        index.push_back(startIndex+step);
        index.push_back(startIndex+step-1);
        
    }

    verts.push_back(DirectX::XMFLOAT4(0,0,r,1.0f));

    tmp.x=atan2(0,1)/(2*M_PI) +0.5f;
    tmp.y=0;
    verts.push_back(tmp);

    for (int startIndex=step*(numSlices-1)*2;startIndex<step*(numSlices-1)*2+step-1;startIndex++)
    {
        index.push_back(startIndex);
        index.push_back(startIndex+1);
        index.push_back(verts.size()/2-1);
        
        
    }
    index.push_back(step*(numSlices-1)*2+step-1);
    index.push_back(step*(numSlices-1)*2);
    
   
    
    
    
    index.push_back(verts.size()/2-1);
}



void SphereShader::Initialize(LPCWSTR shaderPath)
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

    hr = game->device->CreateSamplerState(&sampDesc, samplerDepthState.GetAddressOf()); //Create sampler state
    if (FAILED(hr))
    {
        std::cout<<"OP"<<std::endl;
    }
    
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
            0
            }
    };

    res = game->device->CreateInputLayout(
        inputElements,
        3,
        vertexBC->GetBufferPointer(),
        vertexBC->GetBufferSize(),
        layout.GetAddressOf());



    //GenerateSphere(gc->r,4,2);
    std::string g="MyGame/Models/sphere.fbx";
    Load(g);
    

    
    

    vertexBuffer=std::make_shared<Buffer>(game);
    vertexBuffer->Initialize(verts);
    
    indexBuffer=std::make_shared<Buffer>(game);
    indexBuffer->Initialize(index,D3D11_USAGE_DEFAULT,D3D11_BIND_INDEX_BUFFER);

    constantBuffer=std::make_shared<Buffer>(game);
    constantBuffer->Initialize(sizeof(CB_ModelLightning));


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

    D3D11_VIEWPORT viewport = {};
    viewport.Width = static_cast<float>(800);
    viewport.Height = static_cast<float>(800);
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MinDepth = 0;
    viewport.MaxDepth = 1.0f;

    

    game->context->RSSetViewports(1, &viewport);
}

void SphereShader::Render()
{
    
    //sphere->Draw( DirectX::XMMatrixIdentity(),game->camera->GetViewMatrix(),game->camera->GetProjectionMatrix());
    
//!!!!!!!!!!! REDO

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    auto helper = constantBuffer;
    auto help2 = helper->buffer.Get();
    
    HRESULT res= game->context->Map(help2, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    CopyMemory(mappedResource.pData, &gc->data, sizeof(CB_ModelLightning));
    game->context->Unmap(constantBuffer->buffer.Get(), 0);
    game->context->VSSetConstantBuffers(0, 1, constantBuffer->buffer.GetAddressOf());
    game->context->PSSetConstantBuffers(0, 1, constantBuffer->buffer.GetAddressOf());
    
    game->context->IASetInputLayout(layout.Get());
    game->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    game->context->IASetIndexBuffer(indexBuffer->buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    game->context->IASetVertexBuffers(0, 1, vertexBuffer->buffer.GetAddressOf(),
        strides, offsets);
    game->context->PSSetSamplers(0,1,this->samplerState.GetAddressOf());
    //game->context->OMSetDepthStencilState(game->depthStencilState.Get(), 0);
    game->context->VSSetShader(vertexShader.Get(), nullptr, 0);
    game->context->PSSetShader(pixelShader.Get(), nullptr, 0);
    game->context->PSSetShaderResources(0, 1, this->myTexture.GetAddressOf());
    game->context->OMSetRenderTargets(1, game->rtv.GetAddressOf(), game->depthStencilView.Get());
    
    game->context->DrawIndexed(index.size(), 0, 0);
    
    game->context->OMSetRenderTargets(0, nullptr, nullptr);
}

