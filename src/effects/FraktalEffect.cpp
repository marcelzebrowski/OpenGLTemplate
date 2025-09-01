#include "objects/Fraktal.hpp"
#include "effects/FraktalEffect.hpp"

FraktalEffect::FraktalEffect(Fraktal* fraktal, float startTime, float duration):Effect(startTime,duration), fraktal(fraktal){
    frakValue = 0.0f;
}

void FraktalEffect::onUpdate(float deltaTime, float localTime){
    frakValue += deltaTime;
    fraktal->update(frakValue);
}

void FraktalEffect::onRender(){
    fraktal->render(alpha);
}
