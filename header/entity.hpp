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
    glm::ivec2 atlasPosition;
public:
    glm::vec3 position, scale, rotation;
    Entity(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::ivec2 atlasPosition);
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

class Tree : public Entity {
public:
    Tree(glm::vec3 position) : Entity(position, glm::vec3(1,2,1)) {};
};

class Building : public Entity {
public:
    Building(glm::vec3(position), int type) : Entity(position, glm::vec3(3.6f,2.56f,1.0f)) {
        atlasPosition.x = type;
    };
};

#endif ENTITY_H