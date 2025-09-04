#include <memory>
#include "core/Scene.hpp"
#include "core/SceneFactory.hpp"
#include "effects/FadeEffect.hpp"
#include "effects/LogoEffect.hpp"
#include "objects/Fraktal.hpp"
#include "effects/FraktalEffect.hpp"

std::unique_ptr<Scene> SceneFactory::createFraktalScene(int height, int width){
    auto scene = std::make_unique<Scene>();

    scene->addEffect(createFraktalSceneFadeEffect(height, width));
    //scene->addEffect(createFraktalSceneLogoEffect());

    return scene;
}

std::unique_ptr<Effect> SceneFactory::createFraktalSceneFadeEffect(int height, int width){
    auto shader = std::make_unique<Shader>("shader/fraktal/fraktal_vs.glsl","shader/fraktal/fraktal_fs.glsl");
    auto fraktal = std::make_unique<Fraktal>(std::move(shader), height, width);
	auto fraktalEffect = std::make_unique<FraktalEffect>(std::move(fraktal),0.0f, 50.0f);
    return std::make_unique<FadeEffect>(std::move(fraktalEffect), 0.0f,10.0f, true);
}

std::unique_ptr<Effect> SceneFactory::createFraktalSceneLogoEffect(){
    //return std::make_unique<LogoEffect>();
    return nullptr;
}
