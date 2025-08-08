#include "Scene.hpp"

Scene::Scene(float duration):duration(duration){

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
    return sceneTime >= duration;
}


void Scene::addEffect(std::unique_ptr<Effect> effect){
    effects.push_back(std::move(effect));
}