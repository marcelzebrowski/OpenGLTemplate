#include "PictureFadeController.hpp"

PictureFadeController::PictureFadeController(Picture* picture):picture(picture){
    this->alpha = 0.0f;
    this->fadeSpeed = 0.2f;
}

PictureFadeController::~PictureFadeController(){
}

void PictureFadeController::render(float delta, float elapsed, glm::mat4 view, glm::mat4 model){

    if(alpha > 1.0f){
        alpha = 1.0f;
    }else {
        alpha += fadeSpeed * delta;
    }

    picture->render(alpha, elapsed, view, model);
}