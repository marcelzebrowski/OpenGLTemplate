#ifndef FRAKTALEFFECT_HPP
#define FRAKTALEFFECT_HPP
#include "Effect.hpp"
#include "Fraktal.hpp"
class FraktalEffect : public Effect{
private:
    float frakValue;
    Fraktal* fraktal;
public:
    FraktalEffect(Fraktal* fraktal, float start, float duration);
protected:
    void onUpdate(float deltaTime, float localTime) override;
    void onRender() override;

};
#endif