#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Camera {
public:
    Camera();
    Camera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up);

    glm::mat4 getViewMatrix() const;

    void move(const glm::vec3& offset);

    void rotate(float yaw, float pitch);

    // get and set
    glm::vec3 getPosition() const;
    glm::vec3 getTarget() const;
    glm::vec3 getUp() const;

    void setPosition(const glm::vec3& position);
    void setTarget(const glm::vec3& target);
    void setUp(const glm::vec3& up);

    

private:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    glm::vec3 front;
};

#endif