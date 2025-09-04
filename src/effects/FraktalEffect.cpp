#include "objects/Fraktal.hpp"
#include "effects/FraktalEffect.hpp"

FraktalEffect::FraktalEffect(std::unique_ptr<Fraktal> fraktal, float startTime, float duration):Effect(startTime,duration), fraktal(std::move(fraktal)){
    frakValue = 0.0f;
}

void FraktalEffect::onUpdate(float deltaTime, float localTime){
    frakValue += deltaTime;
    fraktal->update(frakValue);
}

void FraktalEffect::onRender(){
    fraktal->render(alpha);
}
