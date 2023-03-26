#include "Buffer.h"
#include "../Game.h"

Buffer::Buffer(std::shared_ptr<Game> _game)
{
        this->game=_game;
}

void Buffer::Initialize(std::vector<DirectX::XMFLOAT4> points,D3D11_USAGE usage,
  UINT bindFlags,UINT cpuAccess,
  UINT miscFlags,UINT stride,UINT memPitch, UINT memSlicePitch)
{

    
    D3D11_BUFFER_DESC indexBufDesc = {};
    indexBufDesc.Usage = usage;
    indexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    indexBufDesc.CPUAccessFlags = cpuAccess;
    indexBufDesc.MiscFlags = miscFlags;
    indexBufDesc.StructureByteStride = stride;
    indexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * points.size();
        
    D3D11_SUBRESOURCE_DATA indexData = {};
        
    indexData.pSysMem = static_cast<void*>(points.data());
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    size=sizeof(DirectX::XMFLOAT4) * points.size();
    //this->stride=stride;
    //this->offset=offset;
    HRESULT res = game->device->CreateBuffer(&indexBufDesc, &indexData, buffer.GetAddressOf());
}


void Buffer::Initialize(std::vector<int> points,D3D11_USAGE usage,
   UINT bindFlags,UINT cpuAccess,
   UINT miscFlags,UINT stride,UINT memPitch, UINT memSlicePitch)
{
    D3D11_BUFFER_DESC indexBufDesc = {};
    indexBufDesc.Usage = usage;
    indexBufDesc.BindFlags = bindFlags;
    indexBufDesc.CPUAccessFlags = cpuAccess;
    indexBufDesc.MiscFlags = miscFlags;
    indexBufDesc.StructureByteStride = stride;
    indexBufDesc.ByteWidth = sizeof(int) * points.size();
        
    D3D11_SUBRESOURCE_DATA indexData = {};
        
    indexData.pSysMem = static_cast<void*>(points.data());
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    size= points.size();
   
    HRESULT res = game->device->CreateBuffer(&indexBufDesc, &indexData, buffer.GetAddressOf());
}

void  Buffer::Initialize(size_t structSize,D3D11_USAGE usage,
    UINT bindFlags,UINT cpuAccess,
    UINT miscFlags,UINT stride,UINT memPitch, UINT memSlicePitch)
{
    D3D11_BUFFER_DESC constantBufDesc = {};
    constantBufDesc.Usage = usage;
    constantBufDesc.BindFlags = bindFlags;
    constantBufDesc.CPUAccessFlags = cpuAccess;
    constantBufDesc.MiscFlags = 0;

    constantBufDesc.ByteWidth = structSize + (16 - structSize) % 16;
    constantBufDesc.StructureByteStride = 0;

    size=structSize + (16 - structSize) % 16;
    //this->stride=stride;
    //this->offset=offset;
    HRESULT res=game->device->CreateBuffer(&constantBufDesc, 0, buffer.GetAddressOf());
}
