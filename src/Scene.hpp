#ifndef SCENE_HPP
#define SCENE_HPP
#include <memory>
#include <vector>
#include "Effect.hpp"

class Scene {
protected:
    std::vector<std::unique_ptr<Effect>> effects;
    float duration;
    float sceneTime = 0.0f;

    

public:
    Scene(float duration);
    void addEffect(std::unique_ptr<Effect> effect);
    void update(float deltaTime);
    void render();
    bool isFinished() const;

};
#endif
