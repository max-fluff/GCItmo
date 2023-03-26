#pragma once
#include <d3d11.h>

#include "SimpleMath.h"


class DebugWindow
{
private:
    struct VertexType
    {
        DirectX::SimpleMath::Vector3 position;
        DirectX::SimpleMath::Vector2 texture;
    };

public:
    DebugWindow();
    DebugWindow(const DebugWindow&);
    ~DebugWindow();

    bool Initialize(ID3D11Device*, int, int, int, int);
    void Shutdown();
    bool Render(ID3D11DeviceContext*, int, int);

    int GetIndexCount();

private:
    bool InitializeBuffers(ID3D11Device*);
    void ShutdownBuffers();
    bool UpdateBuffers(ID3D11DeviceContext*, int, int);
    void RenderBuffers(ID3D11DeviceContext*);

private:
    ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
    int m_vertexCount, m_indexCount;
    int m_screenWidth, m_screenHeight;
    int m_bitmapWidth, m_bitmapHeight;
    int m_previousPosX, m_previousPosY;
    
};
