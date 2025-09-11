#ifndef PICTUREANIMATORMANAGER_HPP
#define PICTUREANIMATORMANAGER_HPP

#include <memory>
#include <vector>
#include "render/PictureAnimator.hpp"

class PictureAnimatorManager {
public:
    PictureAnimatorManager(std::vector<std::unique_ptr<PictureAnimator>> animators, float displayTimePerImage);
    void update(float delta, glm::mat4* projection);
    void render();

private:
    std::vector<std::unique_ptr<PictureAnimator>> animators;
    int currentIndex;
    float timeAccumulator;
    float displayTime;

    enum class AnimationPhase {
        IN,
        HOLD,
        OUT
    };
    AnimationPhase phase;

};

#endif