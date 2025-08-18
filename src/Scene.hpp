#ifndef SCENE_HPP
#define SCENE_HPP
#include <memory>
#include <vector>
#include "Effect.hpp"

class Scene {
protected:
    std::vector<std::unique_ptr<Effect>> effects;
    float sceneTime;
public:
    Scene();
    void addEffect(std::unique_ptr<Effect> effect);
    void update(float deltaTime);
    void render();
    bool isFinished() const;

};
#endif
