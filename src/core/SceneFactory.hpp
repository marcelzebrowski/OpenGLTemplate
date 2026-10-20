#ifndef SCENEFACTORY_HPP
#define SCENEFACTORY_HPP
#include <memory>
#include "core/Scene.hpp"
#include "core/Effect.hpp"

class SceneFactory{
public:

    static std::unique_ptr<Scene> createEmptyScene();
private:
    static std::unique_ptr<Effect> createEmptyEffect();

};
#endif