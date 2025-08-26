#ifndef LOGOEFFECT_HPP
#define LOGOEFFECT_HPP
#include "PictureFadeController.hpp"
#include "Effect.hpp"

class LogoEffect : public Effect {
private:
    PictureFadeController* pictureFadeController;
public:
    LogoEffect(PictureFadeController* pictureFadeController, float startTime, float duration);

protected:
    void onUpdate(float deltaTime, float localTime) override;
    void onRender() override;

};

#endif