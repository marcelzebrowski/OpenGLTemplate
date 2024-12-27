#include "ViewportHandler.hpp"
#include <GLFW/glfw3.h>


ViewportHandler::ViewportHandler(Shader* shader):shader(shader),projection(glm::mat4(1.0f)){
}

void ViewportHandler::framebufferSizeCallBack(int width, int height){
    glViewport(0,0, width, height);
    float aspect = static_cast<float>(width)/static_cast<float>(height);
    projection = glm::ortho(-aspect, aspect, -1.0f,1.0f,-1.0f,1.0f);

    if(shader){
        shader->attach();
        shader->setMat4("projection",projection);
        shader->detach();
    }
}

void ViewportHandler::registerWithWindow(GLFWwindow* window){
    glfwSetWindowUserPointer(window,this);

    // set call back function
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win,int w, int h){
        auto* handler = static_cast<ViewportHandler*>(glfwGetWindowUserPointer(win));
        if(handler){
            handler->framebufferSizeCallBack(w,h);
        }
    });
}

glm::mat4 ViewportHandler::getProjection() const{
    return projection;
}