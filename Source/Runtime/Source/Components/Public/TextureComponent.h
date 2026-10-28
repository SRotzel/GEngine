#pragma once

#include "Component.h"
#include "Texture.h"
using gRenderer::Texture;

#include <memory>
#include <string>

// Standard gEngine Component to attach textures to an entity.
// Params: 
// std::string texturePath. default "". specifies file path to your texture image file
// unsigned int id. default = 0. Unique identifier to your component. Tip: use gGenerateID() for safe IDs.
class TextureComponent : public Component {
public:
    TextureComponent(std::string texturePath = "", unsigned int id = 0) 
        : Component(id), path(texturePath) {
        texture = std::make_unique<Texture>(path);
    }
    
    inline Texture* getTexture() const { return texture.get(); }

private:
    std::string path;
    std::unique_ptr<Texture> texture;
};