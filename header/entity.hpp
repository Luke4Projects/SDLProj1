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

class Entity {
protected:
    glm::vec3 position, scale, rotation;
    glm::ivec2 atlasPosition;
public:
    Entity(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation);
    Entity(glm::vec3 position, glm::vec3 scale);
    Entity();
    void renderAsQuad(Renderer& renderer, SDL_GPURenderPass* renderPass, SDL_GPUCommandBuffer* cmdBuffer, TextureAtlas& textureAtlas);
};

class Player : public Entity {
private:
    Animation animation;
    glm::vec3 velocity;
    const float friction = 5.0f;
    const float moveSpeed = 5.0f;
public:
    Player();
    void update();
};

#endif ENTITY_H