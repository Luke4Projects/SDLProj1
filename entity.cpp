#include <glm/gtc/matrix_transform.hpp>
#include "header/entity.hpp"
#include "header/game.hpp"

glm::ivec2 Animation::update() {
    currentTick += GameUtil::deltaTime;
    if(currentTick < frameDelay) {
        return currentFrame;
    }
    currentTick = 0;
    if(cycleX) {
        currentFrame.x = (currentFrame.x + 1) % numberOfFrames;
    } else {
        currentFrame.y = (currentFrame.y + 1) % numberOfFrames;
    }
    return currentFrame;
}

Entity::Entity(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) : position(position), scale(scale), rotation(rotation) {
    atlasPosition = glm::ivec2(0, 0);
}

Entity::Entity(glm::vec3 position, glm::vec3 scale) : Entity(position, scale, glm::vec3(0)) {
}

Entity::Entity() : Entity(glm::vec3(0), glm::vec3(1)) {
}

void Entity::renderAsQuad(Renderer& renderer, SDL_GPURenderPass* renderPass, SDL_GPUCommandBuffer* cmdBuffer, TextureAtlas& textureAtlas) {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    model = glm::scale(model, scale);
    renderer.bindTextureAtlas(renderPass, textureAtlas);
    renderer.performQuadRender(renderPass, cmdBuffer, model, textureAtlas.getTransform(atlasPosition.x, atlasPosition.y));
}

Player::Player() : Entity(glm::vec3(0, 0.5f, -5), glm::vec3(1, 1, 1)) {
    velocity = glm::vec3(0);
    animation = Animation{
        .numberOfFrames = 4
    };
}

void Player::update() {

    const bool *keyStates = SDL_GetKeyboardState(NULL);
    if(keyStates[SDL_SCANCODE_W]) {
        velocity.z-=moveSpeed * (float)GameUtil::deltaTime;
        animation.currentFrame.y = 3;
    }
    if(keyStates[SDL_SCANCODE_S]) {
        velocity.z+=moveSpeed * (float)GameUtil::deltaTime;
        animation.currentFrame.y = 0;
    }
    if(keyStates[SDL_SCANCODE_D]) {
        velocity.x+=moveSpeed * (float)GameUtil::deltaTime;
        animation.currentFrame.y = 2;
    }
    if(keyStates[SDL_SCANCODE_A]) {
        velocity.x-=moveSpeed * (float)GameUtil::deltaTime;
        animation.currentFrame.y = 1;
    }


    position += velocity * (float)GameUtil::deltaTime;
    velocity *= SDL_exp(-friction * GameUtil::deltaTime);

    if(glm::dot(velocity, velocity) > 0.01f) {
        atlasPosition = animation.update();
    } else {
        atlasPosition = glm::ivec2(0, animation.currentFrame.y);
    }
}