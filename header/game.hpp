#ifndef GAME_H
#define GAME_H

#include <glm/glm.hpp>
#include "render.hpp"

struct RenderData {
    glm::mat4 model;
    glm::vec4 texture;
};

class Entity {
private:
    glm::vec3 position, velocity, scale, rotation;
    glm::vec2 atlasPosition;
public:
    Entity(glm::vec3 position, glm::vec3 scale, glm::vec3 velocity, glm::vec3 rotation);
    Entity(glm::vec3 position, glm::vec3 scale);
    Entity();
    void update();
    void renderAsQuad(Renderer& renderer, SDL_GPURenderPass* renderPass, SDL_GPUCommandBuffer* cmdBuffer, TextureAtlas& textureAtlas);
};

struct GameObjects {
    Entity player;
    GameObjects();
};

class Game {
private:
    SDL_Event event;
    Camera camera;
    Renderer renderer;
    GameObjects gameObjects;
public:
    Game();
    bool update();
    void quit();
};

#endif GAME_H