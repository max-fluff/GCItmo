#pragma once
#include <d3d11.h>

#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

class TextureShaderClass
{
private:
    struct ConstantBufferType
    {
        Matrix world;
        Matrix view;
        Matrix projection;
        DirectX::SimpleMath::Vector4 index;
    };

public:
    TextureShaderClass();
    TextureShaderClass(const TextureShaderClass&);
    ~TextureShaderClass();

    bool Initialize(ID3D11Device*, HWND);
    void Shutdown();
    bool Render(ID3D11DeviceContext*, int, Matrix, Matrix, Matrix, ID3D11ShaderResourceView*,int);
    int index;
private:
    bool InitializeShader(ID3D11Device*, HWND, LPCWSTR, LPCWSTR);
    void ShutdownShader();
    void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

    bool SetShaderParameters(ID3D11DeviceContext*, Matrix, Matrix, Matrix, ID3D11ShaderResourceView*);
    void RenderShader(ID3D11DeviceContext*, int);

private:
    ID3D11VertexShader* m_vertexShader;
    ID3D11PixelShader* m_pixelShader;
    ID3D11InputLayout* m_layout;
    ID3D11Buffer* m_constantBuffer;
    ID3D11SamplerState* m_sampleState;
    
};
