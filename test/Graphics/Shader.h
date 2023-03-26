#pragma once

#include <windows.h>
#include <wrl.h>
#include <iostream>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Buffer.h"
#include "../ConstantBufferTypes.h"


class Game;




class Shader
{
public:
	virtual void Initialize(LPCWSTR shaderPath)=0;
	virtual void Render()=0;

	
	std::shared_ptr<Buffer> constantBuffer;
	std::shared_ptr<Buffer> indexBuffer;
	std::shared_ptr<Buffer> vertexBuffer;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> layout;
	Microsoft::WRL::ComPtr<ID3DBlob> vertexBC; 
	Microsoft::WRL::ComPtr<ID3DBlob> pixelBC;
	Microsoft::WRL::ComPtr<ID3DBlob> geomBC;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> geomShader;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rastState;
	
protected:

	

	

	Microsoft::WRL::ComPtr<ID3DBlob> errorPixelCode = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorVertexCode = nullptr;
	std::shared_ptr<Game> game;

	CB_VS_vertexshader constBufferData;

};


