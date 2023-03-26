#pragma once
#include "../Graphics/Shader.h"

#include <vector>

class Game;

class DepthShader: public Shader
{
public:
    DepthShader(std::shared_ptr<Game> _game);
    void Initialize(LPCWSTR shaderPath) override;
    void Render() override;

    LPCWSTR filepath;
    
};
