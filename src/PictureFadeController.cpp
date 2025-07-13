#include "PictureFadeController.hpp"

PictureFadeController::PictureFadeController(Picture* picture):picture(picture){
    this->alpha = 0.0f;
    this->fadeSpeed = 0.2f;
}

PictureFadeController::~PictureFadeController(){
}

void PictureFadeController::update(bool fadeIn, float delta, float elapsed, glm::mat4* projection, glm::mat4* view, glm::mat4* model){
    this->elapsed = elapsed; // todo brauchen wir das?
    this->projection = projection;
    this->view = view;
    this->model = model;

    if(fadeIn){    
        if(alpha > 1.0f){
            alpha = 1.0f;
        }else {
            alpha += fadeSpeed * delta;
        }
    }else{
        if(alpha < 0.0f){
            alpha = 0.0f;
        }else {
            alpha -= fadeSpeed * delta;
        }
    }

    picture->update(alpha, elapsed, projection, view, model);
}

void PictureFadeController::render(){
    picture->render();
}