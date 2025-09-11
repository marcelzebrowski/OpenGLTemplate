#ifndef SCENEFACTORY_HPP
#define SCENEFACTORY_HPP
#include <memory>
#include "core/Scene.hpp"
#include "core/Effect.hpp"
#include "render/Shader.hpp"


class SceneFactory{
public:
    static std::unique_ptr<Scene> createFraktalScene(int height, int width, glm::mat4& projectionOrthogonal);
private:
    static std::unique_ptr<Effect> createFraktalSceneFadeEffect(int height, int width);
    static std::unique_ptr<Effect> createFraktalSceneLogoEffect(glm::mat4& projectionOrthogonal); 
};
#endif