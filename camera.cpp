#include "header/camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() {
    initialize();
}

void Camera::initialize() {
    position = glm::vec3(0.0f,0.1f,0.0f);
    up = glm::vec3(0.0f,1.0f,0.0f);
    yaw = -90.0f;
    pitch = 0.0f;
}

glm::mat4 Camera::getViewMatrix() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f,1.0f,0.0f)));  
    up = glm::normalize(glm::cross(right, front));
    return glm::lookAt(position, position + front, up);
}