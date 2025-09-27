#include <vector>
#include <memory>
#include "core/Scene.hpp"
#include "core/SceneFactory.hpp"
#include "effects/FadeEffect.hpp"
#include "effects/LogoEffect.hpp"
#include "objects/Fraktal.hpp"
#include "effects/FraktalEffect.hpp"
#include "render/Texture.hpp"
#include "render/PictureAnimatorManager.hpp"
#include "render/PictureAnimator.hpp"
#include "render/PictureFadeController.hpp"

#include "utils/RessourceManager.hpp"


std::unique_ptr<Scene> SceneFactory::createFraktalScene(int height, int width,glm::mat4& projectionOrthogonal){
    auto scene = std::make_unique<Scene>();
    scene->addEffect(createFraktalSceneFadeEffect(height, width));
    scene->addEffect(createFraktalSceneLogoEffect(projectionOrthogonal));
    return scene;
}

std::unique_ptr<Effect> SceneFactory::createFraktalSceneFadeEffect(int height, int width){
    auto shader = RessourceManager::getShader(ShaderID::Fraktal);
    auto fraktal = std::make_unique<Fraktal>(shader, height, width);
	auto fraktalEffect = std::make_unique<FraktalEffect>(std::move(fraktal),0.0f, 50.0f);
    return std::make_unique<FadeEffect>(std::move(fraktalEffect), 0.0f,10.0f, true);
}

std::unique_ptr<Effect> SceneFactory::createFraktalSceneLogoEffect(glm::mat4& projectionOrthogonal){

    std::vector<std::unique_ptr<PictureAnimator>> animators;

    std::unique_ptr<Picture> overflow; // todo hier noch den shader und die textur übergeben!
    auto pictureFadeController = std::make_unique<PictureFadeController>(std::move(overflow));
    auto overflowAnimator = std::make_unique<PictureAnimator>(1273.0f, 241.0f,10.0f, std::move(pictureFadeController), AnimationType::Swing);

    animators.push_back(std::move(overflowAnimator));

    auto pictureAnimatorManager = std::make_unique<PictureAnimatorManager>(std::move(animators), 10.0f);
    auto effect = std::make_unique<LogoEffect>(std::move(pictureAnimatorManager), 10.0f, 5.0f);
    effect->setProjectionMatrix(projectionOrthogonal);

    return effect;
}
