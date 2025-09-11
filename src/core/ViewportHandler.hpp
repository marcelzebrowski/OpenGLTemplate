#ifndef VIEWPORTHANDLER_HPP
#define VIEWPORTHANDLER_HPP

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "render/Shader.hpp"


struct GLFWwindow;

class ViewportHandler {
private:
    glm::mat4 projection;
    glm::mat4 orthogonalProjection;
    float fov;

public:
    ViewportHandler();
    void framebufferSizeCallBack(int width, int height);
    void registerWithWindow(GLFWwindow* window);
    glm::mat4* getProjection();
    glm::mat4* getOrthogonalProjectionPtr();
    glm::mat4& getOrthogonalProjectionAddress();
    void setFov(float fov);

    static void framebufferSizeCallbackStatic(GLFWwindow* window, int width, int height);
};

#endif