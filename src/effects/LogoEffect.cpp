#include "effects/LogoEffect.hpp"


LogoEffect::LogoEffect(std::unique_ptr<PictureAnimatorManager> pictureAnimatorManager, 
    float startTime, float duration)
    : Effect(startTime, duration), pictureAnimatorManager(std::move(pictureAnimatorManager)){
}


void LogoEffect::onUpdate(float deltaTime, float elapsed){
    pictureAnimatorManager->update(deltaTime, &projection);
}

void LogoEffect::onRender(){
    pictureAnimatorManager->render();
}
