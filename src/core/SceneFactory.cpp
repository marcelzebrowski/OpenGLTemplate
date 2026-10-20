#include <vector>
#include <memory>
#include "core/Scene.hpp"
#include "core/SceneFactory.hpp"

#include "objects/Empty.hpp"
#include "effects/EmptyEffect.hpp"

// Empty Scene
std::unique_ptr<Scene> SceneFactory::createEmptyScene(){
    auto scene = std::make_unique<Scene>();
    scene->addEffect(createEmptyEffect());
    return scene;
}


std::unique_ptr<Effect> SceneFactory::createEmptyEffect(){
    float startTime = 1.0f;
    float durationTime = 10.0f;
    auto empty = std::make_unique<Empty>();
    auto emptyEffect = std::make_unique<EmptyEffect>(std::move(empty),startTime, durationTime);
    return emptyEffect;
}
