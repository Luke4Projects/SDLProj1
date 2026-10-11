#ifndef TOWN_H
#define TOWN_H

#include "entity.hpp"

class TownPlayer : public Entity {
private:
    Animation animation;
    const float friction = 5.0f;
    const float moveSpeed = 5.0f;
public:
    glm::vec3 velocity;
    BoxCollider collider;
    TownPlayer();
    void update();
};

class Building : public Entity {
public:
    BoxCollider collider;
    Building(glm::vec3(position), int type);
};

class TownExit : public Building {
public:
    TownExit();
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
    std::vector<Building> buildings;
    std::vector<Entity> tileSegments;
    TownGameData();
};


#endif TOWN_H