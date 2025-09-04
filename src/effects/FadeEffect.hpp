#ifndef FADEEFFECT_HPP
#define FADEEFFECT_HPP
#include <memory>
#include "core/Effect.hpp"


class FadeEffect : public Effect{
private:
    std::unique_ptr<Effect> innerEffect;
    bool fadeIn;
    float fadeTime;


public:
    FadeEffect(std::unique_ptr<Effect> innerEffect, float startTime, float fadeTime, bool fadeIn);

protected:
    void onUpdate(float deltaTime, float localTime) override;
    void onRender() override;

public:
    bool isFinished() const override;
};

#endif