#include "core/Effect.hpp"

Effect::Effect(float startTime, float duration):startTime(startTime), duration(duration){

}

void Effect::update(float sceneTime, float deltaTime){

    if(finished){
        return;
    }

    if(sceneTime < startTime){
        return;
    }

    if(!started){
        started = true;
    }

    elapsed = sceneTime - startTime;

    // check scene is finished
    if(duration > 0.0f && elapsed >= duration){
        finished = true;
        return;
    }

    onUpdate(deltaTime, elapsed);
}

void Effect::render(){

    if(!started || finished){
        return;
    }

    onRender();
}

bool Effect::isFinished() const{
    return finished;
}

float Effect::getAlpha() const{
    return alpha;
}

void Effect::setAlpha(float a){
    alpha = a;
}

void Effect::setMatrices(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model){
    this->projection = projection;
    this->view = view;
    this->model = model;
}