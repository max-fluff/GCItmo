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
	DirectX::SimpleMath::Vector4 light;
	DirectX::SimpleMath::Vector4 ambient;
	DirectX::SimpleMath::Vector4 specular;
	DirectX::SimpleMath::Vector4 direction;
	DirectX::SimpleMath::Vector4 eyePos;
};
