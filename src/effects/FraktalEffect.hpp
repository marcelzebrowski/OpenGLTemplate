#ifndef FRAKTALEFFECT_HPP
#define FRAKTALEFFECT_HPP
#include <memory>
#include "core/Effect.hpp"
#include "objects/Fraktal.hpp"
class FraktalEffect : public Effect{
private:
    float frakValue;
    std::unique_ptr<Fraktal> fraktal;
public:
    FraktalEffect(std::unique_ptr<Fraktal> fraktal, float start, float duration);
protected:
    void onUpdate(float deltaTime, float localTime) override;
    void onRender() override;

};
#endif