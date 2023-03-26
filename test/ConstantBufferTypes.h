#pragma once

#include <directxmath.h>

#include "SimpleMath.h"


struct CB_VS_vertexshader
{ 
	DirectX::XMMATRIX worldViewProj;
	DirectX::XMMATRIX worldInverseT;
	DirectX::SimpleMath::Vector4 ambient;
	DirectX::SimpleMath::Vector4 diffuse;
	DirectX::SimpleMath::Vector4 specular;
	DirectX::SimpleMath::Vector4 direction;
	DirectX::SimpleMath::Vector4 eyePos;
};

struct CB_ModelLightning
{
	DirectX::XMMATRIX worldViewProj;
	DirectX::XMMATRIX worldInverseT;
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX lightViewProj;
	DirectX::XMMATRIX worldView;
	DirectX::SimpleMath::Vector4 light;
	DirectX::SimpleMath::Vector4 ambient;
	DirectX::SimpleMath::Vector4 specular;
	DirectX::SimpleMath::Vector4 direction;
	DirectX::SimpleMath::Vector4 eyePos;
	DirectX::SimpleMath::Matrix viewProj;
};

struct CB_DepthShader
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
};

struct CB_DepthPass
{
	DirectX::SimpleMath::Matrix world;
	DirectX::SimpleMath::Matrix worldViewProj;
	
};