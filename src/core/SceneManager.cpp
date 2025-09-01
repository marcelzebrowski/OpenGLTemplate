#include "core/SceneManager.hpp"

SceneManager::SceneManager(){
}

void SceneManager::addScene(std::unique_ptr<Scene> scene){
    scenes.push_back(std::move(scene));
}

void SceneManager::update(float deltaTime){
    if(currentSceneIndex >= scenes.size()){
        return;
    }

    globalTime += deltaTime;

    auto& scene = scenes[currentSceneIndex];
    scene->update(deltaTime);
    if(scene->isFinished()){
        currentSceneIndex++;
    }
}

void SceneManager::render(){
    if(currentSceneIndex >= scenes.size()){
        return;
    }

    auto& scene = scenes[currentSceneIndex];
    // todo add matrix to scene
    scene->render();
}

bool SceneManager::allFinished() const{
    return currentSceneIndex >= scenes.size();
}