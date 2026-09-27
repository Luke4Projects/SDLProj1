#include "header/game.hpp"
#include "header/render.hpp"

#include <glm/gtc/matrix_transform.hpp>

Entity::Entity(glm::vec3 position, glm::vec3 scale, glm::vec3 velocity, glm::vec3 rotation) : position(position), scale(scale), velocity(velocity), rotation(rotation) {

}

Entity::Entity(glm::vec3 position, glm::vec3 scale) : Entity(position, scale, glm::vec3(0), glm::vec3(0)) {
}

Entity::Entity() : Entity(glm::vec3(0), glm::vec3(1)) {
}

void Entity::renderAsQuad(Renderer& renderer, SDL_GPURenderPass* renderPass, SDL_GPUCommandBuffer* cmdBuffer, TextureAtlas& textureAtlas) {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    model = glm::scale(model, scale);
    renderer.bindTextureAtlas(renderPass, textureAtlas);
    renderer.performQuadRender(renderPass, cmdBuffer, model, textureAtlas.getTransform(static_cast<int>(atlasPosition.x), static_cast<int>(atlasPosition.y)));
}

void Entity::update() {

}

GameObjects::GameObjects() {
    player = Entity(glm::vec3(0,0,-1), glm::vec3(1));
}

Game::Game() : renderer(camera, gameObjects) {

}

bool Game::update() {
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            return false;
        }
    }

    renderer.updateRendering();

    return true;
}

void Game::quit() {
    renderer.cleanup();
}