#include "header/util.hpp"
#include "header/town.hpp"

TownGameData::TownGameData() {

    int iterations = 0;
    glm::ivec2 position = glm::ivec2(0,0);
    glm::ivec2 atlasPos = glm::ivec2(2,1);
    float rotation = glm::radians(90.0f);
    while(iterations < 4) {
        int amountUp = SDL_rand(3) + 3;
        for(int i = 0; i < amountUp; i++) {
            // create road going up (z)
            position.y--;
            Entity tile(glm::vec3(position.x, 0.01f, position.y), glm::vec3(1), glm::vec3(rotation, 0, 0), atlasPos);
            tileSegments.push_back(tile);
        }
        // create house
        Building building(glm::vec3(position.x, 1.44f, position.y - 0.3f), SDL_rand(4));
        buildings.push_back(building);
        // change direction
        int xDir = SDL_rand(2)*2 - 1;
        SDL_Log("%d",xDir);
        int amountSide = SDL_rand(4) + 2;
        for(int i = 0; i < amountSide; i++) {
            // create road going side ways
            position.x += xDir;
            Entity tile(glm::vec3(position.x, 0.01f, position.y), glm::vec3(1), glm::vec3(rotation, 0, 0), atlasPos);
            tileSegments.push_back(tile);
        }
        iterations++;
    }

}

TownPlayer::TownPlayer() : Entity(glm::vec3(0, 0.5f, -5), glm::vec3(1, 1, 1)) {
    velocity = glm::vec3(0);
    animation = Animation{
        .numberOfFrames = 4
    };
}

void TownPlayer::update() {

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

bool TownExit::shouldExit(glm::vec3 playerPosition) {
    float distanceSqr = glm::dot(playerPosition-position, playerPosition-position);
    float exitDistance = 2;
    if(distanceSqr <= exitDistance*exitDistance) {
        return true;
    }
    return false;
}