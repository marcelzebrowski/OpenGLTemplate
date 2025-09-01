#include "render/Camera.hpp"

Camera::Camera():position(0.0f,0.0f,3.0f), target(0.0f,0.0f,0.0f), up(0.0f,1.0f,0.0f){
}

Camera::Camera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up):position(position),target(target),up(up){
}

glm::mat4 Camera::getViewMatrix() const{
    return glm::lookAt(position, position + target,up);
}

void Camera::move(const glm::vec3& offset){
    position += offset;
    target += offset;
}


void Camera::rotate(float yaw, float pitch){
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw))* cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw))* cos(glm::radians(pitch));
    direction = glm::normalize(direction);
    target = direction;
}

glm::vec3 Camera::getPosition() const{
    return position;
}

glm::vec3 Camera::getTarget() const{
    return target;
}

glm::vec3 Camera::getUp() const{
    return up;
}

void Camera::setPosition(const glm::vec3& position){
    this->position = position;
}

void Camera::setTarget(const glm::vec3& target){
    this->target = target;
}

void Camera::setUp(const glm::vec3& up){
    this->up = up;
}