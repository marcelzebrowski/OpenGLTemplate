#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "core/Scene.hpp"

class SceneManager{
private:
    std::vector<std::unique_ptr<Scene>> scenes;
    int currentSceneIndex = 0;
    float globalTime = 0.0f;
public:
    SceneManager();

    void addScene(std::unique_ptr<Scene> scene);
    void update(float deltaTime);
    void render();
    bool allFinished() const;


};
#endif


