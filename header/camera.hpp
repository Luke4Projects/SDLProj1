#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
private:
    glm::vec3 front, up;
    float yaw,pitch;
public:
    glm::vec3 position;
    Camera();

    void initializeTownView();
    void initializeRunView();

    glm::mat4 getViewMatrix();
};

#endif