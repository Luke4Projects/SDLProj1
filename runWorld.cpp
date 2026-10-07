#include "header/runWorld.hpp"
#include "header/util.hpp"

RunningPlayer::RunningPlayer() {
    position = glm::vec3(0, 0.5f, 0);
    strafeVelocity = glm::vec3(0, 0, 0);
}

void RunningPlayer::update() {
    const bool *keyStates = SDL_GetKeyboardState(NULL);
    if(keyStates[SDL_SCANCODE_W]) {
        strafeVelocity.z-=strafeSpeed * (float)GameUtil::deltaTime;
        animation.currentFrame.y = 3;
    }
    if(keyStates[SDL_SCANCODE_S]) {
        strafeVelocity.z+=strafeSpeed * (float)GameUtil::deltaTime;
        animation.currentFrame.y = 0;
    }
    if(keyStates[SDL_SCANCODE_D]) {
        strafeVelocity.x+=strafeSpeed * (float)GameUtil::deltaTime;
        animation.currentFrame.y = 2;
    }
    if(keyStates[SDL_SCANCODE_A]) {
        strafeVelocity.x-=strafeSpeed * (float)GameUtil::deltaTime;
        animation.currentFrame.y = 1;
    }

    position += strafeVelocity * (float)GameUtil::deltaTime;
    strafeVelocity *= SDL_exp(-strafeFriction * GameUtil::deltaTime);
    position.z -= runSpeed * (float)GameUtil::deltaTime;

}