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

Entity::Entity(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::ivec2 atlasPosition) : position(position), scale(scale), rotation(rotation), atlasPosition(atlasPosition) {
}

Entity::Entity(glm::vec3 position, glm::vec3 scale) : Entity(position, scale, glm::vec3(0), glm::ivec2(0,0)) {
}

Entity::Entity() : Entity(glm::vec3(0), glm::vec3(1)) {
}

void Entity::renderAsQuad(Renderer& renderer, SDL_GPURenderPass* renderPass, SDL_GPUCommandBuffer* cmdBuffer, TextureAtlas& textureAtlas) {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    model = glm::rotate(model, rotation.x, glm::vec3(1,0,0));
    model = glm::rotate(model, rotation.y, glm::vec3(0,1,0));
    model = glm::rotate(model, rotation.z, glm::vec3(0,0,1));
    model = glm::scale(model, scale);
    //renderer.bindTextureAtlas(renderPass, textureAtlas);
    renderer.performQuadRender(renderPass, cmdBuffer, model, textureAtlas.getTransform(atlasPosition.x, atlasPosition.y));
}

