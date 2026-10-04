#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
private:
    glm::vec3 position, front, up;
    float yaw,pitch;
public:
    Camera();

    void initialize();
    void setPosition(glm::vec3 pos);

    glm::mat4 getViewMatrix();
};

#endif