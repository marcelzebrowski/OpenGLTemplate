#include "LogoEffect.hpp"


LogoEffect::LogoEffect(PictureFadeController* pictureFadeController, 
    float startTime, float duration)
    : Effect(startTime, duration), pictureFadeController(pictureFadeController){
}


void LogoEffect::onUpdate(float deltaTime, float localTime){
    pictureFadeController->update(true, deltaTime, localTime, &projection, &view, &model);
}

void LogoEffect::onRender(){
    pictureFadeController->render();
}
