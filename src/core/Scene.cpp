#include "core/Scene.hpp"

Scene::Scene():sceneTime(0.0f){
}

void Scene::update(float deltaTime){
    sceneTime += deltaTime;

    for(auto& effect : effects){
        effect->update(sceneTime, deltaTime);
    }
}

void Scene::render(){
    if(!isFinished()){
        for(auto& effect : effects){
            effect->render();
        }
    }
}

bool Scene::isFinished() const {
    for(const auto& effect : effects){
        if(!effect->isFinished()){
            return false;
        }
    }

    return true; // all effects are done
}


void Scene::addEffect(std::unique_ptr<Effect> effect){
    effects.push_back(std::move(effect));
}