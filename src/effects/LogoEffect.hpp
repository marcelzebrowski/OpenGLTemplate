#ifndef LOGOEFFECT_HPP
#define LOGOEFFECT_HPP
#include <memory>
#include "render/PictureAnimatorManager.hpp"
#include "core/Effect.hpp"

class LogoEffect : public Effect {
private:
    std::unique_ptr<PictureAnimatorManager> pictureAnimatorManager;
public:
    LogoEffect(std::unique_ptr<PictureAnimatorManager> pictureAnimatorManager, float startTime, float duration);

protected:
    void onUpdate(float deltaTime, float elapsed) override;
    void onRender() override;

};

#endif