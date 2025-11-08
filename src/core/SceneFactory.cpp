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

// Fraktal Scene

std::unique_ptr<Scene> SceneFactory::createFraktalScene(int height, int width,glm::mat4& projectionOrthogonal){
    auto scene = std::make_unique<Scene>();
    scene->addEffect(createFraktalSceneFadeEffect(height, width));
    scene->addEffect(createFraktalSceneLogoEffect(projectionOrthogonal));
    return scene;
}

std::unique_ptr<Effect> SceneFactory::createFraktalSceneFadeEffect(int height, int width){
    float startTime = 0.0f;
    float fadeTime = 10.0f;
    float durationTime = 50.0f; 
    auto& shader = RessourceManager::getShader(ShaderID::Fraktal);
    auto fraktal = std::make_unique<Fraktal>(shader, height, width);
	auto fraktalEffect = std::make_unique<FraktalEffect>(std::move(fraktal), startTime, durationTime);
    return std::make_unique<FadeEffect>(std::move(fraktalEffect), startTime, fadeTime, true);
}

std::unique_ptr<Effect> SceneFactory::createFraktalSceneLogoEffect(glm::mat4& projectionOrthogonal){
    float pictureWidth = 1273.0f;
    float pictureHeight = 241.0f;
    float animationDuration = 10.0f;
    float displayTimePerImage = 10.0f;
    float pictureStartTime = 10.0f;
    float pictureDuration = 5.0f;

    auto& shaderPicture = RessourceManager::getShader(ShaderID::Picture);
    auto& textureOverflowLogo = RessourceManager::getTexture(TexutureID::OverflowLogo);

    std::unique_ptr<Picture> overflow = std::make_unique<Picture>(shaderPicture,textureOverflowLogo);
    auto pictureFadeController = std::make_unique<PictureFadeController>(std::move(overflow));
    auto overflowAnimator = std::make_unique<PictureAnimator>(pictureWidth, pictureHeight,animationDuration, std::move(pictureFadeController), AnimationType::Swing);

    std::vector<std::unique_ptr<PictureAnimator>> animators;
    animators.push_back(std::move(overflowAnimator));

    auto pictureAnimatorManager = std::make_unique<PictureAnimatorManager>(std::move(animators), displayTimePerImage);
    auto effect = std::make_unique<LogoEffect>(std::move(pictureAnimatorManager), pictureStartTime, pictureDuration);
    effect->setProjectionMatrix(projectionOrthogonal);

    return effect;
}
