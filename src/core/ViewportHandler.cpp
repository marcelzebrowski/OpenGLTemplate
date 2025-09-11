
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "core/ViewportHandler.hpp"


ViewportHandler::ViewportHandler():projection(glm::mat4(1.0f)),fov(45.0f){
}

void ViewportHandler::framebufferSizeCallBack(int width, int height){
    glViewport(0,0, width, height);
    float aspect = static_cast<float>(width)/static_cast<float>(height);
    projection = glm::perspective(glm::radians(fov),aspect, 0.1f, 100.0f);
    orthogonalProjection = glm::ortho(0.0f, static_cast<float>(width),0.0f, static_cast<float>(height));  
}

void ViewportHandler::registerWithWindow(GLFWwindow* window){
    glfwSetWindowUserPointer(window,this);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallbackStatic);
}

void ViewportHandler::framebufferSizeCallbackStatic(GLFWwindow* window, int width, int height){
    auto* handler = static_cast<ViewportHandler*>(glfwGetWindowUserPointer(window));
    if(handler){
        handler->framebufferSizeCallBack(width,height);
    }
}

glm::mat4* ViewportHandler::getProjection(){
    return &projection;
}

glm::mat4* ViewportHandler::getOrthogonalProjectionPtr(){
    return &orthogonalProjection;
}

glm::mat4& ViewportHandler::getOrthogonalProjectionAddress(){
    return orthogonalProjection;
}

void ViewportHandler::setFov(float fov){
    this->fov = fov;
}