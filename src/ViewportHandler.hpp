#ifndef VIEWPORTHANDLER_HPP
#define VIEWPORTHANDLER_HPP


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.hpp"


struct GLFWwindow;

class ViewportHandler {
private:
    Shader* shader;
    glm::mat4 projection;

public:
    ViewportHandler(Shader* shader);
    void framebufferSizeCallBack(int width, int height);
    void registerWithWindow(GLFWwindow* window);
    glm::mat4 getProjection() const;
};

#endif