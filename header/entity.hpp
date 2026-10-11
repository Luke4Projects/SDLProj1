#ifndef ENTITY_H
#define ENTITY_H

#include "render.hpp"

struct Animation {
    glm::ivec2 currentFrame = glm::ivec2(0,0);
    float frameDelay = 0.1f;
    int numberOfFrames = 1;
    bool cycleX = true;
    float currentTick = 0;
    glm::ivec2 update();
};

struct BoxCollider {
    glm::vec3 position;
    glm::vec2 scale;
    float zBuffer = 0.1f;
    bool isColliding(BoxCollider &collider2);
    void renderDebugQuad(Renderer& renderer, SDL_GPURenderPass* renderPass, SDL_GPUCommandBuffer* cmdBuffer, TextureAtlas& textureAtlas);
    void update(glm::vec3 position, glm::vec3 scale);
    void preventCollision(BoxCollider &collider2, glm::vec3 &pos2, glm::vec3 &vel2);
};

struct FXFadeIn {
    ColorData &colorData;
    FXFadeIn(ColorData &colorData);
};

class Entity {
protected:
    glm::ivec2 atlasPosition;
public:
    ColorData colorData;
    glm::vec3 position, scale, rotation;
    Entity(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::ivec2 atlasPosition);
    Entity(glm::vec3 position, glm::vec3 scale);
    Entity();
    void renderAsQuad(Renderer& renderer, SDL_GPURenderPass* renderPass, SDL_GPUCommandBuffer* cmdBuffer, TextureAtlas& textureAtlas);
};

#endif ENTITY_H