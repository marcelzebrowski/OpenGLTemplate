#ifndef EMPTYEFFECT_HPP
#define EMPTYFFECT_HPP
#include <memory>
#include "core/Effect.hpp"
#include "objects/Empty.hpp"


class EmptyEffect : public Effect{
private:
    std::unique_ptr<Empty> empty;
public:
    EmptyEffect(std::unique_ptr<Empty> empty, float startTime, float durationTime);

protected:
    void onUpdate(float deltaTime, float elapsed) override;
    void onRender() override;

};

#endif