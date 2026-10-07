#ifndef TOWN_H
#define TOWN_H

#include "entity.hpp"

class TownPlayer : public Entity {
private:
    Animation animation;
    glm::vec3 velocity;
    const float friction = 5.0f;
    const float moveSpeed = 5.0f;
public:
    TownPlayer();
    void update();
};

class Building : public Entity {
public:
    Building(glm::vec3(position), int type) : Entity(position, glm::vec3(3.6f,2.56f,1.0f)) {
        atlasPosition.x = type;
    };
};

class TownExit : public Building {
public:
    TownExit() : Building(glm::vec3(0,1.54f,0), 0) {};
    bool shouldExit(glm::vec3 playerPosition);
};

enum class DeliverySize {
    SMALL,
    MEDIUM,
    LARGE
};

struct Delivery {
    int destinationTownId;
    int destinationCharacterId;
    int rewardMoney;
    DeliverySize size;
};

class TownCharacter : public Entity {
public:
    const int id;
    std::vector<Delivery> deliveries;
};

struct TownGameData {
    TownPlayer player;
    TownExit exit;
    std::vector<Tree> trees;
    std::vector<Building> buildings;
    std::vector<Entity> tileSegments;
    TownGameData();
};


#endif TOWN_H