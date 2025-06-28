

#ifndef PICTUREANIMATOR_HPP
#define PICTUREANIMATOR_HPP

#include "PictureFadeController.hpp"

enum class AnimationType {
    EaseOutBack,
    Swing
};

class PictureAnimator {
public:
    PictureAnimator(float originalWidth, float originalHeight, float durationSeconds,
        PictureFadeController* pictureFadeController, AnimationType animationType = AnimationType::EaseOutBack);

    void start();
    void render(float delta);

    void setTargetSize(float screenWidth, float screenHeight);

private:
    float originalWidth;
    float originalHeight;
    float scaledWidth;
    float scaledHeight;

    float durationSeconds;
    float elapsed;

    float maxHeight;
    float maxWidth;

    glm::mat4 model;
    glm::mat4 view;

    PictureFadeController* pictureFadeController;

    AnimationType animationType;


    float easeOutBack(float t);
    glm::vec2 swing(float t);
};

#endif
