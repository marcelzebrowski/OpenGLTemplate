

#ifndef PICTUREANIMATOR_HPP
#define PICTUREANIMATOR_HPP

#include "PictureFadeController.hpp"

enum class AnimationType {
    EaseOutBack,
    Swing
};

enum class AnimationDirection {
    IN,OUT
};

class PictureAnimator {
public:
    PictureAnimator(float originalWidth, float originalHeight, float durationSeconds,
        PictureFadeController* pictureFadeController, AnimationType animationType = AnimationType::EaseOutBack);

    void start();
    void startExit();
    void render();
    void update(float delta, glm::mat4* projection);

    void setTargetSize(float screenWidth, float screenHeight);

    float getDurationSeconds() const;
    

private:
    AnimationType animationType;
    AnimationDirection animationDirection;
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
    glm::mat4* projection;

    glm::vec2 offset;

    PictureFadeController* pictureFadeController;

    


    float easeOutBack(float t);
    glm::vec2 swing(float t);

    
};

#endif
