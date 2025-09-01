#include <iostream>
#include <algorithm>
#include "core/FadeEffect.hpp"

FadeEffect::FadeEffect(std::unique_ptr<Effect> innerEffect, float start, float fadeTime, bool fadeIn)
    :   Effect(start, -1.0f), // no automatic end 
        innerEffect(std::move(innerEffect)), 
        fadeIn(fadeIn),
        fadeTime(fadeTime){
}

void FadeEffect::onUpdate(float deltaTime, float localTime){

    innerEffect->update(startTime + elapsed, deltaTime);
}

void FadeEffect::onRender(){

    const float t = fadeTime > 0.0f ? std::clamp(elapsed / fadeTime, 0.0f, 1.0f) : 1.0f;
    const float myAlpha = fadeIn ? t : (1.0f - t);

    innerEffect->setAlpha(myAlpha);
    innerEffect->render();

}

bool FadeEffect::isFinished() const{
    return innerEffect->isFinished();
}