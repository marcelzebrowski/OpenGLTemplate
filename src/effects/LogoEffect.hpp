#ifndef LOGOEFFECT_HPP
#define LOGOEFFECT_HPP
#include "render/PictureAnimator.hpp"
#include "core/Effect.hpp"

class LogoEffect : public Effect {
private:
    PictureAnimator* pictureAnimator;
public:
    LogoEffect(PictureAnimator* pictureAnimator, float startTime, float duration);

protected:
    void onUpdate(float deltaTime, float localTime) override;
    void onRender() override;

};

#endif