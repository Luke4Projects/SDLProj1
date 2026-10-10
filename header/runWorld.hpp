#ifndef RUNWORLD_H
#define RUNWORLD_H

#include "entity.hpp"

class RunningPlayer : public Entity {
private:
    Animation animation;
    glm::vec3 strafeVelocity;
    const float strafeFriction = 5.0f;
    const float strafeSpeed = 5.0f;
    float runSpeed = 5.0f;
public:
    RunningPlayer();
    void update();
};

class Tree : public Entity {
public:
    Tree(glm::vec3 position) : Entity(position, glm::vec3(1,2,1)) {};
};

struct RunGameData {
    std::vector<Tree> trees;
    RunningPlayer player;
    RunGameData();
    void update();
};

#endif RUNWORLD_H