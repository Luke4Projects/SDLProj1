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

bool BoxCollider::isColliding(BoxCollider &collider2) {
    if(position.x + scale.x/2 > collider2.position.x - collider2.scale.x/2 && position.x - scale.x/2 < collider2.position.x + collider2.scale.x/2) {
        if(position.y + scale.y/2 > collider2.position.y - collider2.scale.y/2 && position.y - scale.y/2 < collider2.position.y + collider2.scale.y/2) {
            if(position.z + zBuffer > collider2.position.z - zBuffer && position.z - zBuffer < collider2.position.z + zBuffer) {
                return true;
            }
        }
    }
    return false;
}

void BoxCollider::renderDebugQuad(Renderer& renderer, SDL_GPURenderPass* renderPass, SDL_GPUCommandBuffer* cmdBuffer, TextureAtlas& textureAtlas) {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    model = glm::scale(model, glm::vec3(scale.x, scale.y, 1));
    renderer.performQuadRender(renderPass, cmdBuffer, model, textureAtlas.getTransform(0,0), glm::vec4(0.0f, 1.0f, 0.0f, 0.4f));
}

void BoxCollider::update(glm::vec3 position, glm::vec3 scale) {
    this->position = position;
    this->scale = scale;
}

void BoxCollider::preventCollision(BoxCollider &collider2, glm::vec3 &pos2, glm::vec3 &vel2) {
    if(!isColliding(collider2)) {
        return;
    }
    float diffX = position.x*position.x - pos2.x*pos2.x;
    float diffZ = position.z*position.z - pos2.z*pos2.z;
    if(abs(diffX) < abs(diffZ)) {
        //x axis collision
        if(diffX > 0) {
            pos2.x = position.x - collider2.scale.x;
        } else {
            pos2.x = position.x + scale.x;
        }
    } else {
        // z axis collision
        if(diffZ > 0) {
            pos2.z = position.z - zBuffer;
        } else {
            pos2.z = position.z + zBuffer;
        }
    }
}


FXFadeIn::FXFadeIn(ColorData &colorData) : colorData(colorData) {

}

Entity::Entity(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::ivec2 atlasPosition) : position(position), scale(scale), rotation(rotation), atlasPosition(atlasPosition) {
    colorData = {.color=glm::vec4(1)};
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
    renderer.performQuadRender(renderPass, cmdBuffer, model, textureAtlas.getTransform(atlasPosition.x, atlasPosition.y), colorData.color);
}
