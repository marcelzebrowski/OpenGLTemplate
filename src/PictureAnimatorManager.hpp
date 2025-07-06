#ifndef PICTUREANIMATORMANAGER_HPP
#define PICTUREANIMATORMANAGER_HPP

#include <vector>
#include "PictureAnimator.hpp"

class PictureAnimatorManager {
public:
    PictureAnimatorManager(std::vector<PictureAnimator> animators, float displayTimePerImage);
    void update(float delta);
    void render(float delta);

private:
    std::vector<PictureAnimator> animators;
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