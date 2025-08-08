#include "Fraktal.hpp"
#include "FraktalEffect.hpp"

FraktalEffect::FraktalEffect(Fraktal* fraktal, float start, float duration):Effect(start,duration), fraktal(fraktal){
    frakValue = 0.0f;
}

void FraktalEffect::onUpdate(float deltaTime, float localTime){
    frakValue += deltaTime;
    fraktal->update(frakValue);
}

void FraktalEffect::onRender(){
    fraktal->render();
}