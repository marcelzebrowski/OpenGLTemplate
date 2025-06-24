#include "PictureFadeController.hpp"

PictureFadeController::PictureFadeController(Picture* picture):picture(picture){
    this->alpha = 0.0f;
    this->fadeSpeed = 0.02f;
}

PictureFadeController::~PictureFadeController(){
}

void PictureFadeController::render(float delta){

    if(alpha > 1.0f){
        alpha = 1.0f;
    }else {
        alpha += fadeSpeed * delta;
    }

    picture->render(alpha);
}