#include "objects/Empty.hpp"
#include "effects/EmptyEffect.hpp"

EmptyEffect::EmptyEffect(std::unique_ptr<Empty> empty, float startTime, float durationTime)
:Effect(startTime, durationTime), empty(std::move(empty)){

}

void EmptyEffect::onUpdate(float deltaTime, float localTime){
    empty->update(deltaTime);
}

void EmptyEffect::onRender(){
    empty->render();
}