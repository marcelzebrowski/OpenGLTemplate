#include "Effect.hpp"

Effect::Effect(float start, float duration):startTime(start), duration(duration){

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