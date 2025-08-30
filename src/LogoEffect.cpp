#include "LogoEffect.hpp"


LogoEffect::LogoEffect(PictureAnimator* pictureAnimator, 
    float startTime, float duration)
    : Effect(startTime, duration), pictureAnimator(pictureAnimator){
}


void LogoEffect::onUpdate(float deltaTime, float localTime){
    pictureAnimator->update(deltaTime, &projection);
}

void LogoEffect::onRender(){
    pictureAnimator->render();
}
